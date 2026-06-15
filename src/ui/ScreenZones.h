#pragma once

#include <Arduino.h>

namespace ScreenZones
{
    constexpr int SCREEN_WIDTH  = 240;
    constexpr int SCREEN_HEIGHT = 320;

    constexpr int TOP_HEIGHT = 50;

    inline bool isInHeader(
        int16_t x,
        int16_t y)
    {
        (void)x;

        return y < TOP_HEIGHT;
    }

    inline bool isInLeftZone(
        int16_t x,
        int16_t y)
    {
        return y >= TOP_HEIGHT &&
               x < (SCREEN_WIDTH / 2);
    }

    inline bool isInRightZone(
        int16_t x,
        int16_t y)
    {
        return y >= TOP_HEIGHT &&
               x >= (SCREEN_WIDTH / 2);
    }
}