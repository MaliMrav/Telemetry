#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "config/settings.h"
#include "models/SensorTile.h"
#include "display/DisplayManager.h"
#include "screens/WeatherScreen.h"
#include "mqtt/MqttManager.h"
#include "ota/OtaManager.h"

DisplayManager display;
WeatherScreen weatherScreen(display);
MqttManager mqtt;
OtaManager ota;

unsigned long lastRedraw = 0;

void setup() {
  Serial.begin(115200);

  display.begin();

  configTime(TimeConfig::TIMEZONE.c_str(), TimeConfig::NTP_SERVER);

  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    sensorTiles[i].value  = NAN;
    sensorTiles[i].minVal = NAN;
    sensorTiles[i].maxVal = NAN;
    sensorTiles[i].trend  = TREND_NONE;
    sensorTiles[i].valid  = false;
  }

  String hostname = String(System::HOSTNAME) + String(ESP.getChipId(), HEX);
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
