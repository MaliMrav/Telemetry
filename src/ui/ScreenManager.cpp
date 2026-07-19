#include "ScreenManager.h"

#include "../touch/TouchManager.h"

void ScreenManager::bindTouchManager(TouchManager* touchManager)
{
    touchManager_ = touchManager;
}

void ScreenManager::activate(Screen* screen)
{
    if (currentScreen_)
    {
        currentScreen_->leave();
    }

    currentScreen_ = screen;

    if (touchManager_ && currentScreen_)
    {
        switch (currentScreen_->kind())
        {
            case ScreenKind::Weather:
                touchManager_->setProfile(TouchManager::Profile::Weather);
                break;

            case ScreenKind::ControlPanel:
                touchManager_->setProfile(TouchManager::Profile::Generic);
                break;

            case ScreenKind::Calibration:
                touchManager_->setProfile(TouchManager::Profile::Calibration);
                break;

            default:
                touchManager_->setProfile(TouchManager::Profile::Generic);
                break;
        }
    }

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

void ScreenManager::onInput(
    const InputEvent& event)
{
    //
    // ScreenManager owns screen-level navigation.
    //
    // If the event represents a request to leave the current
    // screen or transition to another screen, that decision
    // belongs here.
    //
    // Events that do not represent screen-level navigation
    // belong to the active screen.
    //

    if (currentScreen_)
    {
        currentScreen_->onInput(event);
    }
}