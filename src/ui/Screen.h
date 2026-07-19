#pragma once

// Screen is the base class for every screen in the framework.
//
// A screen owns its own rendering and input handling. The framework
// calls enter() when a screen becomes active, leave() when it is
// replaced, update() on a regular interval to redraw, and onInput()
// when the InputManager dispatches an event.
//
// Screens do not call each other directly. Navigation is expressed
// by returning intent to ScreenManager (future) or by having
// SystemManager observe screen state (current approach).
//
// To add a new screen: subclass Screen, override the relevant methods,
// add a ScreenKind value, and register it with ScreenManager.

#include "../input/InputEvent.h"

enum class ScreenKind
{
    Generic,
    Boot,
    Weather,
    Status,
    Calibration,
    ControlPanel,
    ConnectivityPage
};

class Screen
{
public:
    virtual ~Screen() = default;

    virtual void enter() {}
    virtual void leave() {}

    virtual void update() = 0;

    virtual void onInput(
        const InputEvent& event)
    {
    }

    virtual ScreenKind kind() const
    {
        return ScreenKind::Generic;
    }
};