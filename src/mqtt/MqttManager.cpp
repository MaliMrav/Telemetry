#include "MqttManager.h"
#include <EspMQTTClient.h>
#include "../config/settings.h"
#include "../models/SensorTile.h"

static EspMQTTClient client(
  WiFiConfig::SSID,
  WiFiConfig::PASSWORD,
  MQTT::SERVER,
  MQTT::USERNAME,
  MQTT::PASSWORD,
  MQTT::CLIENT,
  MQTT::PORT
);

void onConnectionEstablished() {
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    client.subscribe(sensorTiles[i].topic,
      [i](const String &p) { MqttManager::updateValue(i, p.toFloat()); });
    client.subscribe(sensorTiles[i].minTopic,
      [i](const String &p) { MqttManager::updateMin(i, p.toFloat()); });
    client.subscribe(sensorTiles[i].maxTopic,
      [i](const String &p) { MqttManager::updateMax(i, p.toFloat()); });
    client.subscribe(sensorTiles[i].trendTopic,
      [i](const String &p) { MqttManager::updateTrend(i, p); });
  }
}

void MqttManager::begin() {}

void MqttManager::loop() {
  client.loop();
}

void MqttManager::updateValue(uint8_t i, float v) {
  if (isnan(v)) return;
  sensorTiles[i].value = v;
  sensorTiles[i].valid = true;
}

void MqttManager::updateMin(uint8_t i, float v) {
  if (!isnan(v)) sensorTiles[i].minVal = v;
}

void MqttManager::updateMax(uint8_t i, float v) {
  if (!isnan(v)) sensorTiles[i].maxVal = v;
}

void MqttManager::updateTrend(uint8_t i, const String &p) {
  sensorTiles[i].trend = parseTrend(p);
}

TrendDirection MqttManager::parseTrend(const String &p) {
  String s = p;
  s.trim();
  s.toLowerCase();
  if (s == "up")   return TREND_UP;
  if (s == "down") return TREND_DOWN;
  if (s == "flat") return TREND_FLAT;
  return TREND_NONE;
}
