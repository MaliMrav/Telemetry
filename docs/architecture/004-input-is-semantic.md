# 004 — Input Is Semantic

## Why should a screen not care whether an action originated from touch, a joystick, or a rotary encoder?

A user does not think:

> The GPIO pin changed state.

A user thinks:

> I want to move down.

Or:

> I want to select this.

Or:

> I want to go back.

That distinction is the foundation of semantic input.

## Physical input is not application intent

Different devices produce different physical signals:

```text
Touch
    └── coordinates + contact

Joystick
    └── direction + movement

Rotary Encoder
    └── rotation + button press

Keyboard
    └── key events
```

The application should not need to understand all of these physical representations.

Instead:

```text
Physical Input
      │
      ▼
Input Source
      │
      ▼
Input Manager
      │
      ▼
Semantic InputEvent
```

## The semantic boundary

A touch input might become:

```text
TAP + position
```

A joystick movement might become:

```text
SCROLL_DOWN
```

A rotary encoder movement might become:

```text
SCROLL_UP
```

The screen receives the meaning of the interaction.

It does not receive the hardware implementation.

## Context interprets meaning

```text
Touch
  │
  ▼
TAP + position
  │
  ▼
WeatherScreen
  │
  ▼
Position is inside the clock focus area
  │
  ▼
Change time presentation
```

Or:

```text
Rotary Encoder
  │
  ▼
SCROLL_DOWN
  │
  ▼
ControlMenuPage
  │
  ▼
Move selection to the next menu item
```

The same semantic action can mean different things in different contexts.

That is contextual interpretation.

## The event model

```text
InputEvent
├── Action
│   ├── TAP
│   ├── SELECT
│   └── SCROLL_DOWN
│
└── Interaction data
    └── Position, when applicable
```

The event contract does not force every input device to pretend it has coordinates.

## The lesson

Input devices produce physical signals.

The framework produces semantic events.

The application interprets those events within context.

That is the difference between:

> What happened to the hardware?

and:

> What did the user intend to do?

Telemetry is interested in the second question.
