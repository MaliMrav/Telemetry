#pragma once

#include <Arduino.h>
#include "../models/SensorIds.h"
#include "Topics.h"

// TopicMapping binds an MQTT topic string to a SensorId and field type.
// MqttDataSource iterates this table on every incoming message.
// To add a new sensor: add a SensorId, add its tile in SensorRepository,
// and add its topic bindings here.

struct TopicMapping
{
    const char* topic;
    SensorId    sensorId;

    enum Field { VALUE, MIN, MAX, TREND } field;
};

extern const TopicMapping topicMappings[];
extern const uint8_t      TOPIC_COUNT;