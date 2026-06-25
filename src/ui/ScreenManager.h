#pragma once

#include "Screen.h"
#include "../input/InputEvent.h"

class TouchManager;

class ScreenManager
{
public:
    void bindTouchManager(TouchManager* touchManager);

    void show(Screen* screen);

    void update();

    void onInput(const InputEvent& event);

private:
    Screen* currentScreen_ = nullptr;
    TouchManager* touchManager_ = nullptr;
};