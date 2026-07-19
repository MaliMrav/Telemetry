#pragma once

#include "ControlPage.h"
#include "ControlPageKind.h"

class DisplayManager;

class ControlMenuPage : public ControlPage
{
public:

    const char* title() const override;

    void render(DisplayManager& display) override;

    void onInput(const InputEvent& event) override;

    // [[nodiscard]]
    ControlPageKind destination() const
    {
        return selectedPage_;
    }

private:
    ControlPageKind selectedPage_ = ControlPageKind::About;
};