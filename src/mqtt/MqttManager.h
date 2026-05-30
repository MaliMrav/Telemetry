#pragma once

#include <EspMQTTClient.h>

class MqttManager {
public:
    MqttManager();

    void begin();
    void loop();

private:
    EspMQTTClient mqttClient;

    void subscribeTopics();

    static void updateValue(uint8_t index, float value);
    static void updateMin(uint8_t index, float value);
    static void updateMax(uint8_t index, float value);
    static void updateTrend(uint8_t index, const String& payload);
};
