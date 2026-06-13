#pragma once

#include <Arduino.h>
#include <MiniGrafx.h>
#include <ILI9341_SPI.h>

class DisplayManager {
public:
  enum Color : uint8_t {
    BLACK  = 0,
    WHITE  = 1,
    YELLOW = 2,
    BLUE   = 3
  };

  enum TextAlignment : uint8_t {
    LEFT   = TEXT_ALIGN_LEFT,
    CENTER = TEXT_ALIGN_CENTER,
    RIGHT  = TEXT_ALIGN_RIGHT
  };

  DisplayManager();

  void begin();

  void clear(Color color = BLACK);
  void setColor(Color color);

  template<typename T>
  void setFont(const T* font) { gfx_.setFont(font); }

  void setTextAlignment(TextAlignment alignment);

  void drawString(int16_t x, int16_t y, const String& text);
  void drawString(int16_t x, int16_t y, const char* text);

  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h);
  void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
  void setPixel(int16_t x, int16_t y);

  int16_t getWidth();
  int16_t getHeight();
  // Returns the width of the given text in pixels, based on the currently set font
  int16_t getStringWidth(const String& text);

  void commit();

private:
  ILI9341_SPI tft;
  MiniGrafx gfx_;
};