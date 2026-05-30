#pragma once

struct WeatherData {
  float temperature;
  float humidity;
  String condition;
  String icon;

  bool updated = false;
};