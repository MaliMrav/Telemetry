#include "InputManager.h"

UIAction InputManager::pendingAction_ =
    ACTION_NONE;

void InputManager::trigger(
    UIAction action)
{
    pendingAction_ =
        action;
}

UIAction InputManager::getAction()
{
    UIAction action =
        pendingAction_;

    pendingAction_ =
        ACTION_NONE;

    return action;
}