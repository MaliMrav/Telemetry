#pragma once
#include "../models/SensorTile.h"

class MqttManager {
public:
  void begin();
  void loop();

  static void updateValue(uint8_t i, float v);
  static void updateMin(uint8_t i, float v);
  static void updateMax(uint8_t i, float v);
  static void updateTrend(uint8_t i, const String &p);
  static TrendDirection parseTrend(const String &p);
};
