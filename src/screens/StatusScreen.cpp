#include "StatusScreen.h"

void StatusScreen::onInput(const InputEvent& event)
{
    switch (event.action)
    {
        case InputAction::BACK:
            // reset/home
            break;

        case InputAction::PREVIOUS_SCREEN:
            // previous page
            break;

        case InputAction::NEXT_SCREEN:
            // next page
            break;

        case InputAction::SCROLL_UP:
            // scroll up
            break;

        case InputAction::SCROLL_DOWN:
            // scroll down
            break;

        default:
            break;
    }
}