#pragma once

// BootScreen displays boot progress while the firmware initialises.
//
// It shows a status message and a progress bar driven by SystemManager
// via setStatus(). It renders immediately on each call so progress is
// visible even before the main loop starts.
//
// It has no input handling — the user cannot interact with it.

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