#pragma once

#include "TouchController.h"
#include "../input/InputSource.h"

class TouchManager : public IInputSource
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
    void update() override;

private:
    TouchController& controller_;
    Mode mode_ = Mode::Normal;

    bool wasTouched_ = false;
    unsigned long lastEventMs_ = 0;
};