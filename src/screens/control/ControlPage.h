#pragma once

#include "../../input/InputEvent.h"

class DisplayManager;

class ControlPage
{
public:
    virtual ~ControlPage() = default;

    // Displayed in the Control Panel title bar.
    virtual const char* title() const = 0; // later this'll become: virtual std::string_view title() const = 0;

    // Called when the page becomes active.
    virtual void onEnter() {}

    // Called immediately before the page is hidden.
    virtual void onLeave() {}

    // Draw the page.
    virtual void render(DisplayManager& display) = 0;

    // Handle semantic input.
    virtual void onInput(const InputEvent& event) = 0;
};