# Learning — From Sensor to Screen

This is the conceptual path from a physical observation to human understanding.

```text
Physical world
      ↓
Data source
      ↓
Observation identity
      ↓
Runtime handle
      ↓
Repository
      ↓
Screen composition
      ↓
Presentation
      ↓
Human understanding
```

## 1. The Physical World

Something measurable exists: temperature, power, humidity, battery state or another observation.

The physical phenomenon is not yet a software object.

## 2. The Data Source

A source mechanism obtains a value.

It might be:

- MQTT;
- a local sensor;
- an API;
- another transport supported by the platform.

The source answers:

> “How can I obtain this information?”

## 3. Observation Identity

The application needs a stable answer to:

> “What information am I interested in?”

That is the job of `ObservationKey`.

```text
sensor.alphaess_instantaneous_battery_soc
```

is semantic identity.

## 4. Runtime Identity

The runtime should not expose repository storage positions as application meaning.

Telemetry therefore resolves semantic identity to an opaque `ObservationHandle`.

```text
ObservationKey
      ↓
ObservationHandle
```

## 5. Repository

The repository owns runtime storage.

The application should not need to know which array slot contains the value.

## 6. Composition

The Screen references an observation by composition alias.

```text
YAML alias
    ↓
ObservationDefinition
    ↓
ObservationHandle
    ↓
SensorRepository
```

## 7. Presentation

The Screen turns the observation into something a human can understand.

Presentation may include:

- label;
- unit;
- scaling;
- precision;
- colour;
- directional indicators;
- layout.

## 8. Human Understanding

This is the point of the chain.

The value is not useful merely because it reached RAM.

It is useful because somebody can understand what it means.

## The Architectural Lesson

Each stage answers a different question:

```text
Source       → How did we obtain it?
Identity     → What do we mean?
Runtime      → How do we refer to it?
Repository   → Where is it stored?
Composition  → Where is it presented?
Presentation → How is it understood?
```

When one component answers several of these questions at once, architectural pressure usually follows.
