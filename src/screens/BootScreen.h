#pragma once

#include "../ui/Screen.h"
#include "../display/DisplayManager.h"

class BootScreen : public Screen {

public:

    explicit BootScreen(
        DisplayManager& display);

    void update() override;

    void setStatus(
        const String& message,
        uint8_t progress);

private:

    DisplayManager& display_;

    String status_ = "Starting...";
    uint8_t progress_ = 0;
};