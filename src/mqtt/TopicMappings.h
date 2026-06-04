#pragma once

#include <Arduino.h>
#include "../models/SensorIds.h"
#include "Topics.h"

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

extern const TopicMapping topicMappings[];
extern const uint8_t TOPIC_COUNT;