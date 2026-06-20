#include "InputManager.h"

#include "InputSource.h"
#include "../ui/ScreenManager.h"
#include "InputAction.h"

IInputSource* InputManager::sources_[InputManager::MAX_SOURCES] = {};
uint8_t InputManager::sourceCount_ = 0;

InputEvent InputManager::queue_[InputManager::QUEUE_SIZE] = {};
uint8_t InputManager::head_  = 0;
uint8_t InputManager::tail_  = 0;
uint8_t InputManager::count_ = 0;

void InputManager::registerSource(IInputSource* source)
{
    if (!source || sourceCount_ >= MAX_SOURCES)
    {
        return;
    }

    sources_[sourceCount_++] = source;
}

void InputManager::update()
{
    for (uint8_t i = 0; i < sourceCount_; ++i)
    {
        if (sources_[i])
        {
            sources_[i]->update();
        }
    }
}

bool InputManager::push(const InputEvent& event)
{
    if (count_ >= QUEUE_SIZE)
    {
        return false;
    }

    queue_[tail_] = event;
    tail_ = (tail_ + 1) % QUEUE_SIZE;
    ++count_;

    return true;
}

void InputManager::trigger(const InputEvent& event)
{
    push(event);
}

bool InputManager::poll(InputEvent& event)
{
    if (count_ == 0)
    {
        return false;
    }

    event = queue_[head_];
    head_ = (head_ + 1) % QUEUE_SIZE;
    --count_;

    return true;
}

bool InputManager::dispatchTo(ScreenManager& screenManager)
{
    bool handled = false;

    InputEvent event;
    while (poll(event))
    {
        if (event.action != InputAction::NONE)
        {
            screenManager.onInput(event);
            handled = true;
        }
    }

    return handled;
}