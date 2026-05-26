#pragma once
#include <Arduino.h>

enum SensorType {
  TEMP,
  HUMIDITY,
  PRESSURE
};

enum TrendDirection {
  TREND_NONE,
  TREND_UP,
  TREND_DOWN,
  TREND_FLAT
};

struct SensorTile {
  const char *label;
  const char *topic;
  const char *unit;
  SensorType type;

  const char *minTopic;
  const char *maxTopic;
  const char *trendTopic;

  float value;
  float minVal;
  float maxVal;

  TrendDirection trend;
  bool valid;
};

// Expose globally (clean enough for embedded)
extern SensorTile sensorTiles[];
extern const uint8_t SENSOR_COUNT;