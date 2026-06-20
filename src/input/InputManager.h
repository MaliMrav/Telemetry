#pragma once

#include "InputEvent.h"

class InputManager
{
public:
    static void trigger(const InputEvent& event);
    static InputEvent getEvent();

private:
    static InputEvent pendingEvent_;
};