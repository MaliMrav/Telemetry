#pragma once

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