#pragma once

// InputEvent is the unit of communication between input sources and
// the active screen.
//
// Input sources translate physical interaction into semantic actions.
// The event may also carry interaction data that helps the active
// context interpret the action.
//
// The event itself does not contain application-specific meaning.
//
// Examples:
//
//     Touch at (120, 20)
//             ↓
//     TAP + position
//             ↓
//     WeatherScreen interprets the location
//
// Or:
//
//     Joystick moved upward
//             ↓
//     SCROLL_UP
//
// The active screen or page interprets the event within its own context.

#include <Arduino.h>
#include "InputAction.h"

struct InputPosition
{
    int16_t x = 0;
    int16_t y = 0;

    InputPosition() {}

    InputPosition(
        int16_t x,
        int16_t y)
        : x(x)
        , y(y)
    {
    }
};

struct InputEvent
{
    // What the user did.
    InputAction action = InputAction::NONE;

    // When the interaction occurred.
    uint32_t timestamp = 0;

    // True when this is a repeated action rather than the
    // initial occurrence.
    bool repeat = false;

    // The position associated with the action, if any.
    //
    // A position is optional because not all input devices are spatial.
    // For example, a rotary encoder may produce SCROLL_UP or SCROLL_DOWN
    // without supplying a position.
    bool hasPosition = false;

    InputPosition position;

    InputEvent() {}

    InputEvent(
        InputAction action,
        uint32_t timestamp,
        bool repeat,
        int16_t x,
        int16_t y)
        : action(action)
        , timestamp(timestamp)
        , repeat(repeat)
        , hasPosition(true)
        , position(x, y)
    {
    }
};