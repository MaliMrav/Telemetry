#include "ScreenManager.h"

void ScreenManager::show(Screen* screen)
{
    if (currentScreen_)
    {
        currentScreen_->leave();
    }

    currentScreen_ = screen;

    if (currentScreen_)
    {
        currentScreen_->enter();
    }
}

void ScreenManager::update()
{
    if (currentScreen_)
    {
        currentScreen_->update();
    }
}

void ScreenManager::onInput(const InputEvent& event)
{
    if (currentScreen_)
    {
        currentScreen_->onInput(event);
    }
}