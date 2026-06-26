#pragma once

// InputManager is the central hub for all user input.
//
// Responsibilities:
//   - maintain a registry of input sources (touch, buttons, encoder)
//   - poll all registered sources each loop via update()
//   - hold a queue of pending InputEvents
//   - dispatch queued events to the active screen via dispatchTo()
//
// Input sources call InputManager::trigger() to enqueue events.
// Nothing outside InputManager needs to know which sources are registered.

#include <Arduino.h>
#include "InputEvent.h"

class IInputSource;
class ScreenManager;

class InputManager
{
public:
    static void registerSource(IInputSource* source);

    static void update();

    static void trigger(const InputEvent& event);

    static bool poll(InputEvent& event);

    static bool dispatchTo(ScreenManager& screenManager);

private:
    static constexpr uint8_t MAX_SOURCES = 4;
    static constexpr uint8_t QUEUE_SIZE   = 8;

    static IInputSource* sources_[MAX_SOURCES];
    static uint8_t sourceCount_;

    static InputEvent queue_[QUEUE_SIZE];
    static uint8_t head_;
    static uint8_t tail_;
    static uint8_t count_;

    static bool push(const InputEvent& event);
};