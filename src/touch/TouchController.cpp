#include "TouchController.h"

#include "../config/touch_config.h"

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
    const float leftY =
        (data.topLeft.y + data.bottomLeft.y) * 0.5f;

    const float rightY =
        (data.topRight.y + data.bottomRight.y) * 0.5f;

    const float topX =
        (data.topLeft.x + data.topRight.x) * 0.5f;

    const float bottomX =
        (data.bottomLeft.x + data.bottomRight.x) * 0.5f;

    const float rawWidth  = leftY - rightY;
    const float rawHeight = bottomX - topX;

    if (rawWidth <= 0.0f || rawHeight <= 0.0f)
    {
        return false;
    }

    dx_ = static_cast<float>(TouchConfig::SCREEN_WIDTH) / rawWidth;
    dy_ = static_cast<float>(TouchConfig::SCREEN_HEIGHT) / rawHeight;

    ax_ = static_cast<int>(leftY);
    ay_ = static_cast<int>(topX);

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