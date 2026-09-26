#pragma once

// =============================================================================
// ILI9341 Display Driver Configuration
// =============================================================================
//
// Hardware: 2.4" 240x320 TFT LCD (SPI interface)
// Controller: ILI9341
//
// MiniGrafx uses an indexed color framebuffer.
//
// The previous Telemetry profile used a 2-bit framebuffer with four palette
// entries. The Energy Composition Proving Slice requires two additional
// presentation colours (RED and GREEN) for the generic signed-flow indicator.
//
// The framebuffer therefore uses 4 bits per pixel. Existing colour indices
// remain unchanged so current Weather / Energy colours keep their meaning.
//
// IMPORTANT: 4 bits per pixel doubles framebuffer storage compared with the
// previous 2-bit profile. This is an intentional resource trade-off for the
// proving slice and should be verified against the ESP8266 memory budget.

#include <ILI9341_SPI.h>

// MiniGrafx's 4-bit indexed paths can address 16 palette entries.
// Keep the first six entries stable and fill the remaining entries with
// BLACK so every valid 4-bit palette index has a defined value.
static uint16_t display_palette[] = {
    ILI9341_BLACK,   // 0
    ILI9341_WHITE,   // 1
    ILI9341_YELLOW,  // 2
    ILI9341_BLUE,    // 3
    ILI9341_RED,     // 4
    ILI9341_GREEN,   // 5
    ILI9341_BLACK,   // 6
    ILI9341_BLACK,   // 7
    ILI9341_BLACK,   // 8
    ILI9341_BLACK,   // 9
    ILI9341_BLACK,   // 10
    ILI9341_BLACK,   // 11
    ILI9341_BLACK,   // 12
    ILI9341_BLACK,   // 13
    ILI9341_BLACK,   // 14
    ILI9341_BLACK    // 15
};

constexpr uint8_t DISPLAY_BITS_PER_PIXEL = 4;

// SPI clock speed in Hz.
constexpr uint32_t DISPLAY_SPI_FREQUENCY = 27000000;
