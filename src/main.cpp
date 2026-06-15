#include <Arduino.h>

#include "config/settings.h"
#include "config/BootProgress.h"
#include "screens/BootScreen.h"
#include "config/touch_config.h"

#include "display/DisplayManager.h"
#include "touch/TouchController.h"

#include "system/SystemManager.h"
#include "wifi/WifiSetup.h"

#include "models/SensorRepository.h"

#include "screens/WeatherScreen.h"

#include "ui/ScreenManager.h"

#include "ota/OtaManager.h"
#include "mqtt/MqttManager.h"

#include "system/StatusCallback.h"

DisplayManager display;

TouchController touchController(
    TouchConfig::TOUCH_CS,
    TouchConfig::TOUCH_IRQ);

ScreenManager screenManager; // Global instance of the screen manager
BootScreen bootScreen(display); // Global instance of the boot screen
WeatherScreen weatherScreen(display); // Global instance of the weather screen, which will be managed by the screen manager

OtaManager ota;
MqttManager mqtt;

uint32_t lastRedraw = 0;

// This helper function is used as a callback to report the boot status. It prints the status to the serial console and also updates the display with the current progress and message.
void bootStatus(
    const String& message,
    uint8_t progress)
{
    Serial.printf(
        "[BOOT] %u%% %s\n",
        progress,
        message.c_str());

    bootScreen.setStatus(message, progress);
    screenManager.update(); // Ensure the screen is updated immediately to reflect the new status on the display
}

void setup() {

    Serial.begin(115200);

    display.begin();

    screenManager.show(
        &bootScreen); // Show the boot screen immediately to provide feedback during the boot process

    bootStatus(
        "Starting display",
        BootProgress::DISPLAY_INIT);

    touchController.begin();
    
    bootStatus(
        "Starting touch",
        BootProgress::TOUCH_INIT);
    
    bool fsMounted =
        SystemManager::mountFileSystem();
    
    if (!fsMounted)
    {
        bootStatus(
            "Filesystem failed",
            BootProgress::FS_MOUNT);

        delay(3000);
    
        ESP.restart();
        return;
    }

    if (!touchController.loadCalibration())
    {
        bootStatus(
            "Loading default touch calibration",
            BootProgress::FS_MOUNT);
    
        touchController.loadFactoryDefaults();
    
        touchController.saveCalibration();
    }
    
    bootStatus(
        "Filesystem mounted",
        BootProgress::FS_MOUNT);
    
    const String hostname =
        SystemManager::getHostname();
        
    WifiSetup::begin(
        hostname.c_str(),
        bootStatus);
    
    bootStatus(
        "Synchronizing time",
        BootProgress::TIME_SYNC);
    
    configTime(
        TimeConfig::TIMEZONE.c_str(),
        TimeConfig::NTP_SERVER);
    
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
    
    bootStatus(
        "Ready",
        BootProgress::READY);
    
    delay(500);

    screenManager.show(
        &weatherScreen);
    
    screenManager.update();

    lastRedraw = millis();

}

void loop()
{
    ota.loop();
    mqtt.loop();

    if (millis() - lastRedraw > 1000)
    {
        screenManager.update();

        lastRedraw = millis();
    }

    int16_t x;
    int16_t y;

    if (touchController.getTouch(x, y))
    {
        TouchEvent event
        {
            x,
            y
        };

        screenManager.onTouch(event);
    }
}
