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

begin()
void DisplayManager::begin() {
  pinMode(Pins::TFT_LED, OUTPUT);
  digitalWrite(Pins::TFT_LED, LOW);

  gfx.init();
  gfx.fillBuffer(MINI_BLACK);
  gfx.commit();
}

draw()
void DisplayManager::draw() {
  gfx.fillBuffer(MINI_BLACK);
  drawHeader();
  drawWifiQuality();
  drawSensorGrid();
  gfx.commit();
}
// Then paste your existing:

// drawHeader
// drawWifiQuality
//drawSensorGrid
// drawTrendArrow
// helpers
