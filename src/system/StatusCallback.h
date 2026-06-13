#pragma once

#include <Arduino.h>

using StatusCallback =
    void (*)(const String& message,
             uint8_t progress);