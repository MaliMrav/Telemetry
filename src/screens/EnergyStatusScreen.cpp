#include "EnergyStatusScreen.h"

#include "../display/DisplayManager.h"

#include "../models/SensorRepository.h"

#include "../input/InputAction.h"
#include "../ui/ScreenIntent.h"
#include "../ui/ScreenKind.h"

#include "../config/config.h"
#include "../system/DebugOverlay.h"
#include "ScreenConfig.h"

#include "TelemetryComposition.h"

#include <ArialRounded.h>
#include <ESP8266WiFi.h>
#include <time.h>


namespace
{
    constexpr int ROWS = 3;
    constexpr int COLS = 2;

    constexpr int MARGIN = 4;
    constexpr int GAP = 4;
}


EnergyStatusScreen::EnergyStatusScreen(
    DisplayManager& display)
    : display_(display)
{
}


void EnergyStatusScreen::enter()
{
    // -------------------------------------------------------------------------
    // Application composition
    // -------------------------------------------------------------------------
    //
    // Observations are generated from telemetry.yaml.
    //
    // EnergyStatusScreen deliberately retains its current six-tile layout,
    // but resolves those tiles through the generic composition interface
    // rather than through generated member names.
    //

    const auto findHandle =
        [](const char* alias) -> ObservationHandle
        {
            const auto* observation =
                TelemetryComposition::findObservation(
                    alias);

            if (!observation)
            {
                return ObservationHandle{};
            }

            return observation->handle;
        };


    currentProductionHandle_ =
        findHandle(
            "current_power_production");

    currentConsumptionHandle_ =
        findHandle(
            "current_power_consumption");

    todayProductionHandle_ =
        findHandle(
            "energy_production_today");

    todayConsumptionHandle_ =
        findHandle(
            "energy_consumption_today");

    lifetimeProductionHandle_ =
        findHandle(
            "energy_production_lifetime");

    lifetimeConsumptionHandle_ =
        findHandle(
            "energy_consumption_lifetime");
}


void EnergyStatusScreen::leave()
{
}


void EnergyStatusScreen::update()
{
    display_.clear(
        DisplayManager::BLACK);

    drawHeader();
    drawWifiQuality();
    drawGrid();

    display_.commit();

    DBG_DRAW(display_);
}


bool EnergyStatusScreen::isClockArea(
    const InputPosition& position) const
{
    return position.x >=
               ScreenConfig::CLOCK_AREA_LEFT &&

           position.x <
               ScreenConfig::CLOCK_AREA_RIGHT &&

           position.y >=
               ScreenConfig::CLOCK_AREA_TOP &&

           position.y <
               ScreenConfig::CLOCK_AREA_BOTTOM;
}


ScreenIntent EnergyStatusScreen::onInput(
    const InputEvent& event)
{
    DBG_RECORD(event);

    switch (event.action)
    {
        case InputAction::TAP:

            if (event.hasPosition &&
                isClockArea(event.position))
            {
                use12HourClock_ =
                    !use12HourClock_;

                update();
            }

            break;


        case InputAction::NEXT_SCREEN:
            return ScreenIntent::navigateTo(
                ScreenKind::ControlPanel);


        case InputAction::PREVIOUS_SCREEN:
            return ScreenIntent::navigateTo(
                ScreenKind::Weather);


        case InputAction::SELECT:
            // Contextual selection.
            break;


        default:
            break;
    }

    return ScreenIntent();
}


void EnergyStatusScreen::drawHeader()
{
    time_t now =
        time(nullptr);

    struct tm* t =
        localtime(&now);

    if (!t)
    {
        return;
    }


    display_.setTextAlignment(
        DisplayManager::CENTER);

    display_.setColor(
        DisplayManager::WHITE);


    // -------------------------------------------------------------------------
    // Date
    // -------------------------------------------------------------------------

    display_.setFont(
        ArialRoundedMTBold_14);

    String date =
        TimeConfig::WDAY_NAMES[t->tm_wday] +
        " " +
        TimeConfig::MONTH_NAMES[t->tm_mon] +
        " " +
        String(t->tm_mday);

    display_.drawString(
        display_.getWidth() / 2,
        ScreenConfig::HEADER_DATE_Y,
        date);


    // -------------------------------------------------------------------------
    // Time
    // -------------------------------------------------------------------------

    display_.setFont(
        ArialRoundedMTBold_36);

    char buf[10];

    if (use12HourClock_)
    {
        int hour =
            t->tm_hour % 12;

        if (hour == 0)
        {
            hour = 12;
        }

        snprintf(
            buf,
            sizeof(buf),
            "%d:%02d:%02d",
            hour,
            t->tm_min,
            t->tm_sec);
    }
    else
    {
        snprintf(
            buf,
            sizeof(buf),
            "%02d:%02d:%02d",
            t->tm_hour,
            t->tm_min,
            t->tm_sec);
    }


    display_.drawString(
        display_.getWidth() / 2,
        ScreenConfig::HEADER_TIME_Y,
        buf);


    // -------------------------------------------------------------------------
    // AM / PM indicator
    // -------------------------------------------------------------------------

    if (use12HourClock_)
    {
        const char* amPm =
            t->tm_hour < 12
                ? "AM"
                : "PM";

        display_.setFont(
            ArialRoundedMTBold_14);

        display_.setTextAlignment(
            DisplayManager::CENTER);

        display_.setColor(
            DisplayManager::WHITE);

        display_.fillRect(
            8,
            32,
            34,
            18);

        display_.setColor(
            DisplayManager::BLUE);

        display_.drawString(
            25,
            34,
            amPm);
    }
}


int8_t EnergyStatusScreen::getWifiQuality()
{
    int32_t dbm =
        WiFi.RSSI();

    if (dbm <= -100)
    {
        return 0;
    }

    if (dbm >= -50)
    {
        return 100;
    }

    return 2 * (dbm + 100);
}


void EnergyStatusScreen::drawWifiQuality()
{
    int8_t q =
        getWifiQuality();


    display_.setTextAlignment(
        DisplayManager::RIGHT);

    display_.setFont(
        ArialMT_Plain_10);

    display_.setColor(
        DisplayManager::WHITE);

    display_.drawString(
        228,
        9,
        String(q) + "%");


    for (int8_t i = 0; i < 4; ++i)
    {
        for (
            int8_t j = 0;
            j < 2 * (i + 1);
            ++j)
        {
            if (
                q > i * 25 ||
                j == 0)
            {
                display_.setPixel(
                    230 + 2 * i,
                    18 - j);
            }
        }
    }
}


void EnergyStatusScreen::drawGrid()
{
    const int topY =
        ScreenConfig::TOP_MARGIN;


    const int tileW =
        (
            display_.getWidth() -
            (2 * MARGIN) -
            ((COLS - 1) * GAP)
        ) / COLS;


    const int tileH =
        (
            display_.getHeight() -
            topY -
            ((ROWS - 1) * GAP)
        ) / ROWS;


    const ObservationHandle productionHandles[ROWS] =
    {
        currentProductionHandle_,
        todayProductionHandle_,
        lifetimeProductionHandle_
    };


    const ObservationHandle consumptionHandles[ROWS] =
    {
        currentConsumptionHandle_,
        todayConsumptionHandle_,
        lifetimeConsumptionHandle_
    };


    const char* rowLabels[ROWS] =
    {
        "Current",
        "Today",
        "Lifetime"
    };


    for (int row = 0;
         row < ROWS;
         ++row)
    {
        const int y =
            topY +
            row * (tileH + GAP);


        drawQuadrant(
            MARGIN,
            y,
            tileW,
            tileH,
            rowLabels[row],
            "Production",
            productionHandles[row]);


        drawQuadrant(
            MARGIN + tileW + GAP,
            y,
            tileW,
            tileH,
            rowLabels[row],
            "Consumption",
            consumptionHandles[row]);
    }
}


void EnergyStatusScreen::drawQuadrant(
    int x,
    int y,
    int w,
    int h,
    const char* rowLabel,
    const char* columnLabel,
    ObservationHandle handle)
{
    const SensorTile* tile =
        SensorRepository::getTile(handle);


    // -------------------------------------------------------------------------
    // Tile frame
    // -------------------------------------------------------------------------

    display_.setColor(
        DisplayManager::WHITE);

    display_.drawRect(
        x,
        y,
        w,
        h);


    // -------------------------------------------------------------------------
    // Context heading
    // -------------------------------------------------------------------------

    display_.setTextAlignment(
        DisplayManager::CENTER);

    display_.setFont(
        ArialRoundedMTBold_14);

    display_.setColor(
        DisplayManager::YELLOW);

    display_.drawString(
        x + w / 2,
        y + 3,
        rowLabel);


    // -------------------------------------------------------------------------
    // Value
    // -------------------------------------------------------------------------

    display_.setFont(
        ArialMT_Plain_24);

    display_.setColor(
        DisplayManager::WHITE);

    display_.setTextAlignment(
        DisplayManager::CENTER);


    const String value =
        tile
            ? formatValue(*tile)
            : "--";


    display_.drawString(
        x + w / 2,
        y + h / 2 - 7,
        value);


    // -------------------------------------------------------------------------
    // Measurement label
    // -------------------------------------------------------------------------

    display_.setFont(
        ArialMT_Plain_10);

    display_.setColor(
        DisplayManager::BLUE);

    display_.drawString(
        x + w / 2,
        y + h - 14,
        columnLabel);
}


String EnergyStatusScreen::formatValue(
    const SensorTile& tile) const
{
    if (!tile.valid ||
        isnan(tile.value))
    {
        return "--";
    }


    // -------------------------------------------------------------------------
    // Generic display scaling
    // -------------------------------------------------------------------------
    //
    // Display policy is supplied by telemetry.yaml through the generated
    // composition and attached to SensorTile.
    //

    if (tile.displayScales &&
        tile.displayScaleCount > 0)
    {
        const float absoluteValue =
            fabs(tile.value);

        const SensorDisplayScale* selected =
            nullptr;

        for (uint8_t i = 0;
             i < tile.displayScaleCount;
             ++i)
        {
            if (absoluteValue >=
                tile.displayScales[i].threshold)
            {
                selected =
                    &tile.displayScales[i];
            }
        }

        if (selected)
        {
            return String(
                       tile.value /
                       selected->divisor,
                       selected->precision) +
                   " " +
                   selected->unit;
        }
    }


    // -------------------------------------------------------------------------
    // Generic fallback
    // -------------------------------------------------------------------------

    String result =
        String(
            tile.value,
            1);

    if (tile.unit &&
        tile.unit[0] != '\0')
    {
        result +=
            " ";

        result +=
            tile.unit;
    }

    return result;
}
