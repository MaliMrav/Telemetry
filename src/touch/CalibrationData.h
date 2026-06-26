#pragma once

// CalibrationData holds the four raw touch points collected during
// the guided calibration workflow.
//
// The four corners are sampled in screen coordinates by CalibrationScreen
// and passed to TouchController::calibrate(), which derives the linear
// mapping coefficients and persists them to LittleFS.

#include <XPT2046_Touchscreen.h>

struct CalibrationData
{
    TS_Point topLeft;
    TS_Point topRight;
    TS_Point bottomLeft;
    TS_Point bottomRight;
};