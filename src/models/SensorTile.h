#pragma once

// SensorTile is the runtime presentation model for a single observation.
//
// It deliberately contains no domain-specific value type such as:
//   power, energy, temperature, humidity, etc.
//
// Semantic meaning belongs to the ObservationDefinition generated from
// telemetry.yaml.
//
// SensorTile contains only:
//   - presentation identity
//   - canonical unit
//   - runtime value
//   - min/max
//   - trend
//   - validity
//   - generic display scaling metadata
//   - generic display indicator metadata
//
// This keeps the runtime data model independent of application domains.

#include <Arduino.h>

#include "SensorPresentation.h"


enum TrendDirection
{
    TREND_NONE,
    TREND_UP,
    TREND_DOWN,
    TREND_FLAT
};


struct SensorTile
{
    const char* label = nullptr;
    const char* unit = nullptr;

    float value = NAN;
    float minVal = NAN;
    float maxVal = NAN;

    TrendDirection trend = TREND_NONE;
    bool valid = false;

    const SensorDisplayScale* displayScales = nullptr;
    uint8_t displayScaleCount = 0;

    SensorDisplayIndicator displayIndicator =
        SensorDisplayIndicator::NONE;
};
