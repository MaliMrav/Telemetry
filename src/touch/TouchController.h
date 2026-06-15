#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

typedef void (*CalibrationCallback)(int16_t x, int16_t y);

class TouchController {

public:

    TouchController(uint8_t csPin,
                    uint8_t irqPin);

    bool begin();

    void loadFactoryDefaults();

    bool loadCalibration();
    bool saveCalibration();

    void startCalibration(CalibrationCallback callback); // Reserved for future CalibrationScreen
    bool continueCalibration();
    bool isCalibrationFinished() const;

    bool isTouched();
    bool isTouched(uint16_t debounceMs);

    bool getTouch(int16_t& x,int16_t& y,uint16_t debounceMs = 250); // Returns true if a touch is detected and the coordinates are updated, false otherwise

    TS_Point getRawPoint();

    TS_Point getPoint();


private:

    XPT2046_Touchscreen touchScreen_;

    CalibrationCallback calibrationCallback_ = nullptr;

    float dx_ = 0.0f;
    float dy_ = 0.0f;

    int ax_ = 0;
    int ay_ = 0;

    uint8_t state_ = 0;

    unsigned long lastStateChange_ = 0;
    unsigned long lastTouched_ = 0;

    TS_Point p1_;
    TS_Point p2_;
};