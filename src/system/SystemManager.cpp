#include "SystemManager.h"
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "../config/settings.h"

namespace SystemManager {

    String getHostname() { // Generate a unique hostname using the chip ID

        return String(System::HOSTNAME) +
               String(ESP.getChipId(), HEX);
    }

    bool mountFileSystem() { // Try to mount the file system, if it fails (e.g. due to corruption) try to format it and mount again

        if (LittleFS.begin()) {

            Serial.println(
                "LittleFS mounted");

            return true;
        }

        Serial.println(
            "LittleFS mount failed");

        Serial.println(
            "Formatting LittleFS...");

        if (!LittleFS.format()) {

            Serial.println(
                "LittleFS format failed");

            return false;
        }

        return LittleFS.begin();
    }
}