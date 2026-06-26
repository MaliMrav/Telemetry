#pragma once

// =============================================================================
// ILI9341 Display Driver Configuration
// =============================================================================
//
// Hardware: 2.4" 240x320 TFT LCD (SPI interface)
// Controller: ILI9341
// Common in: ESP8266 development kits, Arduino shields
//
// This file defines driver-specific settings for the ILI9341.
// General display settings (resolution, pins) are in config_defaults.h.
//
// =============================================================================

#include <ILI9341_SPI.h>

// Color palette for 2-bit (4 color) framebuffer mode.
// MiniGrafx uses an indexed color model to save RAM. Each pixel is 2 bits,
// mapping to one of these 4 colors.
//
// You can change these to any valid 16-bit RGB565 color:
//   BLACK   = 0x0000
//   WHITE   = 0xFFFF
//   RED     = 0xF800
//   GREEN   = 0x07E0
//   BLUE    = 0x001F
//   YELLOW  = 0xFFE0
//   CYAN    = 0x07FF
//   MAGENTA = 0xF81F
//
// The DisplayManager::Color enum (BLACK=0, WHITE=1, YELLOW=2, BLUE=3)
// indexes into this array.
static uint16_t display_palette[] = {
    ILI9341_BLACK,   // 0
    ILI9341_WHITE,   // 1
    ILI9341_YELLOW,  // 2
    ILI9341_BLUE     // 3
};

// Bits per pixel for the framebuffer.
// 2 = 4 colors (uses display_palette above)
// Higher bit depths use more RAM but support more colors.
constexpr uint8_t DISPLAY_BITS_PER_PIXEL = 2;

// SPI clock speed in Hz.
// ILI9341 supports up to 40 MHz, but stability varies by wiring length
// and board design. 20-27 MHz is a safe default for most setups.
// Lower values reduce flicker risk but slow down screen updates.
constexpr uint32_t DISPLAY_SPI_FREQUENCY = 27000000;  // 27 MHz
