#pragma once

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