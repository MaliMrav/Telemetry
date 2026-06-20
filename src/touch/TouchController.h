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

    void loadFactoryDefaults();

    bool loadCalibration();
    bool calibrate(const CalibrationData& data);

    bool isTouched();
    bool isTouched(uint16_t debounceMs);

    bool getTouch(
        int16_t& x,
        int16_t& y,
        uint16_t debounceMs = 250);

    TS_Point getRawPoint();
    TS_Point getPoint();

private:
    bool calculateCalibration(const CalibrationData& data);
    bool saveCalibration();

    XPT2046_Touchscreen touchScreen_;
    unsigned long lastTouched_ = 0;

    float dx_ = 0.0f;
    float dy_ = 0.0f;
    int ax_ = 0;
    int ay_ = 0;
};