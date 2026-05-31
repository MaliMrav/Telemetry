#include "MqttManager.h"

#include "../config/settings.h"
#include "../models/SensorTile.h"
#include "../models/SensorRepository.h"

#if MQTT_USE_AUTH

MqttManager::MqttManager()
    : mqttClient(
        WiFiConfig::SSID,
        WiFiConfig::PASSWORD,
        MQTT::SERVER,
        MQTT::USERNAME,
        MQTT::PASSWORD,
        MQTT::CLIENT,
        MQTT::PORT)
{
}

#else

MqttManager::MqttManager()
    : mqttClient(
        WiFiConfig::SSID,
        WiFiConfig::PASSWORD,
        MQTT::SERVER,
        MQTT::CLIENT,
        MQTT::PORT)
{
}

#endif

void MqttManager::begin() {
    mqttClient.enableDebuggingMessages(false);
    mqttClient.enableLastWillMessage("lwt", "offline");
    mqttClient.setKeepAlive(30);
}

void MqttManager::loop() {

    mqttClient.loop();

    if (mqttClient.isConnected()) {

        static bool subscribed = false;

        if (!subscribed) {

            subscribeTopics();
            subscribed = true;
        }

    }
}

void MqttManager::subscribeTopics() {

    SensorTile* tiles = SensorRepository::getTiles();
    for (uint8_t i = 0;
         i < SensorRepository::getCount();
         i++) {

        mqttClient.subscribe(
            tiles[i].topic,
            [i](const String& payload) {
                updateValue(i, payload.toFloat());
            });

        mqttClient.subscribe(
            tiles[i].minTopic,
            [i](const String& payload) {
                updateMin(i, payload.toFloat());
            });

        mqttClient.subscribe(
            tiles[i].maxTopic,
            [i](const String& payload) {
                updateMax(i, payload.toFloat());
            });

        mqttClient.subscribe(
            tiles[i].trendTopic,
            [i](const String& payload) {
                updateTrend(i, payload);
            });
    }
}

void MqttManager::updateValue(uint8_t index, float value) {
    if (isnan(value)) return;
    SensorRepository::getTiles()[index].value = value;
    SensorRepository::getTiles()[index].valid = true;
}

void MqttManager::updateMin(uint8_t index, float value) {
    if (!isnan(value)) SensorRepository::getTiles()[index].minVal = value;
}

void MqttManager::updateMax(uint8_t index, float value) {
    if (!isnan(value)) SensorRepository::getTiles()[index].maxVal = value;
}

void MqttManager::updateTrend(uint8_t index, const String& payload) {
    String s = payload;
    s.trim();
    s.toLowerCase();
    if (s == "up")        SensorRepository::getTiles()[index].trend = TREND_UP;
    else if (s == "down") SensorRepository::getTiles()[index].trend = TREND_DOWN;
    else                  SensorRepository::getTiles()[index].trend = TREND_NONE;
}

// Required by EspMQTTClient - subscriptions are handled via polling in loop()
void onConnectionEstablished() {}
