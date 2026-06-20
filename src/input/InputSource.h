#pragma once

class IInputSource
{
public:
    virtual ~IInputSource() = default;
    virtual void update() = 0;
};