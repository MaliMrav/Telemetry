#pragma once

namespace TouchConfig {
  constexpr int TOUCH_CS  = D0;
  constexpr int TOUCH_IRQ = D1;

  constexpr uint32_t CALIBRATION_PROMPT_MS = 3000;
  constexpr uint16_t MAX_TOUCH_TIME_MS = 2000;
  constexpr uint16_t CALIBRATION_RETRY_MS = 500;


  constexpr int SCREEN_WIDTH  = 240;
  constexpr int SCREEN_HEIGHT = 320;

  constexpr uint16_t DEBOUNCE_MS = 50;

  constexpr const char* CALIBRATION_FILE = "/calibration.txt";
  constexpr const char* UI_STATE_FILE     = "/ui_state.txt";
}