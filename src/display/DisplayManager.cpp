#include "DisplayManager.h"
#include "../config/settings.h"

static uint16_t palette[] = {
  ILI9341_BLACK,
  ILI9341_WHITE,
  ILI9341_YELLOW,
  0x7E3C
};

DisplayManager::DisplayManager()
  : tft(Pins::TFT_CS, Pins::TFT_DC),
    gfx_(&tft, 2, palette) {}

void DisplayManager::begin() {
  pinMode(Pins::TFT_LED, OUTPUT);
  digitalWrite(Pins::TFT_LED, LOW);

  gfx_.init();
  gfx_.fillBuffer(MINI_BLACK);
  gfx_.commit();
}

MiniGrafx& DisplayManager::getGfx() {
  return gfx_;
}

const MiniGrafx& DisplayManager::getGfx() const {
  return gfx_;
}
