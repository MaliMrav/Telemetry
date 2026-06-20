#include <Arduino.h>

#include "config/settings.h"
#include "config/BootProgress.h"
#include "config/CalibrationConfig.h"
#include "config/touch_config.h"

#include "display/DisplayManager.h"

#include "input/InputAction.h"
#include "input/InputEvent.h"
#include "input/InputManager.h"

#include "touch/TouchController.h"
#include "touch/TouchManager.h"

#include "screens/BootScreen.h"
#include "screens/CalibrationScreen.h"
#include "screens/WeatherScreen.h"

#include "ui/ScreenManager.h"

#include "system/SystemManager.h"
#include "system/StatusCallback.h"

#include "wifi/WifiSetup.h"
#include "ota/OtaManager.h"
#include "mqtt/MqttManager.h"

#include "models/SensorRepository.h"

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

uint32_t lastRedraw = 0;
bool calibrationMode = false;
uint32_t calibrationCompleteSince = 0;

//=============================================================================
// Boot status callback
//=============================================================================

void bootStatus(
    const String& message,
    uint8_t progress)
{
    Serial.printf(
        "[BOOT] %u%% %s\n",
        progress,
        message.c_str());

    bootScreen.setStatus(message, progress);
}

//=============================================================================
// Startup helpers
//=============================================================================

static void initialiseFilesystem()
{
    bootStatus(
        "Mounting filesystem",
        BootProgress::FS_MOUNT);

    if (!SystemManager::mountFileSystem())
    {
        bootStatus(
            "Filesystem failed",
            BootProgress::FS_MOUNT);

        delay(3000);
        ESP.restart();
        return;
    }
}

static void initialiseNetwork(
    const String& hostname)
{
    bootStatus(
        "Starting WiFi",
        BootProgress::WIFI_CONNECT);

    WifiSetup::begin(
        hostname.c_str(),
        bootStatus);

    bootStatus(
        "Synchronising time",
        BootProgress::TIME_SYNC);

    configTime(
        TimeConfig::TIMEZONE.c_str(),
        TimeConfig::NTP_SERVER);
}

static void initialiseServices(
    const String& hostname)
{
    bootStatus(
        "Loading sensors",
        BootProgress::SENSORS_INIT);

    SensorRepository::initialise();

    bootStatus(
        "Starting OTA",
        BootProgress::OTA_INIT);

    ota.begin(
        hostname.c_str());

    bootStatus(
        "Starting MQTT",
        BootProgress::MQTT_INIT);

    mqtt.begin();
}

static void initialiseUI()
{
    const bool calibrationLoaded = touchController.loadCalibration();

    calibrationMode =
        CalibrationConfig::FORCE_CALIBRATION ||
        !calibrationLoaded;

    touchManager.setMode(
        calibrationMode ?
            TouchManager::Mode::Calibration :
            TouchManager::Mode::Normal);

    if (calibrationMode)
    {
        screenManager.show(&calibrationScreen);
    }
    else
    {
        screenManager.show(&weatherScreen);
    }

    screenManager.update();
}

static void processInput()
{
    const InputEvent event = InputManager::getEvent();

    if (event.action == InputAction::NONE)
    {
        return;
    }

    screenManager.onInput(event);
    screenManager.update();
    lastRedraw = millis();
}

//=============================================================================
// Arduino setup
//=============================================================================

void setup()
{
    Serial.begin(115200);

    display.begin();

    screenManager.show(&bootScreen);

    bootStatus(
        "Starting display",
        BootProgress::DISPLAY_INIT);

    touchController.begin();

    bootStatus(
        "Starting touch",
        BootProgress::TOUCH_INIT);

    initialiseFilesystem();

    const String hostname =
        SystemManager::getHostname();

    initialiseNetwork(hostname);

    initialiseServices(hostname);

    bootStatus(
        "Ready",
        BootProgress::READY);

    initialiseUI();

    lastRedraw = millis();
}

//=============================================================================
// Arduino loop
//=============================================================================

void loop()
{
    ota.loop();
    mqtt.loop();

    touchManager.update();
    processInput();

    if (calibrationMode &&
        calibrationScreen.isFinished() &&
        calibrationScreen.calibrationSaved())
    {
        if (calibrationCompleteSince == 0)
        {
            calibrationCompleteSince = millis();
        }
        else if (millis() - calibrationCompleteSince > 2000)
        {
            calibrationMode = false;
            touchManager.setMode(TouchManager::Mode::Normal);
            screenManager.show(&weatherScreen);
            screenManager.update();
            lastRedraw = millis();
        }
    }
    else
    {
        calibrationCompleteSince = 0;
    }

    if (millis() - lastRedraw >= 1000)
    {
        screenManager.update();
        lastRedraw = millis();
    }
}