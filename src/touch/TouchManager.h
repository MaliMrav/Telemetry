#pragma once

#include "TouchController.h"
#include "../input/InputSource.h"
#include "../input/InputAction.h"

class TouchManager : public IInputSource
{
public:
    enum class Profile
    {
        Generic,
        Weather,
        Status,
        Calibration
    };

    explicit TouchManager(
        TouchController& controller);

    void setProfile(Profile profile);
    void update() override;

private:
    TouchController& controller_;
    Profile profile_ = Profile::Generic;

    bool wasTouched_ = false;
    unsigned long lastEventMs_ = 0;

    void emitEvent(
        InputAction action,
        const TS_Point& point);
};