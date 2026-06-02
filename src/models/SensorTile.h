#pragma once

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

  const char* label;
  const char* unit;

  SensorType type;

  float value;
  float minVal;
  float maxVal;

  TrendDirection trend;
  bool valid;
};