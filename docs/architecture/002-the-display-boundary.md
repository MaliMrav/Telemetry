# 002 — The Display Boundary

## Why should application code not depend directly on the display driver?

The first version of a display-driven project often looks simple:

```cpp
tft.drawString(...);
tft.drawRect(...);
tft.pushSprite(...);
```

For a prototype, this is understandable.

The problem appears when the application grows.

## The hardware begins to leak upward

If a screen talks directly to the display driver, it knows:

- which display controller is being used
- which graphics library is being used
- how the framebuffer works
- how the display is committed
- potentially how the hardware is wired

The application has become coupled to the implementation.

What happens when the display changes?

The application should not need to answer that question.

## The boundary

Telemetry introduces a display boundary:

```text
┌─────────────────────┐
│     Application     │
│       Screens       │
└──────────┬──────────┘
           ▼
┌─────────────────────┐
│   DisplayManager    │
│  Stable interface   │
└──────────┬──────────┘
           ▼
┌─────────────────────┐
│    Display Driver   │
│  MiniGrafx / SPI    │
└─────────────────────┘
```

The screen asks for capabilities:

```cpp
display.drawString(...);
display.drawLine(...);
display.fillRect(...);
display.commit();
```

It does not ask the hardware driver to perform those operations directly.

## Why is DisplayManager an abstraction?

Because it provides a stable vocabulary.

The application thinks in terms of:

- draw a string
- draw a line
- fill a rectangle
- clear the display
- commit the frame

The driver thinks in terms of:

- graphics library calls
- controller commands
- framebuffer operations
- SPI communication

Those are different vocabularies.

The DisplayManager is the boundary between them.

## The dependency direction

```text
Application
    │
    ▼
DisplayManager
    │
    ▼
Graphics Library
    │
    ▼
Display Driver
    │
    ▼
Hardware
```

The application depends on the capability.

The capability implementation depends on the technology.

The technology depends on the hardware.

## Why not abstract everything?

Because every abstraction has a cost.

The DisplayManager is useful because it hides a meaningful implementation boundary.

It prevents display technology from leaking into application code.

But adding an abstraction merely because abstractions are considered good architecture would be counterproductive.

The question is always:

> Does this boundary reduce complexity?

In this case, yes.

## The lesson

A display is a capability.

The application should consume that capability.

It should not need to know which physical device provides it.

That is the purpose of the display boundary.
