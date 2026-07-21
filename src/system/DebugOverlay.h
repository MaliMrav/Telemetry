#pragma once

// DebugOverlay renders a diagnostic bar directly on the TFT display.
//
// Enabled only when DEBUG_OVERLAY is defined — either in config_override.h
// or as a build flag. Compiles away completely in production builds.
//
// Usage from any screen, after commit():
//
//     DebugOverlay::record(event);   // call from onInput()
//     DebugOverlay::draw(display_);  // call at the end of update()
//
// The overlay draws over the bottom of the screen after the framebuffer
// has been committed, so it does not interfere with normal rendering.

#ifdef DEBUG_OVERLAY

#include <Arduino.h>
#include "../display/DisplayManager.h"
#include "../input/InputEvent.h"
#include "../input/InputAction.h"

namespace DebugOverlay
{
    struct State
    {
        InputEvent  lastEvent;
        bool        hasEvent  = false;
        const char* lastLabel = "";
        int16_t     lastRawX  = 0;
        int16_t     lastRawY  = 0;
    };

    inline State& state()
    {
        static State s;
        return s;
    }

    inline void record(
        const InputEvent& event,
        const char* label = "")
    {
        state().lastEvent = event;
        state().hasEvent  = true;
        state().lastLabel = label;
    }

    inline void recordRaw(
        int16_t rawX,
        int16_t rawY)
    {
        state().lastRawX = rawX;
        state().lastRawY = rawY;
    }

    inline void draw(DisplayManager& display)
    {
        if (!state().hasEvent) return;

        const InputEvent& e = state().lastEvent;

        // Background bar — tall enough for two lines of 10px font
        display.setColor(DisplayManager::BLACK);
        display.fillRect(0, 296, 240, 24);

        const char* actionName = "---";
        switch (e.action)
        {
            case InputAction::TAP:             actionName = "TAP";  break;
            case InputAction::SELECT:          actionName = "SEL";  break;
            case InputAction::BACK:            actionName = "BACK"; break;
            case InputAction::NEXT_SCREEN:     actionName = "NXT";  break;
            case InputAction::PREVIOUS_SCREEN: actionName = "PRV";  break;
            case InputAction::SCROLL_UP:       actionName = "S_UP"; break;
            case InputAction::SCROLL_DOWN:     actionName = "S_DN"; break;
            default:                           break;
        }

        // Line 1 (yellow): mapped action and screen coordinates
        String mapped = String(actionName);
        if (e.hasPosition)
        {
            mapped += " x=" + String(e.position.x) +
                      " y=" + String(e.position.y);
        }
        else
        {
            mapped += " (no pos)";
        }

        display.setTextAlignment(DisplayManager::LEFT);
        display.setColor(DisplayManager::YELLOW);
        display.drawString(4, 297, mapped);

        // Line 2 (blue): raw ADC coordinates from XPT2046
        String raw = "raw x=" + String(state().lastRawX) +
                     " y="   + String(state().lastRawY);

        display.setColor(DisplayManager::BLUE);
        display.drawString(4, 308, raw);

        display.commit();
    }
}

// Convenience macros so call sites read clearly
#define DBG_RECORD(event)        DebugOverlay::record(event)
#define DBG_RECORD_L(event, lbl) DebugOverlay::record(event, lbl)
#define DBG_RAW(rx, ry)          do { DebugOverlay::state().lastRawX = (rx); DebugOverlay::state().lastRawY = (ry); } while(0)
#define DBG_DRAW(display)        DebugOverlay::draw(display)

#else

// Production build — all calls compile away
#define DBG_RECORD(event)        (void)0
#define DBG_RECORD_L(event, lbl) (void)0
#define DBG_RAW(rx, ry)          (void)0
#define DBG_DRAW(display)        (void)0

#endif
