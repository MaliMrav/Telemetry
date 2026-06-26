#pragma once

// StatusCallback is a function pointer type used to report boot progress.
//
// SystemManager calls registered callbacks at each boot stage, passing
// a human-readable message and a progress value (0–100). BootScreen uses
// this to drive its progress bar. Any other observer can register the
// same callback type to receive the same events.

#include <Arduino.h>

using StatusCallback =
    void (*)(const String& message,
             uint8_t progress);