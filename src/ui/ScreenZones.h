#pragma once

#include <Arduino.h>

namespace ScreenZones
{
    constexpr int SCREEN_WIDTH  = 240;
    constexpr int SCREEN_HEIGHT = 320;

    constexpr int HEADER_HEIGHT = 50;

    constexpr int EDGE_WIDTH = 20;

    constexpr int CONTENT_TOP    = HEADER_HEIGHT;
    constexpr int CONTENT_BOTTOM = SCREEN_HEIGHT;

    constexpr int CONTENT_MID_Y =
        CONTENT_TOP + ((CONTENT_BOTTOM - CONTENT_TOP) / 2);

    inline bool isInHeader(
        int16_t x,
        int16_t y)
    {
        (void)x;
        return y < HEADER_HEIGHT;
    }

    inline bool isInLeftEdge(
        int16_t x,
        int16_t y)
    {
        return y >= HEADER_HEIGHT &&
               x < EDGE_WIDTH;
    }

    inline bool isInRightEdge(
        int16_t x,
        int16_t y)
    {
        return y >= HEADER_HEIGHT &&
               x >= (SCREEN_WIDTH - EDGE_WIDTH);
    }

    inline bool isInContentUp(
        int16_t x,
        int16_t y)
    {
        return y >= CONTENT_TOP &&
               y < CONTENT_MID_Y &&
               x >= EDGE_WIDTH &&
               x < (SCREEN_WIDTH - EDGE_WIDTH);
    }

    inline bool isInContentDown(
        int16_t x,
        int16_t y)
    {
        return y >= CONTENT_MID_Y &&
               y < CONTENT_BOTTOM &&
               x >= EDGE_WIDTH &&
               x < (SCREEN_WIDTH - EDGE_WIDTH);
    }

    inline bool isInContentArea(
        int16_t x,
        int16_t y)
    {
        return y >= CONTENT_TOP &&
               y < CONTENT_BOTTOM &&
               x >= EDGE_WIDTH &&
               x < (SCREEN_WIDTH - EDGE_WIDTH);
    }
}