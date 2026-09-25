#pragma once

#include <stdint.h>

struct SensorDisplayScale
{
    float threshold;
    float divisor;
    uint8_t precision;
    const char* unit;
};
