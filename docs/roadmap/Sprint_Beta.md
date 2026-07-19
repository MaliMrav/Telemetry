# Sprint Beta — Establish the Application Boundaries

## Objective

Establish the architectural boundaries that allow Telemetry to evolve from a weather-station application into a reusable embedded UI framework.

## Architectural Pressure

The original project was a working weather station. It was tempting to organise everything around the immediate application: weather data, a display, touch input, Wi-Fi, and MQTT.

That approach works until the project needs another display, another input device, another data source, or another application.

The architecture therefore had to separate framework responsibilities, application responsibilities, hardware capabilities, and concrete hardware implementations.

## The Boundary

Telemetry is the framework.

Weather is an application built upon that framework.

The framework provides capabilities such as display, input, data access, connectivity, storage, diagnostics, and screen management.

The application composes those capabilities into a useful experience.

## The Result

The repository began to reflect the architecture:

```text
hardware/
display/
input/
data/
models/
mqtt/
screens/
system/
```

The important change was not merely moving files. The important change was making responsibility visible.

## Breadcrumb

This sprint established the foundation for the later architectural decisions:

- the display boundary
- the data boundary
- semantic input
- navigation ownership

## Principles Exercised

- Architecture Before Implementation
- The Repository Should Reflect the Architecture
- Capabilities Before Hardware
- One Responsibility Per Capability
- Dependencies Flow Downward
- Framework Before Application

## Outcome

**Complete.**

The project had begun its transition from a weather station into a framework capable of supporting applications.
