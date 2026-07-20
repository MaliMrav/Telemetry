# 005 — Who Owns Navigation?

## When an input event occurs, who decides whether the application should leave the current screen?

This question appears simple.

It is not.

Should the system:

- change screens?
- scroll a menu?
- select an item?
- change a presentation format?
- pass the event to a page?

The answer is an ownership boundary.

## The wrong answer

The input device should not decide.

A touch controller should not contain:

```text
if the user taps here:
    open the Control Panel
```

A rotary encoder should not know:

```text
three clicks means:
    open Connectivity
```

The input layer produces semantic interaction.

It does not own application navigation.

## The ScreenManager owns screen-level navigation

The contract is:

```text
InputEvent
    │
    ▼
ScreenManager
    │
    ├── Screen-level navigation?
    │       │
    │       ├── Yes
    │       │    └── Transition to another Screen
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

The ScreenManager answers:

> Does this event require leaving the current screen?

If yes, it owns the transition.

If no, the event remains within the current screen.

## The active screen owns local interpretation

For example:

```text
SCROLL_DOWN
    │
    ▼
ControlPanelScreen
    │
    ▼
ControlMenuPage
    │
    ▼
Move selection to the next item
```

The event did not require a screen transition.

It was interpreted locally.

Another event might be:

```text
BACK
    │
    ▼
ControlPanelScreen
    │
    ▼
Return to WeatherScreen
```

The exact transition remains a screen-level concern.

## Pages do not navigate between screens

A page belongs to a screen:

```text
ControlPanelScreen
    │
    ├── ControlMenuPage
    ├── AboutPage
    └── ConnectivityPage
```

A page may interpret local input.

It should not decide that the application should leave the Control Panel and activate another top-level screen.

That responsibility belongs above it.

## The ownership model

```text
Input
    │
    ▼
ScreenManager
    │
    ▼
Active Screen
    │
    ▼
Active Page
```

Each layer has a responsibility:

### Input

Detects and normalises interaction.

### ScreenManager

Owns top-level screen transitions.

### Active Screen

Interprets events within its own context.

### Active Page

Handles local content and interaction.

## The important conditional

Not every input event is navigation.

This is critical.

Conceptually:

```text
InputEvent
    │
    ▼
Does this require screen-level navigation?
    │
    ├── Yes → ScreenManager transitions
    │
    └── No  → Active Screen handles locally
```

That distinction allows the same input architecture to support both:

```text
TAP
```

and:

```text
SCROLL_DOWN
```

without confusing local interaction with application navigation.

## The lesson

Navigation is application-level policy.

Input is interaction.

The active screen is context.

The ScreenManager owns the boundary between them.

That ownership prevents lower-level components from becoming aware of higher-level application policy.
