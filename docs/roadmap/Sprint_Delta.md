# Sprint Delta — Interaction Architecture

## Objective

Establish the semantic input architecture before implementing further navigation.

## The Architectural Pressure

Telemetry is intended to be a framework. A framework cannot be designed around one physical input device.

Today, the device may have a touch screen. Tomorrow, the same UI framework may be used with a rotary encoder, five-way switch, buttons, joystick, keyboard, or another input device entirely.

The question became:

> How can different physical devices produce the same meaningful interaction?

## The Principle

Input is semantic.

The UI should not care whether a user touched the screen, pressed a button, moved a joystick, or turned an encoder.

The UI should care only about what the user intended.

## The Interaction Pipeline

```text
Physical device
    ↓
Input Source
    ↓
InputManager
    ↓
Semantic InputEvent
    ↓
ScreenManager
    ↓
Active Screen
    ↓
Active Page
```

## Phase 1 — Define the Input Vocabulary

The vocabulary describes generic user actions:

```text
TAP
LONG_PRESS

SELECT
CANCEL
BACK

SCROLL_UP
SCROLL_DOWN
SCROLL_LEFT
SCROLL_RIGHT

INCREASE
DECREASE
```

The vocabulary deliberately does **not** contain application-specific meanings such as `OPEN_ABOUT_PAGE`, `TOGGLE_TIME_FORMAT`, or `TOGGLE_TEMPERATURE_UNIT`.

Those are contextual interpretations.

The input system says what happened.

The active context decides what that means.

## Phase 2 — Define the Event Model

An `InputEvent` contains the semantic action, time of interaction, repeat state, and optional interaction data such as a position.

For example:

```text
Touch at (120, 20)
        ↓
TAP + position
        ↓
WeatherScreen interprets the location
        ↓
Contextual action
```

Or:

```text
Joystick moved upward
        ↓
SCROLL_UP
        ↓
Active screen interprets the action
```

The event contract supports both spatial and non-spatial input.

## Phase 3 — Define Navigation Ownership

The central question was:

> Who decides that the application should leave the current screen and show another screen?

The answer is:

**ScreenManager owns screen-level navigation.**

But not every input event is navigation.

```text
InputEvent
    │
    ▼
ScreenManager
    │
    ├── Screen-level navigation?
    │       │
    │       ├── Yes
    │       │    └── transition to another Screen
    │       │
    │       └── No
    │
    ▼
Active Screen
    │
    ├── Interpret event locally
    │
    └── Delegate to active Page
```

The ScreenManager owns transitions. The active screen owns local meaning. The active page owns page-specific behaviour.

## Phase 4 — Define Contextual Interpretation

The same semantic action may mean different things in different contexts.

For example, `TAP + position` on the Weather screen might mean:

```text
Tap clock
    → change time presentation

Tap temperature
    → change temperature presentation
```

The same action on a Control Panel menu might mean:

```text
Tap About
    → select About page
```

The input system does not need to know either meaning.

That is the responsibility of the active context.

## Why This Matters

A touch device may produce `TAP + position`.

A joystick may produce `SCROLL_UP`.

An encoder may produce `INCREASE`.

The active context interprets those events according to the interaction model it provides.

The UI framework therefore remains independent of the physical input device.

## Breadcrumb

The next architectural question is:

> How does the active context interpret an event without becoming tightly coupled to every possible physical input device?

Only after that question is settled should Weather → Control Panel navigation resume.

## Principles Exercised

- Architecture Before Implementation
- Capabilities Before Hardware
- Input Is Semantic
- Every Abstraction Has a Cost
- Composition Before Inheritance
- Dependencies Flow Downward
- Good Architecture Should Feel Inevitable
- Architecture is User Experience
- Teach Through Architecture

## Outcome

**Complete.**

The interaction architecture was established before further navigation was implemented.
