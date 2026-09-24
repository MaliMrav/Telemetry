#pragma once

// EnergyStatusScreen displays six energy observations:
//
//                         Production       Consumption
//
// Current                     W                W
// Today                       Wh               Wh
// Lifetime                    Wh               Wh
//
// The observations are supplied by the build-time Telemetry composition.
// EnergyStatusScreen knows only the generated composition aliases and the
// SensorRepository runtime model.
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
#include "../data/ObservationHandle.h"

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

    ObservationHandle currentProductionHandle_;
    ObservationHandle currentConsumptionHandle_;

    ObservationHandle todayProductionHandle_;
    ObservationHandle todayConsumptionHandle_;

    ObservationHandle lifetimeProductionHandle_;
    ObservationHandle lifetimeConsumptionHandle_;

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
        const char* columnLabel,
        ObservationHandle handle);

    int8_t getWifiQuality();

    String formatValue(
        const SensorTile& tile) const;
};