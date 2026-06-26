#pragma once

// SensorRepository is the single source of truth for all sensor data.
//
// Responsibilities:
//   - own the array of SensorTiles (one per SensorId)
//   - expose a typed write API so data sources update values by SensorId
//   - expose a read API for screens to retrieve tiles for display
//
// The write API (setValue, setMin, setMax, setTrend) is the only legitimate
// way for data sources to mutate tile state. Direct array access from
// outside this module is intentionally not supported.

#include <Arduino.h>
#include "SensorTile.h"
#include "SensorIds.h"

namespace SensorRepository
{
    void        initialise();
    uint8_t     getCount();
    SensorTile* getTiles();

    // Typed write API — the only way data sources should write to the repository.
    // SensorId is the stable identity; data sources never touch tile indices directly.
    void setValue(SensorId id, float value);
    void setMin  (SensorId id, float value);
    void setMax  (SensorId id, float value);
    void setTrend(SensorId id, TrendDirection trend);
}