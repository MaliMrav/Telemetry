#pragma once

#include "ControlPage.h"
#include "Destination.h"

class DisplayManager;

class ControlMenuPage : public ControlPage
{
public:

    const char* title() const override;

    void render(DisplayManager& display) override;

    void onInput(const InputEvent& event) override;

    [[nodiscard]]
    Destination destination() const
    {
        return destination_;
    }

private:
    Destination destination_ = Destination::About;
};