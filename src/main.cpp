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

  SensorRepository::initialise();

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
