#pragma once

// InputAction defines the vocabulary of user intent in this framework.
//
// Hardware sources (touch, buttons, encoder) translate physical signals
// into these abstract actions. Screens respond to actions, never to
// raw hardware signals. This is what makes input sources interchangeable.
//
// To add a new gesture or interaction, add it here first, then handle
// it in the relevant screen's onInput() method.

enum class InputAction
{
    NONE,

    TAP,
    LONG_PRESS,

    SELECT,
    CANCEL,
    BACK,

    SCROLL_UP,
    SCROLL_DOWN,
    SCROLL_LEFT,
    SCROLL_RIGHT,

    INCREASE,
    DECREASE
};