#pragma once

#include <Arduino.h>

class DisplayManager;
class TouchController;
class TouchManager;
class ScreenManager;
class BootScreen;
class WeatherScreen;
class CalibrationScreen;
class OtaManager;
class MqttManager;

namespace SystemManager
{
    bool mountFileSystem();

    String getHostname();

    void begin(
        DisplayManager& display,
        TouchController& touchController,
        TouchManager& touchManager,
        ScreenManager& screenManager,
        BootScreen& bootScreen,
        WeatherScreen& weatherScreen,
        CalibrationScreen& calibrationScreen,
        OtaManager& ota,
        MqttManager& mqtt);

    void update();
}