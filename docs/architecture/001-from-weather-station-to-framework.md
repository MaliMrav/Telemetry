# 001 — From Weather Station to Framework

## Why did a weather station need to become a framework?

Telemetry began as a weather station.

That was a perfectly reasonable place to start.

A display. Some sensors. A weather service. A small embedded device that could observe the environment and present useful information.

At first, the application was the architecture.

For a small prototype, that is often enough.

But the project began to grow.

The display became more capable. The data sources became more varied. The input system became more complex. Different hardware became possible. New screens appeared.

The project was no longer simply implementing a weather station.

It was beginning to contain reusable capabilities.

That created an architectural pressure:

> If the project is really a framework, why does the architecture still look like a weather application?

## The application was becoming larger than its name

The original application identity created a natural temptation to organise the system around the weather use case.

But the same display boundary could support:

- weather
- system status
- diagnostics
- configuration
- calibration

The same input boundary could support:

- touch
- buttons
- encoders
- joysticks

The same data boundary could support:

- MQTT
- sensors
- APIs
- simulated data

The architecture therefore needed to change.

## The framework emerged from the boundaries

Telemetry began to separate responsibilities:

```text
Framework
    │
    ├── Display capability
    ├── Input capability
    ├── Data capability
    ├── Connectivity capability
    ├── Storage capability
    └── Diagnostics capability
            │
            ▼
       Application
```

The framework provides the structure.

Applications provide the meaning.

This distinction is central to the project.

## Why not simply keep building the weather station?

Because the next feature would otherwise become a question like:

> Where do I put this weather-specific code?

The better question is:

> What capability does this belong to?

That question scales better.

It also teaches more.

The project becomes a place where architectural decisions can be observed.

The weather station remains an application.

But the architecture underneath it becomes reusable.

## The lesson

The project did not become a framework because someone declared it to be one.

It became a framework because the original application identity was no longer sufficient to explain the architecture.

A system often tells you what it wants to become.

The architect's job is to notice.
