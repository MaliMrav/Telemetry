#pragma once

#include "../ControlPage.h"

class AboutPage : public ControlPage
{
public:
    virtual const char* title() const override;

    virtual void render(DisplayManager& display) override;

    virtual void onInput(const InputEvent&)
    {
    } // ignore input events, as this page currently has no interactive behaviour.
};