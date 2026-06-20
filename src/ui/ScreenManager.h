#pragma once

#include "Screen.h"
#include "../input/InputEvent.h"

class ScreenManager
{
public:
    void show(Screen* screen);
    void update();
    void onInput(const InputEvent& event);

private:
    Screen* currentScreen_ = nullptr;
};