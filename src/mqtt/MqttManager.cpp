#include "MqttManager.h"
#include <EspMQTTClient.h>
#include "../config/settings.h"
#include "../models/SensorRepository.h"

static EspMQTTClient mqttClient(
  MQTT::SERVER,
  MQTT::PORT,
  MQTT::USERNAME,
  MQTT::PASSWORD,
  MQTT::CLIENT
);

void onConnectionEstablished() {
  MqttManager::onConnected();
}

void MqttManager::begin() {
  mqttClient.enableDebuggingMessages(false);
  mqttClient.setKeepAlive(30);
}

void MqttManager::loop() {
  mqttClient.loop();
}

void MqttManager::onConnected() {
  SensorTile* tiles = SensorRepository::getTiles();
  uint8_t count = SensorRepository::getCount();

  for (uint8_t i = 0; i < count; i++) {
    mqttClient.subscribe(tiles[i].topic,
      [i](const String& p) { updateValue(i, p.toFloat()); });
    mqttClient.subscribe(tiles[i].minTopic,
      [i](const String& p) { updateMin(i, p.toFloat()); });
    mqttClient.subscribe(tiles[i].maxTopic,
      [i](const String& p) { updateMax(i, p.toFloat()); });
    mqttClient.subscribe(tiles[i].trendTopic,
      [i](const String& p) { updateTrend(i, p); });
  }
}

void MqttManager::updateValue(uint8_t i, float v) {
  if (isnan(v)) return;
  SensorRepository::getTiles()[i].value = v;
  SensorRepository::getTiles()[i].valid = true;
}

void MqttManager::updateMin(uint8_t i, float v) {
  if (!isnan(v)) SensorRepository::getTiles()[i].minVal = v;
}

void MqttManager::updateMax(uint8_t i, float v) {
  if (!isnan(v)) SensorRepository::getTiles()[i].maxVal = v;
}

void MqttManager::updateTrend(uint8_t i, const String& p) {
  String s = p;
  s.trim();
  s.toLowerCase();
  if (s == "up")        SensorRepository::getTiles()[i].trend = TREND_UP;
  else if (s == "down") SensorRepository::getTiles()[i].trend = TREND_DOWN;
  else                  SensorRepository::getTiles()[i].trend = TREND_NONE;
}
