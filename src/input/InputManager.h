#pragma once

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