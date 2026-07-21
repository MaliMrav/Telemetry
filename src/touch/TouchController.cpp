#include "TouchController.h"

#include "../config/config.h"

TouchController::TouchController(
    uint8_t csPin,
    uint8_t irqPin)
:
    touchScreen_(csPin, irqPin)
{
}

bool TouchController::begin()
{
    touchScreen_.begin();
    return true;
}

bool TouchController::loadCalibration()
{
    File f = LittleFS.open(TouchConfig::CALIBRATION_FILE, "r");
    if (!f)
    {
        return false;
    }

    dx_ = f.parseFloat();
    dy_ = f.parseFloat();
    ax_ = f.parseInt();
    ay_ = f.parseInt();

    f.close();
    return true;
}

bool TouchController::calibrate(const CalibrationData& data)
{
    if (!calculateCalibration(data))
    {
        return false;
    }

    return saveCalibration();
}

bool TouchController::calculateCalibration(const CalibrationData& data)
{
    // The XPT2046 raw axes are rotated 90 degrees relative to the ILI9341
    // screen coordinates:
    //   raw.x  maps to screen Y
    //   raw.y  maps to screen X
    //
    // Average opposite edges to get stable reference values at each boundary,
    // then derive scale factors and offsets.

    // raw.y values at the left and right screen edges
    const float rawYatLeft  = (data.topLeft.y  + data.bottomLeft.y)  * 0.5f;
    const float rawYatRight = (data.topRight.y + data.bottomRight.y) * 0.5f;

    // raw.x values at the top and bottom screen edges
    const float rawXatTop    = (data.topLeft.x  + data.topRight.x)    * 0.5f;
    const float rawXatBottom = (data.bottomLeft.x + data.bottomRight.x) * 0.5f;

    // Determine direction: raw.y may increase left-to-right or right-to-left
    // depending on hardware orientation. Store the sign in the scale factor.
    const float rawWidth  = rawYatLeft - rawYatRight;
    const float rawHeight = rawXatBottom - rawXatTop;

    if (rawWidth == 0.0f || rawHeight == 0.0f)
    {
        return false;
    }

    // dx_ and dy_ carry the sign, so getPoint() always uses the same formula
    dx_ = static_cast<float>(TouchConfig::SCREEN_WIDTH)  / rawWidth;
    dy_ = static_cast<float>(TouchConfig::SCREEN_HEIGHT) / rawHeight;

    ax_ = static_cast<int>(rawYatLeft);
    ay_ = static_cast<int>(rawXatTop);

    return true;
}

bool TouchController::saveCalibration()
{
    File f = LittleFS.open(TouchConfig::CALIBRATION_FILE, "w");
    if (!f)
    {
        return false;
    }

    f.println(dx_);
    f.println(dy_);
    f.println(ax_);
    f.println(ay_);

    f.close();
    return true;
}

bool TouchController::isTouched()
{
    return touchScreen_.touched();
}

TS_Point TouchController::getRawPoint()
{
    return touchScreen_.getPoint();
}

TS_Point TouchController::getPoint()
{
    TS_Point raw = touchScreen_.getPoint();

    TS_Point p;
    p.x = static_cast<int16_t>((ax_ - raw.y) * dx_);
    p.y = static_cast<int16_t>((raw.x - ay_) * dy_);
    p.z = raw.z;

    return p;
}