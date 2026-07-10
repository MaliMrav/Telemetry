# Vision

> "The real product is helping someone say:
>
> **'Ahh... now I understand why.'**"

Telemetry is not a weather station.

It is not an MQTT client.

It is not a dashboard.

It is not another collection of Arduino examples.

Telemetry is an embedded observability framework.

Its purpose is to help developers build firmware that observes the physical world, presents information clearly, and remains understandable as systems grow in complexity.

The framework exists because embedded software often becomes tightly coupled to specific hardware, difficult to extend, and difficult to reason about.

Telemetry takes a different approach.

It treats firmware as architecture.

---

# What Telemetry Is

Telemetry provides reusable building blocks for embedded systems.

It separates hardware from behaviour.

It separates data acquisition from presentation.

It separates user interaction from physical input devices.

Its goal is to let developers think about capabilities instead of components.

Instead of asking:

> "How do I read this sensor?"

Telemetry encourages asking:

> "What capability am I trying to provide?"

---

# The Philosophy

Telemetry is built around a simple belief.

Good architecture makes complexity disappear.

Every abstraction should make the system easier to understand.

If an abstraction creates more confusion than clarity, it should not exist.

Every module should have a single responsibility.

Every dependency should point in one direction.

Every layer should know only what it needs to know.

---

# What Telemetry Does

Telemetry focuses on six capabilities.

## Input

Observing the physical world.

Examples include:

- touch
- buttons
- rotary encoders
- sensors
- serial interfaces

Input becomes semantic actions rather than hardware events.

---

## Output

Presenting information.

Displays should render.

They should not calculate.

Rendering is separate from decision making.

---

## Storage

Persisting state.

Examples include:

- calibration
- configuration
- preferences
- cached observations

Storage exists so behaviour survives reboot.

---

## Connectivity

Communicating with other systems.

Examples include:

- Wi-Fi
- Ethernet
- MQTT
- BLE
- USB
- Serial

Connectivity transports information.

It does not interpret it.

---

## Data Sources

Obtaining observations.

Examples include:

- MQTT
- NTP
- GPS
- OneWire
- Modbus
- OBD-II
- CAN
- REST
- local sensors

Time itself is considered a data source.

Each source contributes observations to the system.

---

## Diagnostics

Making firmware observable.

Examples include:

- health
- uptime
- heap
- watchdog
- CPU load
- connectivity
- boot timing

A framework should always be able to explain what it is doing.

---

# What Telemetry Does Not Do

Telemetry intentionally stops before higher-order reasoning.

It does not attempt to become:

- Home Assistant
- Node-RED
- an automation engine
- an AI platform
- an analytics platform

Those systems already exist.

Telemetry provides reliable observations for them.

They provide intelligence.

---

# The Knowledge Pyramid

Telemetry occupies the lower layers of the knowledge pyramid.
