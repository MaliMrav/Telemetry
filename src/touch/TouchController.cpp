#include "TouchController.h"
#include "../config/DefaultTouchCalibration.h"

#include<Arduino.h> // Temporarily added to prevent compilation errors in VSCode, should be removed when the project is properly configured with platformio  


TouchController::TouchController(
    uint8_t csPin,
    uint8_t irqPin)
:
    touchScreen_(csPin, irqPin)
{
}

bool TouchController::begin() {

    touchScreen_.begin();

    return true;
} //

// Loads factory default calibration values. This can be used to reset the calibration if needed.
void TouchController::loadFactoryDefaults()
{
    dx_ =
        DefaultTouchCalibration::DX;

    dy_ =
        DefaultTouchCalibration::DY;

    ax_ =
        DefaultTouchCalibration::AX;

    ay_ =
        DefaultTouchCalibration::AY;
}

// Loads calibration data from the file system. Returns true if successful, false otherwise (e.g. if the file doesn't exist or is invalid).
bool TouchController::loadCalibration() {

    File f =
        LittleFS.open(
            "/calibration.txt",
            "r");

    if (!f) {
        return false;
    }

    dx_ = f.readStringUntil('\n').toFloat();
    dy_ = f.readStringUntil('\n').toFloat();
    ax_ = f.readStringUntil('\n').toInt();
    ay_ = f.readStringUntil('\n').toInt();

    f.close();

    return true;
}

// Saves calibration data to the file system. Returns true if successful, false otherwise (e.g. if the file system is not mounted or there's an error writing to the file).
bool TouchController::saveCalibration() {

    File f =
        LittleFS.open(
            "/calibration.txt",
            "w");

    if (!f) {
        return false;
    }

    f.println(dx_);
    f.println(dy_);
    f.println(ax_);
    f.println(ay_);

    f.close();

    return true;
}

void TouchController::startCalibration(
    CalibrationCallback callback) {

    calibrationCallback_ = callback;

    state_ = 0;

    lastStateChange_ =
        millis();
}

bool TouchController::continueCalibration() {

    TS_Point p =
        touchScreen_.getPoint();

    switch (state_) {

        case 0:

            if (calibrationCallback_) {
                calibrationCallback_(10, 10);
            }

            if (touchScreen_.touched()) {

                p1_ = p;

                state_ = 1;

                lastStateChange_ =
                    millis();
            }

            break;

        case 1:

            if (calibrationCallback_) {
                calibrationCallback_(230, 310);
            }

            if (touchScreen_.touched() &&
                millis() - lastStateChange_ > 1000) {

                p2_ = p;

                dx_ =
                    240.0f /
                    abs(p1_.y - p2_.y);

                dy_ =
                    320.0f /
                    abs(p1_.x - p2_.x);

                ax_ =
                    min(
                        p1_.y,
                        p2_.y);

                ay_ =
                    min(
                        p1_.x,
                        p2_.x);

                state_ = 2;
            }

            break;
    }

    return state_ == 2;
}

// Returns true if the screen is currently being touched, false otherwise.
bool TouchController::isCalibrationFinished() const {

    return state_ == 2;
}

// Touch detection. Returns true if the screen is currently being touched, false otherwise.
bool TouchController::isTouched() {

    return touchScreen_.touched();
}

// Touch detection with debounce. Returns true if the screen is currently being touched and the last touch was more than debounceMs milliseconds ago, false otherwise.
bool TouchController::isTouched(
    uint16_t debounceMs) {

    if (!touchScreen_.touched()) {
        return false;
    }

    if (millis() - lastTouched_
        < debounceMs) {

        return false;
    }

    lastTouched_ = millis();

    return true;
}

// Touch detection with debounce and coordinate retrieval. Returns true if the screen is currently being touched and the last touch was more than debounceMs milliseconds ago, false otherwise. If a touch is detected, the provided x and y references are updated with the calibrated touch coordinates.
bool TouchController::getTouch(
    int16_t& x,
    int16_t& y,
    uint16_t debounceMs)
{
    if (!isTouched(debounceMs))
    {
        return false;
    }

    TS_Point p =
        getPoint();

    x = p.x;
    y = p.y;

    return true;
}

TS_Point TouchController::getRawPoint()
{
    return touchScreen_.getPoint();
}

/*
TS_Point TouchController::getPoint() {

    TS_Point p =
        touchScreen_.getPoint();

    int x =
        (p.y - ax_) * dx_;

    int y =
        320 -
        ((p.x - ay_) * dy_);

    p.x = x;
    p.y = y;

    return p;
}
*/

TS_Point TouchController::getPoint()
{
    // TEMPORARY calibration transform.
    // Derived from measured corner values.
    // Will be replaced by the 4-point calibration system.
    TS_Point raw =
        touchScreen_.getPoint();

    TS_Point p;

    p.x =
        ((3559 - raw.y) * 240) /
        2977;

    p.y =
        ((raw.x - 425) * 320) /
        3293;

    return p;
}