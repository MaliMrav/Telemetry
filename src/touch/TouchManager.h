#pragma once

#include "TouchController.h"

class TouchManager
{
public:
    enum class Mode
    {
        Normal,
        Calibration
    };

    explicit TouchManager(
        TouchController& controller);

    void setMode(Mode mode);
    void update();

private:
    TouchController& controller_;
    Mode mode_ = Mode::Normal;
    bool wasTouched_ = false;
};