#pragma once

#include <Arduino.h>
#include "InputAction.h"

struct InputEvent
{
    InputAction action = InputAction::NONE;
    uint32_t    timestamp = 0;
    bool        repeat = false;
    int16_t     x = 0;
    int16_t     y = 0;

    InputEvent() {}

    InputEvent(InputAction action, uint32_t timestamp, bool repeat, int16_t x, int16_t y)
        : action(action), timestamp(timestamp), repeat(repeat), x(x), y(y) {}
};