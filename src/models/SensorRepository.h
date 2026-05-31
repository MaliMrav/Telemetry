#pragma once

#include <Arduino.h>
#include "SensorTile.h"

class SensorRepository {
public:
    static SensorTile* getTiles();
    static uint8_t getCount();
};