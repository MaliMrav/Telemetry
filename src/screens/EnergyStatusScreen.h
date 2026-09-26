#pragma once

// EnergyStatusScreen renders the application ScreenDefinition named
// "energy_status".
//
// The Screen layout and observation membership come from telemetry.yaml.
// EnergyStatusScreen therefore does not maintain its own list of observation
// aliases, row labels, or column labels.
//
// It consumes only:
//   - generated ScreenDefinition / ObservationDefinition metadata
//   - SensorRepository runtime values
//   - generic SensorTile presentation metadata
//
// It has no knowledge of:
//   - MQTT
//   - Home Assistant
//   - Envoy
//   - semantic ObservationKeys
//   - repository storage slots
//
// Input handling:
//   - TAP within the clock area toggles 12/24-hour clock
//   - NEXT_SCREEN     -> ControlPanelScreen
//   - PREVIOUS_SCREEN -> WeatherScreen

#include <Arduino.h>

#include "../models/SensorTile.h"
#include "../display/DisplayManager.h"
#include "../ui/Screen.h"
#include "../input/InputEvent.h"

#include "TelemetryComposition.h"

class EnergyStatusScreen : public Screen
{
public:
    explicit EnergyStatusScreen(
        DisplayManager& display);

    void enter()  override;
    void leave()  override;
    void update() override;

    ScreenIntent onInput(
        const InputEvent& event) override;

    ScreenKind kind() const override
    {
        return ScreenKind::Solar;
    }

private:
    DisplayManager& display_;

    bool use12HourClock_ = false;

    const TelemetryComposition::ScreenDefinition* screen_ =
        nullptr;

    bool isClockArea(
        const InputPosition& position) const;

    void drawHeader();
    void drawWifiQuality();
    void drawGrid();

    void drawQuadrant(
        int x,
        int y,
        int w,
        int h,
        const char* rowLabel,
        const TelemetryComposition::ObservationDefinition* observation);

    void drawFlowArrow(
        int x,
        int y,
        bool upward);

    int8_t getWifiQuality();

    String formatValue(
        const SensorTile& tile,
        float value) const;
};
