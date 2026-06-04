#include "MqttManager.h"

#include <EspMQTTClient.h>

#include "../config/settings.h"
#include "../models/SensorRepository.h"
#include "../mqtt/Topics.h"
#include "../mqtt/TopicMappings.h"

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
  for (uint8_t i = 0; i < TOPIC_COUNT; ++i) {
    const TopicMapping& m = topicMappings[i];

    mqttClient.subscribe(
      m.topic,
      [m](const String& payload) {
        switch (m.field) {
          case TopicMapping::VALUE:
            updateValue(m.sensorIndex, payload.toFloat());
            break;
          case TopicMapping::MIN:
            updateMin(m.sensorIndex, payload.toFloat());
            break;
          case TopicMapping::MAX:
            updateMax(m.sensorIndex, payload.toFloat());
            break;
          case TopicMapping::TREND:
            updateTrend(m.sensorIndex, payload);
            break;
        }
      }
    );
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

void MqttManager::updateTrend(uint8_t i, const String& payload) {
  String s = payload;
  s.trim();
  s.toLowerCase();

  if (s == "up")
    SensorRepository::getTiles()[i].trend = TREND_UP;
  else if (s == "down")
    SensorRepository::getTiles()[i].trend = TREND_DOWN;
  else if (s == "flat")
    SensorRepository::getTiles()[i].trend = TREND_FLAT;
  else
    SensorRepository::getTiles()[i].trend = TREND_NONE;
}