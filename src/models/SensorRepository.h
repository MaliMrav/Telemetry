#pragma once

#include <Arduino.h>
#include "SensorTile.h"

namespace SensorRepository {

  SensorTile* getTiles();
  uint8_t getCount();

  void initialize();

}