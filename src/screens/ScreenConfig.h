#pragma once

#include <Arduino.h>

namespace ScreenConfig {

constexpr uint8_t COLUMNS = 2;

constexpr int TOP_MARGIN  = 60;
constexpr int SIDE_MARGIN = 8;
constexpr int TILE_GAP    = 8;

constexpr bool LAST_TILE_FULL_WIDTH = true;

constexpr int HEADER_DATE_Y = 6;
constexpr int HEADER_TIME_Y = 20;

constexpr int ARROW_OFFSET = 12;
constexpr int ARROW_WIDTH  = 10;
constexpr int SPACING      = 6;
constexpr int RIGHT_PAD    = 6;

}