#pragma once

#include "../ui/Screen.h"
#include "../display/DisplayManager.h"

class StatusScreen : public Screen {
public:
    explicit StatusScreen(DisplayManager& display);

    void update() override;

private:
    DisplayManager& display_;
};