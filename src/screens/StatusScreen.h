#pragma once

#include "../ui/Screen.h"
#include "../display/DisplayManager.h"

class StatusScreen : public Screen {
public:
    explicit StatusScreen(DisplayManager& display);

    void update() override;
    
    void onInput(const InputEvent& event) override;

    ScreenKind kind() const override
    {
        return ScreenKind::Status;
    }

private:
    DisplayManager& display_;
};