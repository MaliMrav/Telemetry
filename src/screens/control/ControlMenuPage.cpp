#include "ControlMenuPage.h"

#include "../../display/DisplayManager.h"

const char* ControlMenuPage::title() const
{
    return "Control Panel";
}

void ControlMenuPage::onInput(const InputEvent& event)
{
    switch (event.action)
    {
        case InputAction::SCROLL_UP:
            selectedPage_ = ControlPageKind::About;
            break;

        case InputAction::SCROLL_DOWN:
            selectedPage_ = ControlPageKind::Connectivity;
            break;

        default:
            break;
    }
}
void ControlMenuPage::render(DisplayManager& display)
{
    constexpr int TITLE_Y          = 4;
    constexpr int DIVIDER_Y        = 18;
    constexpr int CONTENT_LEFT     = 10;
    constexpr int FIRST_ITEM_Y     = 30;
    constexpr int ITEM_LINE_HEIGHT = 18;
    constexpr int SCREEN_LEFT      = 0;

    display.clear(DisplayManager::BLACK);

    display.setColor(DisplayManager::WHITE);
    display.setTextAlignment(DisplayManager::CENTER);

    display.drawString(display.getWidth() / 2, TITLE_Y, title()); // title

    display.drawLine(SCREEN_LEFT, DIVIDER_Y, display.getWidth() - 1, DIVIDER_Y); // horizontal line

    display.setTextAlignment(DisplayManager::LEFT);

    int y = FIRST_ITEM_Y;

    display.drawString(CONTENT_LEFT, y, selectedPage_ == ControlPageKind::About ? "> About" : "  About"); // first item

    y += ITEM_LINE_HEIGHT;

    display.drawString(CONTENT_LEFT, y, selectedPage_ == ControlPageKind::Connectivity ? "> Connectivity" : "  Connectivity"); // second item

    display.commit();
}