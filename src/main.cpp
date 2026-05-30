#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "config/settings.h"
#include "models/SensorTile.h"
#include "display/DisplayManager.h"
#include "screens/WeatherScreen.h"
#include "mqtt/MqttManager.h"
#include "ota/OtaManager.h"
#include "system/SystemManager.h"

DisplayManager display;
WeatherScreen weatherScreen(display);
MqttManager mqtt;
OtaManager ota;

unsigned long lastRedraw = 0;

void setup() {
  Serial.begin(115200);

  display.begin();

  configTime(TimeConfig::TIMEZONE.c_str(), TimeConfig::NTP_SERVER);

  SensorTile* tiles =
    SensorRepository::getTiles();

    for (uint8_t i = 0;
         i < SensorRepository::getCount();
         i++) {
    tiles[i].value  = NAN;
    tiles[i].minVal = NAN;
    tiles[i].maxVal = NAN;
    tiles[i].trend  = TREND_NONE;
    tiles[i].valid  = false;
  }

  const String hostname = SystemManager::getHostname();
  WiFi.hostname(hostname);

  ota.begin(hostname.c_str());
  mqtt.begin();
}

void loop() {
  ota.loop();
  mqtt.loop();

  if (millis() - lastRedraw > 1000) {
    weatherScreen.draw();
    lastRedraw = millis();
  }
}
