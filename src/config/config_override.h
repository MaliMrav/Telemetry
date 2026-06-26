#pragma once

// =============================================================================
// config_override.h — Your configuration
// =============================================================================
//
// This is the only file you need to edit to adapt this firmware to your
// hardware and environment.
//
// Uncomment and change any value below. Any value not defined here will
// fall back to the default in config_defaults.h.
//
// Credentials (WiFi, MQTT, OTA passwords) belong in secrets.h, not here.
// Copy secrets.example.h to secrets.h and fill in your values.
//
// =============================================================================


// -----------------------------------------------------------------------------
// System
// -----------------------------------------------------------------------------

// #define CFG_HOSTNAME_PREFIX "weather-display-"


// -----------------------------------------------------------------------------
// Connectivity — MQTT
// -----------------------------------------------------------------------------

// #define CFG_MQTT_PORT         1883
// #define CFG_MQTT_KEEPALIVE    30
// #define CFG_MQTT_RECONNECT_MS 5000


// -----------------------------------------------------------------------------
// Time and Locale
// -----------------------------------------------------------------------------
//
// Set your timezone. Full list of valid strings:
//   https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

#undef CFG_TIMEZONE
#define CFG_TIMEZONE   "Australia/Sydney"

#undef CFG_NTP_SERVER
#define CFG_NTP_SERVER "pool.ntp.org"

// #define CFG_USE_12HOUR_CLOCK false


// -----------------------------------------------------------------------------
// Hardware — Display
// -----------------------------------------------------------------------------
//
// This firmware currently uses a 2.4" ILI9341 TFT display (240x320, SPI).
// If you're using the same hardware, you likely don't need to change anything.
//
// If you're using different hardware:
//   1. Uncomment and set CFG_DISPLAY_DRIVER to your chip (ILI9341, SSD1306, etc.)
//   2. Adjust CFG_SCREEN_WIDTH and CFG_SCREEN_HEIGHT to match your display
//   3. Verify pin assignments (CFG_TFT_DC, CFG_TFT_CS, CFG_TFT_LED)
//   4. Check config/display/YourDriver_config.h for driver-specific settings
//
// Available drivers:
//   ILI9341 — 2.4" 240x320 TFT (default, fully configured)
//   SSD1306 — 128x64 OLED (future, not yet implemented)
//   ST7735  — 1.8" 128x160 TFT (future, not yet implemented)

// #define CFG_DISPLAY_DRIVER ILI9341
// #define CFG_HAS_DISPLAY  true
// #define CFG_TFT_DC       D4
// #define CFG_TFT_CS       D2
// #define CFG_TFT_LED      D3
// #define CFG_SCREEN_WIDTH  240
// #define CFG_SCREEN_HEIGHT 320


// -----------------------------------------------------------------------------
// Hardware — Touch
// -----------------------------------------------------------------------------

// #define CFG_HAS_TOUCH             true
// #define CFG_HAS_RESISTIVE_TOUCH   true
// #define CFG_HAS_CAPACITIVE_TOUCH  false
// #define CFG_TOUCH_CS              D0
// #define CFG_TOUCH_IRQ             D1
// #define CFG_TOUCH_DEBOUNCE_MS     50


// -----------------------------------------------------------------------------
// Hardware — Other Input Devices
// -----------------------------------------------------------------------------

// #define CFG_HAS_BUTTONS  false
// #define CFG_HAS_ENCODER  false


// -----------------------------------------------------------------------------
// Touch Calibration
// -----------------------------------------------------------------------------

// #define CFG_FORCE_CALIBRATION false
// #define CFG_CALIBRATION_FILE  "/calibration.txt"
