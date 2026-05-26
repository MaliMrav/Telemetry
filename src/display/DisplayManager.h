#pragma once

#include <MiniGrafx.h>
#include <ILI9341_SPI.h>
#include "../models/SensorTile.h"

class DisplayManager {
public:
  void begin();
  void draw();

private:
  void drawHeader();
  void drawWifiQuality();
  void drawSensorGrid();
  void drawTrendArrow(int x, int y, TrendDirection t);

  String formatValue(SensorType type, float v);
  uint16_t getColor(SensorType t);
};
