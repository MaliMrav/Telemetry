#pragma once

#include <Arduino.h>
#include "../models/SensorTile.h"
// #include "ArialRounded.h"

class DisplayManager;

class WeatherScreen {
public:
  explicit WeatherScreen(DisplayManager& display);

  void draw();

private:
  DisplayManager& display_;

  void drawHeader();
  void drawWifiQuality();
  void drawSensorGrid();
  void drawTrendArrow(int x, int y, TrendDirection t);

  int8_t getWifiQuality();
  String formatValue(SensorType type, float v);
  uint16_t getColor(SensorType t);
};