#pragma once

// TouchEvent is a raw, hardware-level touch reading.
//
// It carries calibrated screen coordinates from a single touch contact.
// TouchController produces TouchEvents; TouchManager consumes them and
// translates them into higher-level InputEvents based on the active profile.
//
// Future fields (gesture, duration, press/release) are noted here
// as placeholders for when gesture recognition is added.

#include <Arduino.h>


struct TouchEvent
{
    int16_t x;
    int16_t y;

    // Future:
    // bool pressed;
    // bool released;
    // uint16_t durationMs;
    // GestureType gesture;
};