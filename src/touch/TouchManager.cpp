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
}

void TouchManager::update()
{
    if (mode_ == Mode::Calibration)
    {
        if (!controller_.isTouched(TouchConfig::DEBOUNCE_MS))
        {
            wasTouched_ = false;
            return;
        }

        if (wasTouched_)
        {
            return;
        }

        const TS_Point raw = controller_.getRawPoint();

        InputManager::trigger(
            InputEvent{
                InputAction::TAP,
                millis(),
                false,
                raw.x,
                raw.y
            });

        wasTouched_ = true;
        return;
    }

    int16_t x = 0;
    int16_t y = 0;

    if (!controller_.getTouch(x, y))
    {
        wasTouched_ = false;
        return;
    }

    if (wasTouched_)
    {
        return;
    }

    InputManager::trigger(
        InputEvent{
            InputAction::TAP,
            millis(),
            false,
            x,
            y
        });

    wasTouched_ = true;
}