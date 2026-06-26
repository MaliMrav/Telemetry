#pragma once

// IInputSource is the interface every input device must implement.
//
// Each source is responsible for polling its hardware and calling
// InputManager::trigger() when an event is ready. Sources register
// themselves with InputManager once at boot.
//
// Examples: TouchManager, ButtonManager, EncoderManager.

class IInputSource
{
public:
    virtual ~IInputSource() = default;
    virtual void update() = 0;
};