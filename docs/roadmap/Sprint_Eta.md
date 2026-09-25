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

# Phase 7 — Migrate Screen Consumers to Generic Composition

**Status: In Progress**

Energy Status currently consumes generated observation definitions, but its presentation code still contains the specific six-tile layout.

The target is:

```text
telemetry.yaml
      ↓
ScreenDefinition
      ↓
generic runtime Screen composition
      ↓
SensorRepository
      ↓
DisplayManager
```

The Screen should not have to know which aliases happen to exist in the application.

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
