# Sprint Eta — Declarative Application Composition

## Objective

> **Establish a single declarative application definition from which Telemetry can validate and generate the runtime structures required to provision observations and Screens, while preserving the existing framework boundaries.**

Sprint Zeta separated observation identity, runtime handles, repository storage and source mechanisms.

That solved an important data-layer problem.

It also exposed the next pressure.

The framework now had reusable pieces, but the application still assembled them manually across multiple implementation files.

Adding a Screen could require changes to:

- observation declarations
- source mappings
- repository registration
- Screen code
- display formatting
- navigation
- application construction

The next architectural question was therefore:

> **Can the application describe its knowledge once and let the build system provision the framework structures it needs?**

Sprint Eta establishes the declarative composition model that answers that question.

---

# Architectural Decisions Frozen by This Sprint

Sprint Eta establishes these principles:

> **`telemetry.yaml` is the source of truth for application composition.**

> **Observation identity and Screen composition are declared, not encoded as distributed implementation knowledge.**

> **Observation sources belong to the observations they provide.**

> **Presentation policy is declarative metadata, not a domain enum embedded in `SensorTile`.**

> **The composer validates and translates application definitions; it does not own application semantics.**

> **Generated sources are build artifacts under `.pio/build/` and are never the source of truth.**

> **Runtime identity remains `ObservationKey → ObservationHandle → SensorRepository`.**

> **Screens consume generated composition; they do not own repository storage or source mapping.**

> **The framework remains independent of application domains such as Weather or Solar.**

---

# Phase 1 — Establish the Declarative Observation Model

**Status: Complete**

`telemetry.yaml` now declares observations with:

- composition alias
- semantic key
- opaque type string
- canonical unit
- label
- optional display scales
- source mechanism
- source details

Example:

```yaml
current_power_production:
  key: sensor.envoy_current_power_production
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
    topic: ha/envoy/current_power_production
```

No numeric repository ID is required.

---

# Phase 2 — Make the Composer Domain-Neutral

**Status: Complete**

The composer must not contain knowledge such as:

```text
Energy
Solar
Weather
Envoy
Home Assistant
```

It validates structure and translates declared metadata.

The application supplies the meaning.

This is the difference between:

```text
composer as architecture
```

and:

```text
composer as translator
```

Only the second is acceptable.

---

# Phase 3 — Generate Generic Observation Definitions

**Status: Complete**

The composer generates a generic `ObservationDefinition` table containing:

```text
alias
key
type
unit
label
sourceType
sourceTopic
handle
displayScales
displayScaleCount
```

This creates a stable bridge between YAML and runtime.

---

# Phase 4 — Make `SensorTile` Domain-Neutral

**Status: Complete**

The previous `SensorType` dependency has been removed from `SensorTile`.

The runtime tile now carries generic presentation state and generic display-scale metadata.

Domain meaning remains in observation definitions rather than becoming a runtime enum.

---

# Phase 5 — Generate Screen Definitions

**Status: Complete**

`telemetry.yaml` now supports:

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
```

The composer validates that every Screen item refers to a declared observation alias.

This is the first important step toward Screen provisioning from a single declarative source.

---

# Phase 6 — Remove Duplicate Generated Source Mappings

**Status: Complete**

YAML-defined MQTT observations are resolved through the generated observation definitions.

The architecture no longer requires a second generated MQTT mapping table for those observations.

The desired flow is:

```text
MQTT topic
    ↓
Generated composition lookup
    ↓
ObservationHandle
    ↓
SensorRepository
```

Legacy Weather mappings remain as transitional code until Weather is migrated into the composition model.

---

# Phase 7 — Generic Screen Composition

**Status: In Progress**

## The Pressure

Energy Status currently consumes the generated ScreenDefinition,
but the Screen still interprets that definition through the
implementation concept of rows, columns and six fixed quadrants.

That means the proving slice has demonstrated declarative
observation membership without yet achieving declarative
presentation composition.

The next pressure is therefore:

> Can a Screen become a generic canvas onto which the application
> composes presentation components, without the Screen implementation
> knowing which observations or layout happen to exist?

The six-tile Energy Status layout is a proving implementation,
not the architectural destination.

## The Target

The target composition is:

telemetry.yaml
      ↓
ScreenDefinition
      ↓
Canvas
      ↓
Card definitions
      ↓
Observation references
      ↓
Card renderers
      ↓
DisplayManager

The Screen implementation should provide the runtime machinery
required to render a declarative composition.

It should not contain knowledge of:

- which observations exist;
- how many cards exist;
- which observations belong together;
- whether the layout uses two columns, three columns, or another grid;
- whether an observation is presented as a value, gauge, battery,
  entity list, graph, or another supported card type.

## Cards

A Card is a generic presentation component.

For example:

```yaml
- type: battery
  observation: battery_charge_percentage
  grid:
    column: 1
    row: 1
    columns: 4
    rows: 3
```
or:

```yaml
- type: gauge
  observation: current_power_production
  grid:
    column: 5
    row: 1
    columns: 4
    rows: 3
```
The observation supplies the knowledge.

The Card supplies the presentation.

The grid supplies the geometry.

## Canvas

A Screen contains a declarative Canvas.

The Canvas defines a logical grid rather than physical pixels.

For example:

```yaml
canvas:
  columns: 9
  rows: 5
```

Cards occupy regions of that grid.

The layout system converts those logical regions into physical display
coordinates.

This keeps application composition independent of display resolution
and display-driver implementation.

## Presentation Types

The initial proving set should remain deliberately small.

Possible initial card types include:
- value
- gauge
- battery
- entitie3s

Additional card types can be introduced when architectural pressure
requires them.

## A Card type is a framework capability.

The application chooses which capability to compose.

Observation and Presentation Remain Separate

An observation must not become a visual component.

The same observation may be rendered by multiple Card types:

```
battery_charge_percentage
        │
        ├── BatteryCard
        ├── GaugeCard
        └── EntitiesCard
```

This preserves the distinction between application knowledge and
presentation.

## Platform Capability

Card capabilities remain subject to platform composition.

A Card may be architecturally valid but unavailable on a constrained
target.

The ESP8266 therefore remains a useful capability boundary rather
than forcing the framework to pretend every presentation is equally
cheap.

## Definition of Done

Phase 7 is complete when:

- [ ]  Energy Status no longer assumes a six-quadrant layout.
- [ ] Screen layout is represented as a logical Canvas.
- [ ] Cards can occupy configurable grid regions.
- [ ] Cards reference observations by composition alias.
- [ ] Observation identity remains independent of presentation.
- [ ] At least two generic Card types can render heterogeneous observations.
- [ ] A Screen can be rearranged through telemetry.yaml without modifying Screen implementation code.
- [ ] The Screen renderer contains no application-specific observation aliases.
- [ ] Layout is independent of physical display pixels.
- [ ] Platform capabilities can constrain available Card types without hanging application semantics.

---

# Phase 8 — Migrate Weather into Declarative Composition

**Status: Planned**

Weather remains manually registered through domain-specific observation keys.

The target is to describe those observations in the same composition model:

```yaml
observations:
  kitchen_temperature:
    ...

  pergola_temperature:
    ...

  kitchen_humidity:
    ...
```

The Weather Screen can then consume the generated Screen definition rather than maintaining its own observation list.

The final result should allow Weather to become an example of the same composition model as Energy rather than a special architectural case.

---

# Phase 9 — Provision Screens Without Hand-Wiring Application Knowledge

**Status: Planned**

The intended authoring workflow is:

```text
1. Declare observations
        ↓
2. Compose the Screen
        ↓
3. Build
```

A developer should not need to modify several unrelated runtime files merely because a new application Screen exists.

This does not mean all Screen behaviour becomes declarative.

Presentation behaviour, interaction logic and specialised rendering may remain code.

The composition layer owns the knowledge of:

- which observations exist
- which observations are presented together
- how those observations are arranged
- which generic presentation metadata applies

The Screen implementation owns behaviour that is genuinely behavioural.

---

# Definition of Done

Sprint Eta is complete when:

- [x] `telemetry.yaml` is established as the application composition source of truth.
- [x] Observation declarations contain identity, meaning, unit, presentation metadata and source.
- [x] Observation sources are co-located with observations.
- [x] The composer is domain-neutral.
- [x] Generic `ObservationDefinition` structures are generated.
- [x] Generic Screen definitions are generated.
- [x] `SensorTile` contains no domain-specific `SensorType`.
- [x] YAML-defined MQTT observations resolve through generated composition metadata.
- [ ] Energy Status consumes `ScreenDefinition` generically rather than hardcoding its six observation aliases.
- [ ] Weather observations are migrated into `telemetry.yaml`.
- [ ] Weather consumes generated Screen composition.
- [ ] New Screens can be provisioned without duplicating application-definition knowledge across runtime files.
- [ ] Generated Screen composition is sufficient for generic Screen rendering where specialised behaviour is not required.

---

# Architectural Result

The architecture now has a third major axis alongside identity and platform composition:

```text
                         TELEMETRY
                             │
          ┌──────────────────┼──────────────────┐
          │                  │                  │
          ▼                  ▼                  ▼
       Identity          Composition        Platform
          │                  │                  │
   ObservationKey       telemetry.yaml     PlatformIO env
          │                  │                  │
 ObservationHandle     generated model    ESP8266 / ESP32
          │                  │                  │
      Repository        Screen assembly     capabilities
```

The framework therefore moves toward a clearer division:

```text
Framework
    provides capabilities

Application composition
    declares what this application uses

Platform composition
    selects what this target instantiates
```

---

# The Architectural Lesson

The pressure looked like a Screen-creation problem.

It was actually a composition problem.

The framework had already learned how to separate:

- hardware
- input
- navigation
- data sources
- observation identity
- runtime storage

The next step was to stop forcing the application to reassemble those concepts manually.

The solution is declarative composition:

```text
Application knowledge
        ↓
telemetry.yaml
        ↓
Build-time composer
        ↓
Generated runtime structures
        ↓
Telemetry framework
```

The source file describes knowledge.

The composer translates it.

The runtime executes it.

> **The Publisher is concerned with knowledge, not files.**

For Telemetry, the equivalent rule is:

> **The application declares knowledge once; the build system provisions the implementation structures that express it.**
