#include "InputManager.h"

InputEvent InputManager::pendingEvent_{};

void InputManager::trigger(const InputEvent& event)
{
    pendingEvent_ = event;
}

InputEvent InputManager::getEvent()
{
    InputEvent event = pendingEvent_;
    pendingEvent_ = {};
    return event;
}