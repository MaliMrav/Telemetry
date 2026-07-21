# Architecture

Telemetry began as a weather station.

It is now becoming something more deliberate.

The project is evolving into a reusable firmware framework whose first application happens to be a weather display.

That distinction explains most of the architecture.

The question is no longer:

> "How do we make this particular device work?"

It is:

> "How do we build a system in which different applications, data sources, displays and input devices can coexist without knowing unnecessary details about one another?"

---

## The Architectural Direction

The overall direction is:

```text
Application
    │
    ▼
Framework Capabilities
    │
    ▼
Hardware Abstractions
    │
    ▼
Drivers
    │
    ▼
Physical Hardware
```

The application should express policy.

Capabilities should provide reusable services.

Drivers should translate hardware.

Physical devices should remain implementation details.

The architecture deliberately moves knowledge downward.

---

# The Major Boundaries

## The Display Boundary

Application code should not depend directly on a display driver.

The intended relationship is:

```text
Application / Screen
        │
        ▼
DisplayManager
        │
        ▼
Display Driver
        │
        ▼
Physical Display
```

A screen should be able to say:

```cpp
display.drawString(...);
display.drawLine(...);
display.commit();
```

It should not need to know whether the underlying hardware is:

- ILI9341
- ST7735
- another display controller

The screen describes what should be rendered.

The display capability decides how that rendering reaches the hardware.

This is the display boundary.

---

## The Data Boundary

The application should not care where observations came from.

The conceptual flow is:

```text
Observation
      │
      ▼
Data Source
      │
      ▼
Repository / Model
      │
      ▼
Application
      │
      ▼
Presentation
```

The source may be:

- MQTT
- a local sensor
- a file
- another transport
- a future data source not yet imagined

The application should consume information through a stable contract.

It should not need to know how the data arrived.

This keeps observation separate from presentation.

It also preserves the distinction between:

```text
Observation
```

and:

```text
Information
```

Telemetry observes and presents information.

It deliberately does not become an intelligence layer.

---

# The Input Boundary

Input is semantic.

The physical device should not determine application meaning.

The conceptual flow is:

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
Appropriate Context
```

A physical interaction might originate from:

- touch
- buttons
- a five-way switch
- a rotary encoder
- a keyboard
- a joystick

The input source translates the physical interaction into a semantic action.

For example:

```text
Touch
  ↓
TAP + position
```

or:

```text
Rotary encoder turned clockwise
  ↓
SCROLL_DOWN
```

The screen should not care which device produced the event.

The event expresses interaction.

The appropriate context determines meaning.

---

## The InputManager Does Not Decide Application Meaning

The `InputManager` has a deliberately limited responsibility.

It:

- receives events from input sources
- normalises physical interaction into semantic events
- queues events
- delivers events to the appropriate higher-level context

It does **not** decide that:

```text
SCROLL_DOWN
```

means:

```text
Open Connectivity
```

It does **not** decide that:

```text
TAP + position
```

means:

```text
Toggle Time Format
```

And it does **not** decide that:

```text
A particular key sequence
```

means:

```text
Reset Device
```

Those are contextual decisions.

The InputManager describes interaction.

The appropriate owner interprets it.

---

# Navigation Is Only One Possible Meaning

This distinction is central to the architecture.

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

Navigation is therefore not synonymous with input.

It is one possible interpretation of input.

For example:

```text
SCROLL_DOWN
```

might mean:

```text
Control Panel
    ↓
Connectivity
```

or:

```text
About Page
    ↓
Next item
```

or:

```text
Current context
    ↓
Adjust a value
```

The event is the same.

The meaning depends on context.

---

# Navigation Ownership

The question:

> "Who owns navigation?"

has a precise answer.

**ScreenManager owns screen-level navigation.**

But that answer must not be interpreted as:

> "ScreenManager owns the meaning of every input event."

The complete model is:

```text
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
      │
      ├── System-level operation?
      │       └── System owner / capability
      │
      ├── Screen-level navigation?
      │       └── ScreenManager
      │
      └── Contextual interaction?
              └── Active Screen / Page
```

The distinction is:

```text
InputManager
    owns normalisation and delivery

Appropriate Context
    owns interpretation

ScreenManager
    owns screen lifecycle and transitions
```

This is the architectural answer.

---

# System-Level Input

Not every input belongs to the visible screen.

A keyboard sequence, button sequence, or other interaction may request an operation affecting the entire device.

Examples include:

```text
Hidden key sequence
        │
        ▼
Factory Reset
```

or:

```text
Key sequence
        │
        ▼
Enter AP Mode
```

or:

```text
Input sequence
        │
        ▼
Request OTA Update
```

or:

```text
Special input
        │
        ▼
Reboot Device
```

These are not screen navigation.

The active screen should not need to know that a particular key sequence means:

```text
Reset the device
```

Likewise, the input source should not know that the application contains a `ConnectivityPage`.

The architecture therefore separates:

```text
Physical interaction
        ↓
Semantic input
        ↓
Contextual interpretation
        ↓
Scope-specific ownership
```

---

# Screen-Level Navigation

When an input event is interpreted as a screen transition, the `ScreenManager` owns that transition.

The active screen does not directly replace itself.

The boundary is:

```text
InputEvent
      │
      ▼
Context interprets event
      │
      ▼
Screen-level navigation intent
      │
      ▼
ScreenManager
      │
      ▼
Screen transition
```

This preserves screen lifecycle ownership.

The `ScreenManager` knows:

- which screen is active
- when a screen is entered
- when a screen is left
- which screen becomes active next

The active screen should not need to own the collection of screens.

The `ScreenManager` does.

---

# Contextual Interaction

If the event is not a system operation and not screen-level navigation, it remains within the active context.

For example:

```text
Control Panel
    SCROLL_DOWN
          ↓
    Select next menu item
```

or:

```text
About Page
    SCROLL_DOWN
          ↓
    Show the next information item
```

or:

```text
Weather Screen
    TAP + position
          ↓
    Interpret the selected focus area
```

The active screen or page owns this interpretation.

The same semantic action may have different meanings in different contexts.

That is not ambiguity.

That is contextual interpretation.

---

# The System Lifecycle

The system is orchestrated through the application lifecycle.

Conceptually:

```text
Boot
  │
  ▼
Initialise Hardware
  │
  ▼
Initialise Capabilities
  │
  ▼
Initialise Services
  │
  ▼
Select Initial Screen
  │
  ▼
Run
```

During normal operation:

```text
SystemManager
      │
      ├── OTA
      ├── Data Source
      ├── InputManager
      └── ScreenManager
```

The system manager coordinates the lifecycle.

It should not become the owner of every responsibility.

Coordination is not the same as implementation.

---

# Dependency Direction

Dependencies should flow downward.

```text
Application
    │
    ▼
Capabilities
    │
    ▼
Drivers
    │
    ▼
Hardware
```

The opposite direction should be avoided.

A display driver should not know about a weather screen.

A touch controller should not know about the Control Panel.

An MQTT data source should not know how a temperature is rendered.

The higher layer expresses policy.

The lower layer provides capability.

---

# The Repository Should Reflect the Architecture

The directory structure is part of the architecture.

A developer should be able to understand the broad design by browsing the repository.

For example:

```text
src/
├── data/
├── display/
├── hardware/
├── input/
├── models/
├── mqtt/
├── ota/
├── screens/
├── system/
├── touch/
├── ui/
└── wifi/
```

The exact structure may evolve.

The architectural intent should remain visible.

A repository should not require a developer to read every file before discovering where responsibilities live.

---

# Architecture Before Implementation

A recurring principle in Telemetry is:

> Architecture precedes implementation.

Before adding a feature, ask:

- What responsibility does this introduce?
- Which layer owns that responsibility?
- What should this code know?
- What should this code not know?
- Does the repository structure reflect the design?
- Does the abstraction reduce complexity?

Only then should implementation begin.

This is why architectural work may appear slower than simply writing code.

The time is not lost.

It is being spent making later code more inevitable.

---

# The Cost of Abstraction

Every boundary introduces a cost.

There may be:

- more files
- more interfaces
- more indirection
- more concepts to explain

That cost is justified only when the abstraction reduces complexity somewhere else.

The question is not:

> "Can we abstract this?"

The question is:

> "Does the abstraction make the system easier to understand, change and reuse?"

If not, the abstraction should be reconsidered.

This is why Telemetry deliberately avoids abstraction for its own sake.

---

# The Architectural Test

A good design should allow a future developer to ask:

> "Why is this boundary here?"

and find a meaningful answer.

For example:

### Why is there a DisplayManager?

Because screens should not depend directly on display hardware.

### Why is there an InputManager?

Because physical input devices should be interchangeable and screens should consume semantic actions.

### Why does ScreenManager own transitions?

Because screen lifecycle should have one owner.

### Why is navigation only one possible interpretation of input?

Because input events may also represent contextual interaction or system-level operations.

### Why can a system-level input bypass screen navigation?

Because not every input event belongs to the visible screen.

### Why is there a data source boundary?

Because the application should not care where observations originate.

These are not arbitrary layers.

They are answers to architectural pressures.

---

# The Guiding Principle

Telemetry is designed around a simple direction:

```text
Physical World
      │
      ▼
Capabilities
      │
      ▼
Semantic Information
      │
      ▼
Application
      │
      ▼
Human Understanding
```

The framework exists to make those transformations explicit.

The architecture is not there to make the project look sophisticated.

It is there to make the project understandable.

> **Good architecture should feel inevitable.**
