#pragma once

#include "../input/InputEvent.h"

enum class ScreenKind
{
    Generic,
    Boot,
    Weather,
    Status,
    Calibration
};

class Screen
{
public:
    virtual ~Screen() = default;

    virtual void enter() {}
    virtual void leave() {}

    virtual void update() = 0;

    virtual void onInput(
        const InputEvent& event)
    {
    }

    virtual ScreenKind kind() const
    {
        return ScreenKind::Generic;
    }
};