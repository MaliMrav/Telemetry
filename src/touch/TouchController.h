#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

#include "CalibrationData.h"

class TouchController
{
public:
    TouchController(
        uint8_t csPin,
        uint8_t irqPin);

    bool begin();

    bool loadCalibration();
    bool calibrate(const CalibrationData& data);

    bool isTouched();

    TS_Point getRawPoint();
    TS_Point getPoint();

private:
    bool calculateCalibration(const CalibrationData& data);
    bool saveCalibration();

    XPT2046_Touchscreen touchScreen_;

    float dx_ = 0.0f;
    float dy_ = 0.0f;
    int ax_ = 0;
    int ay_ = 0;
};