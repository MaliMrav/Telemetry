#pragma once

#include "Screen.h"
#include "../touch/TouchEvent.h"

class ScreenManager {

public:

    void show(Screen* screen);

    void update();

    void onTouch(const TouchEvent& event); // New function to handle touch events and pass them to the

private:

    Screen* currentScreen_ = nullptr;
};