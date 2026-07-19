#pragma once

// ScreenManager owns the currently active screen.
//
// Responsibilities:
//   - activate a screen via activate(), calling leave() on the outgoing
//     screen and enter() on the incoming one
//   - set the appropriate TouchManager profile when the screen changes
//   - receive all InputEvents from InputManager
//   - own screen-level navigation and screen transitions
//   - forward non-navigation events to the active screen
//
// ScreenManager is the boundary between application-wide navigation
// and interaction within an individual screen.
//
// An InputEvent may represent:
//
//   - a request to navigate between screens
//   - an interaction within the active screen
//
// ScreenManager owns the first case.
//
// The active Screen owns the second.
//
// The active Screen may then interpret the event itself or delegate
// it to its active Page.

#include "Screen.h"
#include "../input/InputEvent.h"

class TouchManager;

class ScreenManager
{
public:

    void bindTouchManager(
        TouchManager* touchManager);

    void activate(
        Screen* screen);

    void update();

    void onInput(
        const InputEvent& event);

private:

    Screen* currentScreen_ = nullptr;

    TouchManager* touchManager_ = nullptr;
};