#pragma once

// InputEvent is the unit of communication between input sources and screens.
//
// It carries the abstract action, a timestamp, optional coordinates (for
// touch-based actions), and a repeat flag for held inputs.
// All input sources produce InputEvents; all screens consume them.
// Neither side needs to know anything about the other.

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