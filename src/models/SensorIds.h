#pragma once

// SensorId is the stable identity for every sensor tile in the system.
//
// Data sources write to the repository using SensorId, not array indices.
// This keeps data sources decoupled from the repository's internal layout.
//
// To add a sensor: add its id here before SENSOR_COUNT, add its display
// tile in SensorRepository.cpp, and add its data bindings in the relevant
// data source (e.g. TopicMappings.cpp for MQTT).

enum SensorId {

    SENSOR_KITCHEN_TEMP,
    SENSOR_PERGOLA_TEMP,

    SENSOR_KITCHEN_HUM,
    SENSOR_PERGOLA_HUM,

    SENSOR_PRESSURE,

    SENSOR_COUNT
};