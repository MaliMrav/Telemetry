#pragma once

#include <Arduino.h>
#include "../models/SensorTile.h"

namespace SensorRepository {
  SensorTile* getTiles();
  uint8_t getCount();
  void initialise();
}