#include "DisplayManager.h"
#include "../config/config.h"
#include <ArialRounded.h>

DisplayManager::DisplayManager()
  : tft(Pins::TFT_CS, Pins::TFT_DC),
    gfx_(&tft, DISPLAY_BITS_PER_PIXEL, display_palette) {}

void DisplayManager::begin() {
  if (Pins::TFT_LED >= 0) {
    pinMode(Pins::TFT_LED, OUTPUT);
    digitalWrite(Pins::TFT_LED, LOW);
  }

  gfx_.init();
  gfx_.fillBuffer(BLACK);
  gfx_.commit();
}

void DisplayManager::clear(Color color) {
  gfx_.fillBuffer(color);
}

void DisplayManager::setColor(Color color) {
  gfx_.setColor(color);
}

void DisplayManager::setTextAlignment(TextAlignment alignment)
{
    gfx_.setTextAlignment(
        static_cast<TEXT_ALIGNMENT>(
            alignment));
}

void DisplayManager::drawString(int16_t x, int16_t y, const String& text) {
  gfx_.drawString(x, y, text);
}

void DisplayManager::drawString(int16_t x, int16_t y, const char* text) {
  gfx_.drawString(x, y, text);
}

void DisplayManager::drawRect(int16_t x, int16_t y, int16_t w, int16_t h) {
  gfx_.drawRect(x, y, w, h);
}

void DisplayManager::fillRect(int16_t x, int16_t y, int16_t w, int16_t h) {
  gfx_.fillRect(x, y, w, h);
}

void DisplayManager::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
  gfx_.drawLine(x1, y1, x2, y2);
}

void DisplayManager::setPixel(int16_t x, int16_t y) {
  gfx_.setPixel(x, y);
}

int16_t DisplayManager::getWidth() {
  return gfx_.getWidth();
}

int16_t DisplayManager::getHeight() {
  return gfx_.getHeight();
}

int16_t DisplayManager::getStringWidth(
    const String& text)
{
    return gfx_.getStringWidth(
        text.c_str(),
        text.length());
}

void DisplayManager::commit() {
  gfx_.commit();
}