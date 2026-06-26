#pragma once

// ScreenManager owns the currently active screen and routes events to it.
//
// Responsibilities:
//   - activate a screen via show(), calling leave() on the outgoing
//     screen and enter() on the incoming one
//   - set the appropriate TouchManager profile when the screen changes
//   - forward update() and onInput() calls to the active screen
//
// ScreenManager holds one screen at a time. Multi-screen navigation
// and history are intentionally left to higher-level logic in SystemManager
// or future screen transition helpers.

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