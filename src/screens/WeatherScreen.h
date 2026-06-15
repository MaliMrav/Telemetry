#pragma once

#include <Arduino.h>
#include "../models/SensorTile.h"
#include "../display/DisplayManager.h"
#include "../ui/Screen.h"

class WeatherScreen : public Screen {

public:
  explicit WeatherScreen(DisplayManager& display);

  void enter() override; // override the virtual function from Screen
  void leave() override; // override the virtual function from Screen
  void update() override; // override the pure virtual function from Screen

  void onTouch(const TouchEvent& event) override; // override the virtual function from Screen

  void onInput(const InputEvent& event) override; // override the virtual function from Screen

  private:
  DisplayManager& display_;

  bool use12HourClock_ = false;

  void drawHeader();
  void drawWifiQuality();
  void drawSensorGrid();
  void drawTrendArrow(int x, int y, TrendDirection t);

  int8_t getWifiQuality();
  String formatValue(SensorType type, float v);
  DisplayManager::Color getColor(SensorType t);
};
