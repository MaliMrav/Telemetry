#include <Arduino.h>
#include <LittleFS.h>
#define SPIFFS LittleFS

#include "display/DisplayManager.h"
#include "mqtt/MqttManager.h"
#include "ota/OtaManager.h"
#include "models/SensorTile.h"
#include "settings.h"

DisplayManager display;
MqttManager mqtt;
OtaManager ota;

unsigned long lastRedraw = 0;

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();

  display.begin();

  configTime(TimeConfig::TIMEZONE.c_str(), TimeConfig::NTP_SERVERS);

  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    sensorTiles[i].value = NAN;
    sensorTiles[i].minVal = NAN;
    sensorTiles[i].maxVal = NAN;
    sensorTiles[i].trend = TREND_NONE;
    sensorTiles[i].valid = false;
  }

  String hostname(System::HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);

  WiFi.hostname(hostname);

  ota.begin(hostname.c_str());
  mqtt.begin();
}

void loop() {
  ota.loop();
  mqtt.loop();

  if (millis() - lastRedraw > 1000) {
    display.draw();
    lastRedraw = millis();
  }
}