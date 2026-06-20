#pragma once

namespace HardwareConfig
{
    // Display
    constexpr bool HAS_DISPLAY = true;

    // Input devices
    constexpr bool HAS_TOUCH = true;
    constexpr bool HAS_BUTTONS = false;
    constexpr bool HAS_ENCODER = false;
    constexpr bool HAS_KEYBOARD = false;

    // Touch type
    constexpr bool HAS_RESISTIVE_TOUCH = true;
    constexpr bool HAS_CAPACITIVE_TOUCH = false;
}