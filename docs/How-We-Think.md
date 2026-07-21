# How We Think

These principles guide every architectural decision made within Telemetry.

They are intentionally technology-agnostic.

Frameworks change.

Hardware changes.

Programming languages evolve.

Good engineering principles endure.

# 1. Architecture Before Implementation

Every significant change should begin with architecture.

A well-designed architecture makes implementation feel inevitable.

Code should be the consequence of good design, not the process used to discover it.

# 2. The Repository Should Reflect the Architecture

A well-structured repository should communicate the architecture before a single source file is opened.

Developers should be able to browse the directory tree and immediately understand:

- what the major applications are
- how responsibilities are separated
- where capabilities belong
- how the framework is organised

Documentation should explain the architecture, not compensate for it.

Good architecture should be visible.

Before a single line of code is read.

# 3. Capabilities Before Hardware

The framework thinks in capabilities.

Not displays.

Not touch controllers.

Not Wi-Fi chips.

Hardware provides capabilities.

The framework exposes them.

Applications consume them.

# 4. Observations Before Information

Firmware should observe.

It should collect measurements faithfully.

Transformation into useful information should be deterministic.

Inference belongs elsewhere.

Telemetry deliberately stops before artificial intelligence.

# 5. Information Before Intelligence

Telemetry transforms observations into information.

It intentionally leaves intelligence, automation, analytics and decision making to upstream systems.

Home Assistant.

Node-RED.

Grafana.

Cloud services.

Artificial Intelligence.

Those platforms exist to reason.

Telemetry exists to observe.

# 6. One Responsibility Per Capability

Displays render.

Sensors observe.

Storage persists.

Connectivity transports.

Diagnostics observe.

Each capability should have one responsibility, and perform it well.

# Principle #7 — Input Is Semantic

Input devices produce physical interaction.

The framework translates that interaction into semantic events.

The appropriate context interprets those events.

Screens do not care whether an action originated from:

- touch
- buttons
- rotary encoders
- keyboards
- gamepads
- joysticks

The architectural boundary is:

```text
Physical Input
      │
      ▼
Input Source
      │
      ▼
InputManager
      │
      │ normalises interaction
      ▼
InputEvent
      │
      ▼
Appropriate Context
```

A touch screen may produce:

```text
TAP + position
```

A rotary encoder may produce:

```text
SCROLL_UP
```

A five-way switch may produce:

```text
SELECT
```

A keyboard may produce another semantic action.

The input device should not need to know what the application will do with that action.

---

## Input Is Not Meaning

The `InputManager` normalises interaction.

It does not decide application-specific meaning.

For example, it should not decide that:

```text
SCROLL_DOWN
```

means:

```text
OPEN_CONNECTIVITY_PAGE
```

nor that:

```text
TAP + position
```

means:

```text
TOGGLE_TIME_FORMAT
```

nor that:

```text
A particular key sequence
```

means:

```text
RESET_DEVICE
```

Those decisions belong to the appropriate context and scope.

The event describes the interaction.

The context interprets it.

---

## Navigation Is Only One Possible Interpretation

An input event may result in:

```text
InputEvent
      │
      ▼
Interpretation
      │
      ├── System-level operation
      │
      ├── Screen-level navigation
      │
      └── Contextual interaction
```

Therefore:

> **ScreenManager owns screen-level navigation.**

But:

> **ScreenManager does not own the meaning of every input event.**

The same semantic action may mean different things in different contexts.

For example:

```text
SCROLL_DOWN
```

may:

- move through a Control Panel menu
- move through an information page
- adjust a value
- be ignored

The input event describes what happened.

The appropriate owner interprets it.

---

## Ownership Follows Scope

The conceptual ownership model is:

```text
System-level intent
        ↓
System owner / capability

Screen-level intent
        ↓
ScreenManager

Contextual interaction
        ↓
Active Screen / Page
```

This means:

- system-wide operations belong to system-level owners
- screen transitions belong to `ScreenManager`
- local interaction belongs to the active screen or page

A hidden input sequence might request:

- entering AP mode
- requesting an OTA update
- resetting the device
- restoring factory defaults

Those operations should not be coupled to the visible screen.

Likewise, the input source should not know that the application contains a `ConnectivityPage`.

---

## The Principle

The original idea remains:

> Screens never care whether an action originated from touch, buttons, rotary encoders, keyboards, or gamepads.

But the architectural statement is more precise:

> **Input is semantic. Meaning is contextual. Ownership follows scope.**

Input devices produce semantic actions.

`InputManager` normalises and transports them.

The appropriate context interprets them.

`ScreenManager` owns screen-level lifecycle and transitions.

System-level owners handle operations affecting the device as a whole.

The input device should never need to know the structure of the application.

That is what makes the input boundary reusable.

And that is why a touch screen is only one possible input device—not the definition of the framework.

# 8. Every Abstraction Has a Cost

Abstractions should reduce complexity.

Never increase it.

If an abstraction makes the system harder to understand, it probably shouldn't exist.

# 9. Composition Before Inheritance

Objects should be assembled from smaller responsibilities.

Inheritance should be rare.

Composition keeps systems understandable.

# 10. Dependencies Flow Downward

High-level policy should never depend upon hardware.

Applications depend on capabilities.

Capabilities depend on drivers.

Drivers depend on hardware.

Never the other way around.

Dependencies should become simpler as they move downward.

# 11. Firmware Should Be Observable

Firmware should always be able to explain itself.

If the firmware cannot explain what it is doing,

neither can the engineer.

Health.

Memory.

Connectivity.

Boot progress.

Versions.

Timing.

Failures.

Unknown behaviour is almost always harder to debug than incorrect behaviour.

# 12. Good Architecture Should Feel Inevitable

The best architecture rarely feels surprising.

Instead, it produces the reaction:

> "Of course...
> why would it be designed any other way?"

That is usually a sign the abstractions are correct.

# 13. Architecture is User Experience

Not just for users.

For contributors.

For future maintainers.

For the engineer reading the code five years from now.

Good architecture reduces cognitive load.

A system should feel discoverable long before it feels familiar.

# 14. Teach Through Architecture

Telemetry exists to teach.

Not by simplifying problems.

But by making good engineering visible.

The repository should help developers understand not only *what* the code does...

...but *why* it is designed that way.

# 15. Clarity Over Cleverness

Clever code impresses today's developer.

Clear code teaches tomorrow's developer.

Readable firmware is maintainable firmware.

Prefer obvious solutions over clever ones.

Future readers include your future self.

# 16. Framework Before Application

The framework should never become tightly coupled to a single application.

Weather is one implementation of the framework.

Telemetry is the framework.

The framework must always remain reusable.

# 17. The Knowledge Pyramid

Telemetry occupies only part of the engineering knowledge pyramid.

            Wisdom
               ▲
          Knowledge
               ▲
         Intelligence
               ▲
         Information
               ▲
             Data
               ▲
        Observations

Telemetry is responsible for moving observations toward information.

It deliberately stops there.

Everything above belongs to higher-level systems.

# 18. The Real Product

Telemetry is not the product.

The dashboard is not the product.

The framework is not the product.

The real product is helping someone say:

> "Ahh... now I understand why."

---

# A Final Thought

These principles are not rules.

They are reminders.

They exist to help us make consistent decisions as Telemetry evolves.

If a future design forces us to challenge one of these principles, the discussion itself is valuable.

Good engineering is not about following rules.

It is about understanding why they exist.