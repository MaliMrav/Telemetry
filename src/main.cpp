#include <Arduino.h>

#include "config/touch_config.h"

#include "display/DisplayManager.h"

#include "touch/TouchController.h"
#include "touch/TouchManager.h"

#include "screens/BootScreen.h"
#include "screens/CalibrationScreen.h"
#include "screens/WeatherScreen.h"

#include "ui/ScreenManager.h"

#include "system/SystemManager.h"

#include "ota/OtaManager.h"
#include "mqtt/MqttManager.h"

//=============================================================================
// Global objects
//=============================================================================

DisplayManager display;

TouchController touchController(
    TouchConfig::TOUCH_CS,
    TouchConfig::TOUCH_IRQ);

TouchManager touchManager(touchController);

ScreenManager screenManager;

BootScreen bootScreen(display);
WeatherScreen weatherScreen(display);
CalibrationScreen calibrationScreen(
    display,
    touchController);

OtaManager ota;
MqttManager mqtt;

//=============================================================================
// Arduino setup
//=============================================================================

void setup()
{
    Serial.begin(115200);

    SystemManager::begin(
        display,
        touchController,
        touchManager,
        screenManager,
        bootScreen,
        weatherScreen,
        calibrationScreen,
        ota,
        mqtt);
}

//=============================================================================
// Arduino loop
//=============================================================================

void loop()
{
    SystemManager::update();
}