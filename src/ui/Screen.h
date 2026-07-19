#pragma once

// Screens do not activate other screens directly.
//
// ScreenManager owns screen-level navigation and transitions.
//
// A Screen may interpret an InputEvent locally, or express
// a request for screen-level navigation through the navigation
// contract established by ScreenManager.
//
// Screen is the boundary between application-wide navigation
// and interaction within an individual screen.
//
// A Screen may delegate interaction to its active Page, but it
// should not delegate screen-level navigation to its active Page.
//
// TODO: Sprint Delta - Phase 4 — Define contextual interpretation
// Design the contract between Screen and Page, and implement Page as a base class for the various pages that make up a screen.
// i.e. Write a concrete “return intent” API into Screen


#include "../input/InputEvent.h"

enum class ScreenKind
{
    Generic,
    Boot,
    Weather,
    Calibration,
    ControlPanel
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