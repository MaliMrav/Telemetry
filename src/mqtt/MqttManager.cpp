#include "MqttManager.h"

#include <EspMQTTClient.h>

#include "../config/settings.h"
#include "../models/SensorRepository.h"
#include "../models/SensorTile.h"
#include "../mqtt/Topics.h"

static EspMQTTClient mqttClient(
    MQTT::SERVER,
    MQTT::PORT,
    MQTT::USERNAME,
    MQTT::PASSWORD,
    MQTT::CLIENT
);

struct TopicMapping {

    const char* topic;
    uint8_t sensorIndex;

    enum Type {
        VALUE,
        MIN,
        MAX,
        TREND
    } field;
};

static constexpr TopicMapping topicMappings[] = {

    { Topics::Kitchen::Temp::value, 0, TopicMapping::VALUE },
    { Topics::Kitchen::Temp::min,   0, TopicMapping::MIN   },
    { Topics::Kitchen::Temp::max,   0, TopicMapping::MAX   },
    { Topics::Kitchen::Temp::trend, 0, TopicMapping::TREND },

    { Topics::Pergola::Temp::value, 1, TopicMapping::VALUE },
    { Topics::Pergola::Temp::min,   1, TopicMapping::MIN   },
    { Topics::Pergola::Temp::max,   1, TopicMapping::MAX   },
    { Topics::Pergola::Temp::trend, 1, TopicMapping::TREND },

    { Topics::Kitchen::Hum::value,  2, TopicMapping::VALUE },
    { Topics::Kitchen::Hum::min,    2, TopicMapping::MIN   },
    { Topics::Kitchen::Hum::max,    2, TopicMapping::MAX   },
    { Topics::Kitchen::Hum::trend,  2, TopicMapping::TREND },

    { Topics::Pergola::Hum::value,  3, TopicMapping::VALUE },
    { Topics::Pergola::Hum::min,    3, TopicMapping::MIN   },
    { Topics::Pergola::Hum::max,    3, TopicMapping::MAX   },
    { Topics::Pergola::Hum::trend,  3, TopicMapping::TREND },

    { Topics::Kitchen::Pressure::value, 4, TopicMapping::VALUE },
    { Topics::Kitchen::Pressure::min,   4, TopicMapping::MIN   },
    { Topics::Kitchen::Pressure::max,   4, TopicMapping::MAX   },
    { Topics::Kitchen::Pressure::trend, 4, TopicMapping::TREND }
};

constexpr uint8_t TOPIC_COUNT =
    sizeof(topicMappings) / sizeof(topicMappings[0]);

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

    for (uint8_t i = 0; i < TOPIC_COUNT; i++) {

        const TopicMapping& m = topicMappings[i];

        mqttClient.subscribe(
            m.topic,
            [m](const String& payload) {

                switch (m.field) {

                    case TopicMapping::VALUE:
                        updateValue(
                            m.sensorIndex,
                            payload.toFloat());
                        break;

                    case TopicMapping::MIN:
                        updateMin(
                            m.sensorIndex,
                            payload.toFloat());
                        break;

                    case TopicMapping::MAX:
                        updateMax(
                            m.sensorIndex,
                            payload.toFloat());
                        break;

                    case TopicMapping::TREND:
                        updateTrend(
                            m.sensorIndex,
                            payload);
                        break;
                }
            });
    }
}

void MqttManager::updateValue(uint8_t i, float v) {

    if (isnan(v))
        return;

    SensorRepository::getTiles()[i].value = v;
    SensorRepository::getTiles()[i].valid = true;
}

void MqttManager::updateMin(uint8_t i, float v) {

    if (!isnan(v))
        SensorRepository::getTiles()[i].minVal = v;
}

void MqttManager::updateMax(uint8_t i, float v) {

    if (!isnan(v))
        SensorRepository::getTiles()[i].maxVal = v;
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