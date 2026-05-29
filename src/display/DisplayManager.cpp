#include "DisplayManager.h"
#include "../config/settings.h"
#include <ESP8266WiFi.h>
#include <time.h>

#define MINI_BLACK  0
#define MINI_WHITE  1
#define MINI_YELLOW 2
#define MINI_BLUE   3

uint16_t palette[] = {
  ILI9341_BLACK,
  ILI9341_WHITE,
  ILI9341_YELLOW,
  0x7E3C
};

ILI9341_SPI tft(Pins::TFT_CS, Pins::TFT_DC);
MiniGrafx gfx(&tft, 2, palette);

// begin()
void DisplayManager::begin() {
  pinMode(Pins::TFT_LED, OUTPUT);
  digitalWrite(Pins::TFT_LED, LOW);

  gfx.init();
  gfx.fillBuffer(MINI_BLACK);
  gfx.commit();
}

// draw()
void DisplayManager::draw() {
  gfx.fillBuffer(MINI_BLACK);
  drawHeader();
  drawWifiQuality();
  drawSensorGrid();
  gfx.commit();
}

// **************************************************
//                Draw Header
// **************************************************

void DisplayManager::drawHeader() {
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

// *************************
//    Get WiFi Quality
// *************************
int8_t DisplayManager::getWifiQuality() {
  int32_t dbm = WiFi.RSSI();
  if (dbm <= -100) return 0;
  if (dbm >= -50) return 100;
  return 2 * (dbm + 100);
}
// **************************************************
//                Draw WiFi Quality
// **************************************************
void DisplayManager::drawWifiQuality() {
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

// **************************************************
//                Draw Sensor Grid
// **************************************************
void DisplayManager::drawSensorGrid() {

  const int margin = 8;
  const int gap = 8;
  const int topY = 60;

  const int cols = 2;
  const int rows = (SENSOR_COUNT + 1) / 2;

  const int tileW = (gfx.getWidth() - (2*margin) - gap) / 2;
  const int tileH = (gfx.getHeight() - topY - (rows*gap)) / rows;

  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {

    uint8_t row = i / cols;
    uint8_t col = i % cols;

    bool full = (i == SENSOR_COUNT - 1);

    int x = full ? margin : margin + col * (tileW + gap);
    int w = full ? gfx.getWidth() - (2*margin) : tileW;
    int y = topY + row * (tileH + gap);

    SensorTile &s = sensorTiles[i];

    gfx.setColor(MINI_WHITE);
    gfx.drawRect(x, y, w, tileH);

    // Label
    gfx.setFont(ArialRoundedMTBold_14);
    gfx.setColor(getColor(s.type));
    gfx.setTextAlignment(TEXT_ALIGN_CENTER);
    gfx.drawString(x + w/2, y + 10, s.label);

    // Value + arrow
    gfx.setFont(ArialMT_Plain_24);
    
    String valueStr = formatValue(s.type, s.value) + s.unit;
    int valueWidth = gfx.getStringWidth(valueStr.c_str(), valueStr.length());
    
    // --- Layout constants ---
    const int arrowOffset = 12;   // arrow column
    const int arrowWidth  = 10;
    const int spacing     = 6;
    const int rightPad    = 6;
    
    // Fixed arrow position
    int arrowCenterX = x + arrowOffset;

    // Define content area (to the right of arrow)
    int contentLeft  = x + arrowOffset + arrowWidth + spacing;
    int contentRight = x + w - rightPad;
    int contentWidth = contentRight - contentLeft;
    
    // Center value inside content area
    int cy = y + tileH/2 - 8;
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

    // restore for rest of tile
    gfx.setTextAlignment(TEXT_ALIGN_CENTER);
    
    // Min/Max
    gfx.setFont(ArialMT_Plain_10);
    gfx.setColor(MINI_BLUE);

    if (!isnan(s.minVal) && !isnan(s.maxVal)) {
      String mm = formatValue(s.type, s.minVal) + " / " +
                  formatValue(s.type, s.maxVal);
      gfx.drawString(x + w/2, y + tileH - 18, mm);
    } else {
      gfx.drawString(x + w/2, y + tileH - 18, "-- / --");
    }
  }
}

// =====================================================
// Draw Trend Arrow
// =====================================================
void DisplayManager::drawTrendArrow(int x, int y, TrendDirection t) {

  auto thick = [&](int x1, int y1, int x2, int y2) {
    gfx.drawLine(x1, y1, x2, y2);
    gfx.drawLine(x1+1, y1, x2+1, y2);
  };

  switch (t) {
    case TREND_UP:
      thick(x, y+6, x, y-6);
      thick(x, y-6, x-3, y-2);
      thick(x, y-6, x+3, y-2);
      break;

    case TREND_DOWN:
      thick(x, y-6, x, y+6);
      thick(x, y+6, x-3, y+2);
      thick(x, y+6, x+3, y+2);
      break;

    case TREND_FLAT:
      thick(x-3, y, x+3, y);
      break;

    default:
      break;
  }
}



String DisplayManager::formatValue(SensorType type, float v) {
  if (isnan(v)) return "--";

  switch (type) {
    case TEMP:     return String(v, 1);
    case HUMIDITY:
    case PRESSURE: return String((int)round(v));
  }
  return "--";
}

uint16_t DisplayManager::getColor(SensorType t) {
  if (t == HUMIDITY) return MINI_BLUE;
  if (t == PRESSURE) return MINI_YELLOW;
  return MINI_WHITE;
}




// …and just prefix them with DisplayManager::.