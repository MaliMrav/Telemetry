#include <Arduino.h>

#include "config/settings.h"
#include "models/SensorRepository.h"
#include "system/SystemManager.h"
#include "wifi/WifiSetup.h"
#include "display/DisplayManager.h"
#include "screens/WeatherScreen.h"
#include "ota/OtaManager.h"
#include "mqtt/MqttManager.h"

DisplayManager display;
WeatherScreen weatherScreen(display);
OtaManager ota;
MqttManager mqtt;

uint32_t lastRedraw = 0;

void setup() {
  Serial.begin(115200);

  display.begin();

  const String hostname = SystemManager::getHostname();

  WifiSetup::begin(hostname.c_str());

  configTime(TimeConfig::TIMEZONE.c_str(), TimeConfig::NTP_SERVER);

  SensorTile* tiles = SensorRepository::getTiles();
  for (uint8_t i = 0; i < SensorRepository::getCount(); i++) {
    tiles[i].value  = NAN;
    tiles[i].minVal = NAN;
    tiles[i].maxVal = NAN;
    tiles[i].trend  = TREND_NONE;
    tiles[i].valid  = false;
  }

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
