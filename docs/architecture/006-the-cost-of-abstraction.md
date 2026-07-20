# 006 — The Cost of Abstraction

## When is an abstraction useful?

When does it become a burden?

Telemetry deliberately introduces boundaries.

That does not mean every piece of code should be wrapped in an abstraction.

Abstraction is not automatically good.

It has a cost.

## Every boundary adds something

An abstraction may introduce:

- another interface
- another type
- another file
- another layer of indirection
- another concept a developer must understand

That cost is real.

Therefore the question should never be:

> Can we abstract this?

The better question is:

> What complexity does this abstraction remove?

## A useful abstraction

Consider the display boundary:

```text
Application
    │
    ▼
DisplayManager
    │
    ▼
Display Driver
```

The application does not need to know:

- which driver is installed
- how the framebuffer works
- how pixels reach the controller

The abstraction removes those concerns.

That is a good trade.

## A harmful abstraction

Imagine:

```text
Application
    │
    ▼
DisplayService
    │
    ▼
RenderCoordinator
    │
    ▼
GraphicsFacade
    │
    ▼
DisplayManager
    │
    ▼
Driver
```

If every layer merely forwards the same call, the architecture has become harder to understand.

The abstraction has not reduced complexity.

It has redistributed it.

## The test

A useful abstraction should answer at least one of these questions:

- Does it hide a meaningful implementation detail?
- Does it create a stable boundary?
- Does it allow a capability to have multiple implementations?
- Does it reduce coupling?
- Does it make the architecture easier to understand?

If the answer is no, the abstraction deserves suspicion.

## Abstraction should follow pressure

Telemetry does not attempt to predict every future requirement.

Architecture responds to real pressure.

For example:

```text
One display
    │
    ▼
Direct driver calls
```

may be sufficient for a prototype.

But when the project needs:

- multiple display implementations
- application code independent of hardware
- a stable rendering vocabulary

a display boundary becomes valuable.

Likewise:

```text
Touch only
```

may be sufficient for an early prototype.

But when the framework must support:

- touch
- joysticks
- encoders
- switches

the semantic input boundary becomes valuable.

The architecture emerges from pressure.

## The balance

The goal is not:

```text
Maximum abstraction
```

The goal is:

```text
Minimum complexity
```

Sometimes the simplest design is direct.

Sometimes the simplest design requires a boundary.

The architect must distinguish between the two.

## The principle

This is why Telemetry's principle exists:

> Every Abstraction Has a Cost.

Abstractions should reduce complexity.

Never increase it.

## The lesson

Good architecture is not the architecture with the most layers.

It is the architecture with the right boundaries.

The right abstraction makes the system easier to understand.

The wrong abstraction makes the system harder to understand while claiming to make it easier.

The test is simple:

> After introducing the abstraction, is the system clearer?

If not, the abstraction may not belong.
