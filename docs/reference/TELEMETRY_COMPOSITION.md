# Telemetry Composition Reference

`telemetry.yaml` is the declarative application composition file for Telemetry.

It describes the observations the application knows about and the Screens that compose those observations.

The build-time composer translates this information into generated C++ under `.pio/build/<environment>/generated/telemetry/`.

The generated files are build artifacts. They are not edited by hand and are not the source of truth.

---

# Composition Model

The current model has two top-level sections:

```yaml
observations:
  ...

screens:
  ...
```

The intended authoring workflow is:

```text
Declare observations
        ↓
Compose screens
        ↓
Build
```

---

# Observation Declaration

Every observation has a YAML alias:

```yaml
current_power_production:
```

The alias is a composition handle. Screens use it when they reference the observation.

An observation also has semantic identity:

```yaml
key: sensor.envoy_current_power_production
```

The key describes what the observation means. It is not a repository slot or a numeric ID.

A complete observation currently looks like:

```yaml
current_power_production:
  key: sensor.example_current_power_production
  type: power
  unit: W
  label: Production
  display:
    scales:
      - threshold: 0
        unit: W
        divisor: 1
        precision: 1
      - threshold: 1000
        unit: kW
        divisor: 1000
        precision: 1
  source:
    type: mqtt
    topic: ha/example/current_power_production
```

---

# Observation Fields

## `key`

Stable semantic identity.

Good examples:

```text
sensor.room_temperature
sensor.envoy_current_power_production
room.temperature
```

The key should describe the information, not how it is stored.

Do not use numeric repository IDs.

---

## `type`

An application-defined semantic type string.

Examples:

```text
power
energy
temperature
humidity
pressure
```

The composition boundary treats this as metadata.

The composer should not turn it into a framework-wide domain enum such as `ENERGY_W` or `TEMP`.

---

## `unit`

The canonical runtime unit for the observation.

For example:

```yaml
unit: W
```

or:

```yaml
unit: Wh
```

Display scaling may present the same observation in another unit, but the canonical observation unit remains the declared `unit`.

---

## `label`

Human-readable presentation label.

Example:

```yaml
label: Production
```

This is optional. When omitted, the composer derives a human-readable label from the alias.

---

# Display Scaling

Display scaling is optional presentation metadata.

Example:

```yaml
display:
  scales:
    - threshold: 0
      unit: Wh
      divisor: 1
      precision: 0

    - threshold: 1000
      unit: kWh
      divisor: 1000
      precision: 1

    - threshold: 1000000
      unit: MWh
      divisor: 1000000
      precision: 1
```

The runtime applies the selected scale generically.

The composer does not need to understand that these are energy units. It translates the declared metadata.

## Scale rules

The current composer validates that:

- `threshold` is numeric and non-negative;
- `divisor` is numeric and positive;
- `precision` is a non-negative integer;
- `unit` is a non-empty string;
- thresholds are strictly increasing;
- the first scale has a threshold of `0`.

Scales are evaluated in order and the last scale whose threshold is met is selected.

---

# Source Declaration

Source information is deliberately co-located with the observation.

Example:

```yaml
source:
  type: mqtt
  topic: ha/example/current_power_production
```

This avoids a second application-level source mapping table that must be kept synchronized with observation declarations.

The source describes a mechanism.

For example:

```text
MQTT
API
I²C
Modbus
```

A vendor or product is not itself the architectural source category.

---

# Current Source Support

The current composer supports:

```yaml
source:
  type: mqtt
```

with a required:

```yaml
source:
  topic: ...
```

Additional source mechanisms may be added later through the architecture and composer rather than by creating parallel application-definition systems.

---

# Screen Declaration

A Screen is a composition of observation aliases.

Example:

```yaml
screens:

  energy_status:
    title: Energy Status

    layout:
      rows:

        - title: Current
          items:
            - current_power_production
            - current_power_consumption

        - title: Today
          items:
            - energy_production_today
            - energy_consumption_today

        - title: Lifetime
          items:
            - energy_production_lifetime
            - energy_consumption_lifetime
```

The composer checks that every item names an observation alias declared under `observations`.

---

# Screen Fields

## Screen ID

The mapping key:

```yaml
energy_status:
```

is the stable composition identifier for the Screen.

## `title`

Human-readable Screen title.

## `layout.rows`

An ordered list of rows.

## Row `title`

The row heading.

## Row `items`

An ordered list of observation aliases.

The order is significant because it defines the composition order within that row.

---

# What a Screen Does Not Declare

A Screen definition should not contain:

```text
ObservationKey values
repository indices
numeric sensor IDs
MQTT topics
SensorType values
DisplayManager calls
```

Those concerns belong to other architectural layers.

The Screen composition describes which observations are presented together and how they are arranged.

---

# Runtime Relationship

The generated observation model bridges the declarative composition and the runtime framework:

```text
telemetry.yaml
      │
      ▼
Telemetry Composer
      │
      ▼
ObservationDefinition
      │
      ├── semantic key
      ├── source metadata
      ├── presentation metadata
      └── runtime handle
      │
      ▼
ObservationHandle
      │
      ▼
SensorRepository
```

For YAML-defined MQTT observations, runtime source lookup can use the generated source metadata:

```text
MQTT topic
      │
      ▼
findObservationBySource()
      │
      ▼
ObservationDefinition
      │
      ▼
ObservationHandle
      │
      ▼
SensorRepository
```

---

# Adding a New Screen

Suppose a new application needs a room status Screen.

## Step 1 — Declare its observations

```yaml
observations:

  room_temperature:
    key: sensor.room_temperature
    type: temperature
    unit: "°C"
    label: Temperature
    source:
      type: mqtt
      topic: ha/room/temperature

  room_humidity:
    key: sensor.room_humidity
    type: humidity
    unit: "%"
    label: Humidity
    source:
      type: mqtt
      topic: ha/room/humidity
```

## Step 2 — Compose the Screen

```yaml
screens:

  room_status:
    title: Room Status

    layout:
      rows:
        - title: Room
          items:
            - room_temperature
            - room_humidity
```

## Step 3 — Build

The composer validates the definitions and generates the runtime composition.

Do not manually create repository slots, MQTT mapping tables, or generated C++ for the new observations.

---

# Single Source of Truth

The same observation may appear on multiple Screens:

```yaml
screens:

  room_status:
    layout:
      rows:
        - title: Room
          items:
            - room_temperature

  overview:
    layout:
      rows:
        - title: Environment
          items:
            - room_temperature
```

Only one observation definition exists.

The Screens reference it.

That is the important difference between composition and duplication.

---

# Validation Philosophy

The composer should reject an invalid application model during the build rather than allow inconsistent runtime state.

Examples include:

```text
missing semantic key
missing canonical unit
invalid display scale
unknown source type
duplicate semantic key
duplicate MQTT topic
unknown Screen observation alias
```

The compiler then becomes the second line of defence after composition validation.

---

# Current Migration Boundary

The composition model is established, but not every existing application path has been migrated to it yet.

The current repository still contains transitional Weather registration and specialised Screen implementation.

The target architecture is:

```text
telemetry.yaml
      ↓
generated composition
      ↓
generic runtime Screen composition
      ↓
SensorRepository
      ↓
presentation
```

Some existing Screens currently sit between those stages while the migration is completed.

That is expected during the transition and should not be mistaken for a second architecture.

---

# Generated Artifacts

Generated files are located beneath:

```text
.pio/build/<environment>/generated/telemetry/
```

They should not be committed as source files and should not be edited by hand.

The relationship is:

```text
telemetry.yaml
      │
      ▼
composer
      │
      ▼
.pio/build/.../generated/telemetry/
      │
      ▼
compiler
```

If generated output is wrong, fix the composer or the declarative source, not the generated file.

---

# Authoring Rules

1. **Describe knowledge once.**

   Put observation identity and source information in the observation declaration.

2. **Use aliases for composition.**

   Screens reference YAML aliases; aliases are not storage IDs.

3. **Keep semantic identity stable.**

   A key describes what the application knows about.

4. **Keep source information local to the observation.**

5. **Keep presentation policy generic.**

6. **Do not reintroduce domain enums into runtime presentation models.**

7. **Do not hand-edit generated output.**

8. **When a new requirement cannot be expressed, examine the architecture before adding a special case.**

---

# Architectural Rule

The composition model exists so that the application can answer:

> **What do I want this target to know and present?**

without having to describe the framework's implementation details.

The framework answers:

> **How do I make that happen?**

The composer is the bridge between those two questions.
