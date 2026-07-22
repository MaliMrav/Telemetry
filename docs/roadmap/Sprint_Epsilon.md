# Sprint Epsilon — Screen Navigation

## Objective

> **Introduce screen-to-screen navigation without compromising the interaction architecture established in Sprint Delta.**

The key distinction remains:
```text
InputEvent
    │
    ▼
ScreenManager
    │
    ├── System-level event?
    │       └── System context handles it
    │
    ├── Screen-level navigation?
    │       └── ScreenManager transitions screens
    │
    └── Otherwise
            │
            ▼
        Active Screen
            │
            ▼
        Contextual interpretation
```
The central question for Sprint Epsilon is:

> ***How does an interaction become a request to change screens?***

Not:

> "How does WeatherScreen call ControlPanelScreen?"

That would be the wrong dependency direction.

---

## The target architecture

We already have;
```text
Physical Input
      │
      ▼
Input Source
      │
      ▼
InputManager
      │
      ▼
InputEvent
      │
      ▼
ScreenManager
      │
      ├── Not screen navigation
      │       │
      │       ▼
      │   Active Screen
      │       │
      │       ▼
      │   Contextual interpretation
      │
      └── Screen navigation
              │
              ▼
          Transition
```
Sprint Epsilon should implement the second branch.

For example:
```text
InputEvent
    │
    ▼
ScreenManager
    │
    │  "This is a request to leave the current screen."
    ▼
WeatherScreen
    │
    │  screen-level navigation request
    ▼
ScreenManager
    │
    ▼
ControlPanelScreen
```
The important thing is that **WeatherScreen should not own the transition.**

It may interpret an interaction as a navigation request, but the actual transition remains a `ScreenManager` responsibility.

---


## The first architectural decision

Before writing code, I believe Sprint Epsilon should establish the navigation contract.

Something conceptually like:
```text
InputEvent
    │
    ▼
ScreenManager
    │
    ├── Can this event be handled as screen navigation?
    │
    ├── Yes
    │     │
    │     └── transition to target Screen
    │
    └── No
          │
          ▼
      Active Screen
```
But we need to be precise about where the meaning is determined.

For example, a touch event:

```text
TAP + position
````

should not inheritly mean:
```text
OPEN_CONTROL_PANEL
```

That would violate our existing semantic boundary.

Instead, the active context may determine:

```text
TAP + position
        │
        ▼
WeatherScreen
        │
        ▼
"This interaction means:
request ControlPanelScreen"
```
Then the navigation request must be represented in a way that does not make `WeatherScreen` directly manipulate `ScreenManager`.

That is the architectural problem Sprint Epsilon needs to solve.

---

## The most important distinction

We now have three different things:

**1. Physical interaction**
```text
Touch
Encoder
Joystick
Keyboard
```
**2. Semantic input**
```text
TAP
SELECT
SCROLL_UP
BACK
```
**3. Application interpretation**
```text
Toggle clock format
Open Control Panel
Reset device
Enter AP mode
Begin OTA update
```
These must remain distinct.

So:
```text
TAP
```
is not:
```text
OPEN_CONTROL_PANEL
```
and:
```text
OPEN_CONTROL_PANEL
```
is not necessarily:
```text
CHANGE_SCREEN
```
The final meaning depends on the context.

That is the architectural pressure Sprint Epsilon must resolve.

---

## Sprint Epsilon phases

### Phase 1 — Define the Navigation Contract

What is a screen?

What is a screen transition?

What does `ScreenManager` own?

What does the active screen own?

### Phase 2 — Define Navigation Requests

How does a context express:

> "This interaction means the user wants to leave this screen?"

Without directly coupling the screen to `ScreenManager`?

### Phase 3 — Implement the First Transition

The thin slice:
```text
WeatherScreen
      │
      │ interaction
      ▼
ControlPanelScreen
```
and:
```text
ControlPanelScreen
      │
      │ BACK
      ▼
WeatherScreen
```
### Phase 4 — Establish Transition State

Define the lifecycle:
```text
Current Screen
      │
      ▼
leave()
      │
      ▼
transition
      │
      ▼
enter()
```
and verify that:

- the old screen is left correctly
- the new screen is entered correctly
- state is preserved where appropriate
- the active screen is unambiguous

## The thin slice I would aim for

Ultimately:
```text

                    ┌──────────────────────┐
                    │    WeatherScreen     │
                    │                      │
                    │  Context interprets  │
                    │  semantic input      │
                    └──────────┬───────────┘
                               │
                               │ navigation request
                               ▼
                    ┌──────────────────────┐
                    │    ScreenManager     │
                    │                      │
                    │  Owns transition     │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │ ControlPanelScreen   │
                    │                      │
                    │  Owns ControlPageKind│
                    └──────────────────────┘
```
Then the reverse:
```text
ControlPanelScreen
        │
        │ BACK
        ▼
ScreenManager
        │
        ▼
WeatherScreen
```
That would give us a complete, demonstrable navigation architecture without prematurely solving every possible navigation problem.
