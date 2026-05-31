#include "WeatherScreen.h"
#include "../display/DisplayManager.h"
#include <ArialRounded.h>
#include "../models/SensorRepository.h"
#include "../config/settings.h"

#include <ESP8266WiFi.h>
#include <time.h>

WeatherScreen::WeatherScreen(DisplayManager& display)
  : display_(display) {}

void WeatherScreen::draw() {
  auto& gfx = display_.getGfx();

  gfx.fillBuffer(MINI_BLACK);
  drawHeader();
  drawWifiQuality();
  drawSensorGrid();
  gfx.commit();
}

void WeatherScreen::drawHeader() {
  auto& gfx = display_.getGfx();

  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(MINI_WHITE);

  if (!t) return;

  gfx.setFont(ArialRoundedMTBold_14);
  String date = TimeConfig::WDAY_NAMES[t->tm_wday] + " " +
                TimeConfig::MONTH_NAMES[t->tm_mon] + " " +
                String(t->tm_mday);
  gfx.drawString(120, 6, date);

  gfx.setFont(ArialRoundedMTBold_36);
  char buf[10];
  sprintf(buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
  gfx.drawString(120, 20, buf);
}

int8_t WeatherScreen::getWifiQuality() {
  int32_t dbm = WiFi.RSSI();
  if (dbm <= -100) return 0;
  if (dbm >= -50) return 100;
  return 2 * (dbm + 100);
}

void WeatherScreen::drawWifiQuality() {
  auto& gfx = display_.getGfx();
  int8_t q = getWifiQuality();

  gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
  gfx.setFont(ArialMT_Plain_10);
  gfx.setColor(MINI_WHITE);
  gfx.drawString(228, 9, String(q) + "%");

  for (int8_t i = 0; i < 4; i++) {
    for (int8_t j = 0; j < 2 * (i + 1); j++) {
      if (q > i * 25 || j == 0) {
        gfx.setPixel(230 + 2 * i, 18 - j);
      }
    }
  }
}

void WeatherScreen::drawSensorGrid() {
  SensorTile* tiles =
    SensorRepository::getTiles();

  uint8_t sensorCount =
    SensorRepository::getCount();
  auto& gfx = display_.getGfx();

  const int margin = 8;
  const int gap = 8;
  const int topY = 60;

  const int cols = 2;
  const int rows = (sensorCount + 1) / 2;

  const int tileW = (gfx.getWidth() - (2 * margin) - gap) / 2;
  const int tileH = (gfx.getHeight() - topY - (rows * gap)) / rows;

  for (uint8_t i = 0; i < sensorCount; i++) {
    uint8_t row = i / cols;
    uint8_t col = i % cols;

    bool full = (i == sensorCount - 1);

    int x = full ? margin : margin + col * (tileW + gap);
    int w = full ? gfx.getWidth() - (2 * margin) : tileW;
    int y = topY + row * (tileH + gap);

    SensorTile &s = tiles[i];

    gfx.setColor(MINI_WHITE);
    gfx.drawRect(x, y, w, tileH);

    gfx.setFont(ArialRoundedMTBold_14);
    gfx.setColor(getColor(s.type));
    gfx.setTextAlignment(TEXT_ALIGN_CENTER);
    gfx.drawString(x + w / 2, y + 10, s.label);

    gfx.setFont(ArialMT_Plain_24);

    String valueStr = formatValue(s.type, s.value) + s.unit;
    int valueWidth = gfx.getStringWidth(valueStr.c_str(), valueStr.length());

    const int arrowOffset = 12;
    const int arrowWidth  = 10;
    const int spacing     = 6;
    const int rightPad    = 6;

    int arrowCenterX = x + arrowOffset;

    int contentLeft  = x + arrowOffset + arrowWidth + spacing;
    int contentRight = x + w - rightPad;
    int contentWidth = contentRight - contentLeft;

    int cy = y + tileH / 2 - 8;
    int valueStartX = contentLeft + (contentWidth - valueWidth) / 2;

    if (s.trend != TREND_NONE) {
      gfx.setColor(
        s.trend == TREND_UP   ? MINI_YELLOW :
        s.trend == TREND_DOWN ? MINI_BLUE   :
                                MINI_WHITE
      );

      drawTrendArrow(arrowCenterX, cy + 10, s.trend);
    }

    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    gfx.drawString(valueStartX, cy, valueStr);

    gfx.setTextAlignment(TEXT_ALIGN_CENTER);

    gfx.setFont(ArialMT_Plain_10);
    gfx.setColor(MINI_BLUE);

    if (!isnan(s.minVal) && !isnan(s.maxVal)) {
      String mm = formatValue(s.type, s.minVal) + " / " +
                  formatValue(s.type, s.maxVal);
      gfx.drawString(x + w / 2, y + tileH - 18, mm);
    } else {
      gfx.drawString(x + w / 2, y + tileH - 18, "-- / --");
    }
  }
}

void WeatherScreen::drawTrendArrow(int x, int y, TrendDirection t) {
  auto& gfx = display_.getGfx();

  auto thick = [&](int x1, int y1, int x2, int y2) {
    gfx.drawLine(x1, y1, x2, y2);
    gfx.drawLine(x1 + 1, y1, x2 + 1, y2);
  };

  switch (t) {
    case TREND_UP:
      thick(x, y + 6, x, y - 6);
      thick(x, y - 6, x - 3, y - 2);
      thick(x, y - 6, x + 3, y - 2);
      break;

    case TREND_DOWN:
      thick(x, y - 6, x, y + 6);
      thick(x, y + 6, x - 3, y + 2);
      thick(x, y + 6, x + 3, y + 2);
      break;

    case TREND_FLAT:
      thick(x - 3, y, x + 3, y);
      break;

    default:
      break;
  }
}

String WeatherScreen::formatValue(SensorType type, float v) {
  if (isnan(v)) return "--";

  switch (type) {
    case TEMP:
      return String(v, 1);
    case HUMIDITY:
    case PRESSURE:
      return String((int)round(v));
  }
  return "--";
}

uint16_t WeatherScreen::getColor(SensorType t) {
  if (t == HUMIDITY) return MINI_BLUE;
  if (t == PRESSURE) return MINI_YELLOW;
  return MINI_WHITE;
}
