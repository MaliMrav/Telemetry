#pragma once

#include "Screen.h"
#include "../touch/TouchEvent.h"

class ScreenManager {

public:

    void show(Screen* screen);

    void update();

    void onTouch(const TouchEvent& event); // New function to handle touch events and pass them to the current screen

    void onInput(const InputEvent& event); // New function to handle input events and pass them to the

private:

    Screen* currentScreen_ = nullptr;
};