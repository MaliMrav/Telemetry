#pragma once

#include "../touch/TouchEvent.h"
#include "../input/InputEvent.h"

class Screen {

public:

    virtual ~Screen() = default;

    virtual void enter() {}
    virtual void leave() {}

    virtual void update() = 0;

    virtual void onTouch(
        const TouchEvent& event)
    {
    }

    virtual void onInput(
        const InputEvent& event)
    {
    }
};