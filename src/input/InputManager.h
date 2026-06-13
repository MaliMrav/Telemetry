#pragma once

#include "../ui/UIAction.h"

class InputManager
{
public:

    static void trigger(
        UIAction action);

    static UIAction getAction();

private:

    static UIAction pendingAction_;
};