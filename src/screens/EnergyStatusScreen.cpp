#include "EnergyStatusScreen.h"

#include "../display/DisplayManager.h"

#include "../models/SensorRepository.h"

#include "../input/InputAction.h"
#include "../ui/ScreenIntent.h"
#include "../ui/ScreenKind.h"

#include "../config/config.h"
#include "../system/DebugOverlay.h"
#include "ScreenConfig.h"

#include <ArialRounded.h>
#include <ESP8266WiFi.h>
#include <time.h>


namespace
{
    constexpr int MARGIN = 4;
    constexpr int GAP = 4;

    constexpr const char* SCREEN_ID =
        "energy_status";

    constexpr int FLOW_ARROW_OFFSET = 12;
    constexpr int FLOW_ARROW_WIDTH = 10;
    constexpr int FLOW_SPACING = 6;
    constexpr int FLOW_RIGHT_PAD = 6;
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
    // The layout and observation membership are supplied by telemetry.yaml.
    //

    screen_ =
        TelemetryComposition::findScreen(
            SCREEN_ID);
}


void EnergyStatusScreen::leave()
{
    screen_ = nullptr;
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
            DisplayManager::CYAN);

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
    if (!screen_ ||
        !screen_->rows ||
        screen_->rowCount == 0)
    {
        return;
    }


    const int rows =
        screen_->rowCount;


    const int topY =
        ScreenConfig::TOP_MARGIN;


    const int tileH =
        (
            display_.getHeight() -
            topY -
            ((rows - 1) * GAP)
        ) / rows;


    for (uint8_t rowIndex = 0;
         rowIndex < screen_->rowCount;
         ++rowIndex)
    {
        const auto& row =
            screen_->rows[rowIndex];


        const int columns =
            row.itemCount;


        if (columns <= 0)
        {
            continue;
        }


        const int tileW =
            (
                display_.getWidth() -
                (2 * MARGIN) -
                ((columns - 1) * GAP)
            ) / columns;


        const int y =
            topY +
            rowIndex * (tileH + GAP);


        for (uint8_t columnIndex = 0;
             columnIndex < row.itemCount;
             ++columnIndex)
        {
            const char* alias =
                row.items[columnIndex];


            const auto* observation =
                TelemetryComposition::findObservation(
                    alias);


            if (!observation)
            {
                continue;
            }


            const int x =
                MARGIN +
                columnIndex *
                    (tileW + GAP);


            drawQuadrant(
                x,
                y,
                tileW,
                tileH,
                row.title,
                observation);
        }
    }
}


void EnergyStatusScreen::drawQuadrant(
    int x,
    int y,
    int w,
    int h,
    const char* rowLabel,
    const TelemetryComposition::ObservationDefinition* observation)
{
    if (!observation)
    {
        return;
    }


    const SensorTile* tile =
        SensorRepository::getTile(
            observation->handle);


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
    // Missing observation / invalid value
    // -------------------------------------------------------------------------

    if (!tile)
    {
        display_.setFont(
            ArialMT_Plain_24);

        display_.setColor(
            DisplayManager::WHITE);

        display_.drawString(
            x + w / 2,
            y + h / 2 - 7,
            "--");

        return;
    }


    // -------------------------------------------------------------------------
    // Value
    // -------------------------------------------------------------------------

    display_.setFont(
        ArialMT_Plain_24);

    display_.setColor(
        DisplayManager::WHITE);

    const bool signedFlow =
        tile->displayIndicator ==
            SensorDisplayIndicator::SIGNED_FLOW &&
        tile->valid &&
        !isnan(tile->value) &&
        tile->value != 0.0f;


    const float displayValue =
        signedFlow
            ? fabs(tile->value)
            : tile->value;


    const String value =
        formatValue(
            *tile,
            displayValue);


    display_.setTextAlignment(
        DisplayManager::CENTER);


    if (signedFlow)
    {
        const int arrowWidth =
            FLOW_ARROW_WIDTH;

        const int arrowSpacing =
            FLOW_SPACING;

        const int arrowOffset =
            FLOW_ARROW_OFFSET;

        const int rightPad =
            FLOW_RIGHT_PAD;


        const int contentLeft =
            x +
            arrowOffset +
            arrowWidth +
            arrowSpacing;


        const int contentRight =
            x +
            w -
            rightPad;


        const int contentWidth =
            contentRight -
            contentLeft;


        const int valueWidth =
            display_.getStringWidth(
                value);


        const int valueX =
            contentLeft +
            (contentWidth - valueWidth) / 2;


        display_.setTextAlignment(
            DisplayManager::LEFT);

        display_.setColor(
            tile->value > 0.0f
                ? DisplayManager::YELLOW
                : DisplayManager::CYAN);


        drawFlowArrow(
            x + arrowOffset,
            y + h / 2 + 2,
            tile->value > 0.0f);


        display_.setColor(
            DisplayManager::WHITE);

        display_.drawString(
            valueX,
            y + h / 2 - 7,
            value);
    }
    else
    {
        display_.drawString(
            x + w / 2,
            y + h / 2 - 7,
            value);
    }


    // -------------------------------------------------------------------------
    // Measurement label
    // -------------------------------------------------------------------------

    display_.setTextAlignment(
        DisplayManager::CENTER);

    display_.setFont(
        ArialMT_Plain_10);

    display_.setColor(
        DisplayManager::CYAN);

    display_.drawString(
        x + w / 2,
        y + h - 18,
        observation->label);
}


void EnergyStatusScreen::drawFlowArrow(
    int x,
    int y,
    bool upward)
{
    auto thick =
        [&](int x1, int y1, int x2, int y2)
        {
            display_.drawLine(
                x1,
                y1,
                x2,
                y2);

            display_.drawLine(
                x1 + 1,
                y1,
                x2 + 1,
                y2);
        };


    if (upward)
    {
        thick(
            x,
            y + 6,
            x,
            y - 6);

        thick(
            x,
            y - 6,
            x - 3,
            y - 2);

        thick(
            x,
            y - 6,
            x + 3,
            y - 2);

        return;
    }


    thick(
        x,
        y - 6,
        x,
        y + 6);

    thick(
        x,
        y + 6,
        x - 3,
        y + 2);

    thick(
        x,
        y + 6,
        x + 3,
        y + 2);
}


String EnergyStatusScreen::formatValue(
    const SensorTile& tile,
    float value) const
{
    if (isnan(value))
    {
        return "--";
    }


    // -------------------------------------------------------------------------
    // Generic display scaling
    // -------------------------------------------------------------------------

    if (tile.displayScales &&
        tile.displayScaleCount > 0)
    {
        const float absoluteValue =
            fabs(value);

        const SensorDisplayScale* selected =
            nullptr;


        for (uint8_t i = 0;
             i < tile.displayScaleCount;
             ++i)
        {
            if (
                absoluteValue >=
                tile.displayScales[i].threshold)
            {
                selected =
                    &tile.displayScales[i];
            }
        }


        if (selected)
        {
            return String(
                       value /
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
            value,
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
