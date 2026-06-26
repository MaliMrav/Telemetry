#pragma once

// StatusScreen is a secondary screen for device and connectivity status.
//
// Intended to show information such as WiFi signal, MQTT connection state,
// firmware version, uptime, and IP address. It is navigated to from
// WeatherScreen and supports scrolling for multi-page content.
//
// This screen is a work in progress. The input handler is wired and ready;
// rendering content is the next step.

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