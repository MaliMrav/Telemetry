#include "TouchManager.h"

#include <Arduino.h>

#include "../config/touch_config.h"
#include "../input/InputAction.h"
#include "../input/InputEvent.h"
#include "../input/InputManager.h"

TouchManager::TouchManager(
    TouchController& controller)
:
    controller_(controller)
{
}

void TouchManager::setMode(
    Mode mode)
{
    mode_ = mode;
    wasTouched_ = false;
    lastEventMs_ = 0;
}

void TouchManager::update()
{
    const bool touched = controller_.isTouched();

    if (!touched)
    {
        wasTouched_ = false;
        return;
    }

    if (wasTouched_)
    {
        return;
    }

    if (millis() - lastEventMs_ < TouchConfig::DEBOUNCE_MS)
    {
        return;
    }

    const TS_Point point =
        (mode_ == Mode::Calibration) ?
            controller_.getRawPoint() :
            controller_.getPoint();

    InputManager::trigger(
        InputEvent{
            InputAction::TAP,
            millis(),
            false,
            point.x,
            point.y
        });

    wasTouched_ = true;
    lastEventMs_ = millis();
}