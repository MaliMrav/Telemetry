#pragma once

#include <stdint.h>

enum class SensorDisplayIndicator : uint8_t
{
    NONE,
    SIGNED_FLOW
};


struct SensorDisplayScale
{
    float threshold;
    float divisor;
    uint8_t precision;
    const char* unit;
};
