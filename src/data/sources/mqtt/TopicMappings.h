#pragma once

#include <Arduino.h>

#include "../../../data/ObservationKey.h"


// TopicMapping binds a manually configured transport topic
// to a semantic observation.
//
// This table is currently retained for the Weather composition slice.
// Application-defined Energy mappings are generated from telemetry.yaml.

struct TopicMapping
{
    const char* topic;
    ObservationKey observation;

    enum Field
    {
        VALUE,
        MIN,
        MAX,
        TREND
    } field;
};


extern const TopicMapping topicMappings[];
extern const uint8_t TOPIC_COUNT;
