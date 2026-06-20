#pragma once

#include <XPT2046_Touchscreen.h>

struct CalibrationData
{
    TS_Point topLeft;
    TS_Point topRight;
    TS_Point bottomLeft;
    TS_Point bottomRight;
};