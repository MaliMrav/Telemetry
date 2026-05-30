#pragma once

#include <Arduino.h>
#include <MiniGrafx.h>
#include <ILI9341_SPI.h>

enum MiniColorIndex : uint8_t {
  MINI_BLACK  = 0,
  MINI_WHITE  = 1,
  MINI_YELLOW = 2,
  MINI_BLUE   = 3
};

class DisplayManager {
public:
  DisplayManager();

  void begin();

  MiniGrafx& getGfx();
  const MiniGrafx& getGfx() const;

private:
  ILI9341_SPI tft;
  MiniGrafx gfx_;
};