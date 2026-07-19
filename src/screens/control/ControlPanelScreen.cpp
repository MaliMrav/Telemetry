#include "ControlPanelScreen.h"

ControlPanelScreen::ControlPanelScreen(DisplayManager& display)
    : display_(display)
    , activePage_(nullptr)
{
    activate(menuPage_);
}

void ControlPanelScreen::activate(ControlPage& page)
{
    if (activePage_)
    {
        activePage_->onLeave();
    }

    activePage_ = &page;

    activePage_->onEnter();
}

void ControlPanelScreen::update()
{
    if (activePage_)
    {
        activePage_->render(display_);
    }
}

void ControlPanelScreen::onInput(const InputEvent& event)
{
    if (activePage_)
    {
        activePage_->onInput(event);

        // If the active page is the menu, check if the user has selected a different page.
        if (activePage_ == &menuPage_)
        {
            switch (menuPage_.destination())
            {
                case ControlPageKind::About:
                    activate(aboutPage_);
                    break;

                case ControlPageKind::Connectivity:
                    activate(connectivityPage_);
                    break;
            }
            return;
        }
    }

    if (event.action == InputAction::BACK)
    {
       if (activePage_ == &menuPage_)
       {
           //
           // Already on the menu page, so leave Control Panel.
           //
       }
       else
       {
           activePage_->onLeave();
           activePage_ = &menuPage_;
           activePage_->onEnter();
       }
    }
    else
    {
        //
        // ScreenManager will return to Weather later.
        //
    }

    return;
}