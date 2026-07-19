# Architecture

Telemetry is a small embedded application built on top of a reusable UI and hardware abstraction framework.

The architecture exists to answer one question:

> **What should change when the hardware, application, or interaction method changes?**

The answer should be:

> **Only the layer responsible for that change.**

That is the central architectural goal of Telemetry.

---

## The Architecture at a Glance

```text
┌──────────────────────────────────────────────────────┐
│                    Application                       │
│                                                      │
│  Weather presentation                                │
│  Control Panel                                       │
│  Information pages                                   │
└─────────────────────────┬────────────────────────────┘
                          │
                          ▼
┌──────────────────────────────────────────────────────┐
│                    UI Framework                      │
│                                                      │
│  ScreenManager                                       │
│  Screen                                              │
│  ControlPanelScreen                                  │
│  ControlPage                                         │
│  InputEvent interpretation                           │
└─────────────────────────┬────────────────────────────┘
                          │
                          ▼
┌──────────────────────────────────────────────────────┐
│                    Capabilities                      │
│                                                      │
│  Display · Input · Touch · Connectivity · Data · OTA │
└─────────────────────────┬────────────────────────────┘
                          │
                          ▼
┌──────────────────────────────────────────────────────┐
│                      Hardware                        │
│                                                      │
│  ESP8266 / ESP32 · Displays · Touch · Input devices  │
└──────────────────────────────────────────────────────┘
```

The important characteristic is not the number of layers. It is the direction of dependency.

> **Higher-level policy should not need to know which piece of hardware happens to provide a capability.**

---

# 1. From Weather Station to Framework

Telemetry began as a weather station. A display could read sensors, connect to Wi-Fi, receive data, and draw the result.

The architectural pressure appeared when we started asking:

- What if the display changes?
- What if the input method changes?
- What if the application grows?
- What if another screen is added?
- What if the weather application is only one application using the system?
- What if a future device has no touch screen at all?

A collection of hardware-specific decisions can produce a working device. It does not necessarily produce a reusable system.

The architecture therefore distinguishes between the framework and the application:

```text
Telemetry
    │
    ├── Framework
    │   ├── Display capability
    │   ├── Input capability
    │   ├── Screen framework
    │   ├── Data capability
    │   └── System services
    │
    └── Application
        └── Weather presentation
```

> **The framework should remain useful even if the weather application disappears.**

See `docs/architecture/001-from-weather-station-to-framework.md`.

---

# 2. The Display Boundary

The display driver knows how to talk to a particular piece of hardware. The rest of the application should not.

```text
Screen
    │
    ▼
DisplayManager
    │
    ▼
MiniGrafx
    │
    ▼
ILI9341 Driver
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

It should not need to know about `ILI9341_SPI`, `MiniGrafx`, SPI pins, or framebuffer implementation details.

`DisplayManager` is therefore an architectural boundary, not merely a convenience wrapper.

> **Screens consume display capability. They do not consume display hardware.**

See `docs/architecture/002-the-display-boundary.md`.

---

# 3. The Data Boundary

The application should consume data. It should not need to know where that data came from.

A value might arrive from MQTT, a local sensor, a file, a test source, or another transport mechanism.

```text
┌────────────────────┐
│    Data Source     │
│                    │
│ MQTT / Sensor /    │
│ Test / Other       │
└──────────┬─────────┘
           │
           ▼
┌────────────────────┐
│    IDataSource     │
│                    │
│  Stable contract   │
└──────────┬─────────┘
           │
           ▼
┌────────────────────┐
│    Application     │
│                    │
│  Consumes data     │
└────────────────────┘
```

> **Applications consume capabilities, not implementations.**

See `docs/architecture/003-the-data-boundary.md`.

---

# 4. Input Is Semantic

Input is one of the most important architectural boundaries in Telemetry.

Touch, joystick, rotary encoder, keyboard, and five-way switch all produce different physical signals. The application should not need to know which one was used.

```text
┌─────────────────────┐
│   Physical Input    │
│                     │
│ Touch / Encoder /   │
│ Joystick / Switch   │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│    Input Source     │
│                     │
│ Detects physical    │
│ interaction         │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│    InputManager     │
│                     │
│ Normalises events   │
│ Queues events       │
└──────────┬──────────┘
           │
           ▼
      InputEvent
           │
           ▼
┌─────────────────────┐
│    ScreenManager    │
│                     │
│ Owns screen-level   │
│ navigation          │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│   Active Screen     │
│                     │
│ Interprets event    │
│ within its context  │
└──────────┬──────────┘
           │
           ▼
      Active Page
```

The input source does not say `"Open the About page."` It says something like `TAP + position` or `SCROLL_DOWN`.

The active context decides what that means.

```text
TAP
 │
 ├── WeatherScreen
 │       └── Interpret position as a focus area
 │
 ├── ControlMenuPage
 │       └── Select a menu item
 │
 └── AboutPage
         └── Possibly no action
```

> **Input devices produce interaction. Context produces meaning.**

See `docs/architecture/004-input-is-semantic.md`.

---

# 5. Who Owns Navigation?

An important architectural question is:

> **Who decides that the application should leave the current screen and show another screen?**

The answer is not the input device. It is not the active screen's implementation details.

Navigation belongs to the screen-level coordinator.

```text
InputEvent
    │
    ▼
ScreenManager
    │
    ├── Screen-level navigation?
    │       │
    │       ├── Yes → transition to another Screen
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

Not every input event is navigation. An event may mean:

- open another screen
- return to a previous screen
- scroll a menu
- select an item
- change a presentation format
- adjust a value
- do nothing in the current context

Each layer therefore answers only the questions appropriate to its scope.

See `docs/architecture/005-who-owns-navigation.md` and `docs/roadmap/Sprint_Delta.md`.

---

# 6. The Cost of Abstraction

Abstraction is not automatically good.

Every abstraction introduces another concept, another name, another layer of indirection, and another thing a future reader must understand.

Therefore, every abstraction must earn its place.

> **Does this abstraction reduce complexity overall?**

`DisplayManager` is justified because it prevents every screen from knowing about display-driver details.

`InputEvent` is justified because it prevents every screen from knowing about touch, joystick, encoder, or button hardware.

An abstraction that merely renames an existing concept without reducing coupling is probably unnecessary.

> **We abstract where a boundary protects the rest of the system from a meaningful change.**

See `docs/architecture/006-the-cost-of-abstraction.md`.

---

# 7. Dependency Direction

The architecture follows a general dependency rule:

```text
High-level policy
        │
        ▼
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

Dependencies should flow downward.

A weather screen may depend on a data contract. It should not depend directly on an MQTT implementation.

A screen may depend on `DisplayManager`. It should not depend directly on `ILI9341_SPI`.

A screen may consume `InputEvent`. It should not depend directly on touch coordinates or encoder pulses.

This keeps upper layers stable while lower layers evolve.

---

# 8. The Repository Reflects the Architecture

The directory tree is part of the architecture.

The repository should communicate the system before a source file is opened.

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

The structure communicates that display, input, hardware, screens, system orchestration, data, and transport are separate concerns.

The repository is therefore not merely a filing system.

> **Good architecture should be visible before the code is understood.**

---

# 9. The Architecture Is Deliberately Incomplete

Telemetry is not trying to solve every problem.

It deliberately stops at the information boundary.

```text
Observations
     ▲
     │
   Data
     ▲
     │
Information
     ▲
     │
Intelligence
     ▲
     │
Knowledge
     ▲
     │
Wisdom
```

Telemetry moves observations toward information. It does not attempt to become an analytics platform, automation engine, artificial intelligence system, or general-purpose operating system.

That limitation is intentional.

> **A system becomes easier to understand when it knows what it is not responsible for.**

---

# 10. Architectural History

The architecture did not appear fully formed. It emerged from pressure.

The project repeatedly encountered questions such as:

- What happens if the display changes?
- What happens if the data source changes?
- What happens if the input device changes?
- Who owns navigation?
- Where should context be interpreted?
- Which abstractions are worth their cost?

Each question exposed a boundary.

The architecture was then shaped around that boundary.

```text
Pressure
   │
   ▼
Question
   │
   ▼
Architectural decision
   │
   ▼
Boundary
   │
   ▼
Simpler change
```

That is the reason the architecture documentation exists.

It is not merely a description of the final code.

It is a record of why the code became this way.

---

# The Short Version

Telemetry is built around a small number of ideas:

1. **Architecture comes before implementation.**
2. **The repository should reflect the architecture.**
3. **Capabilities matter more than hardware.**
4. **Applications consume contracts, not implementations.**
5. **Input is semantic.**
6. **Context interprets interaction.**
7. **ScreenManager owns screen-level navigation.**
8. **Every abstraction must earn its cost.**
9. **Dependencies flow downward.**
10. **The framework comes before the application.**

The goal is not to create the most sophisticated architecture.

The goal is to create an architecture that makes the next decision easier.

And, ideally, makes the correct decision feel inevitable.

> **"Ahh… now I understand why."**
