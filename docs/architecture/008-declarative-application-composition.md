# Declarative Application Composition

_Architecture 008_

## The Pressure

Telemetry began as a firmware project in which adding a new screen meant adding code.

A screen was not simply a presentation. It pulled together several pieces of implementation:

- observations
- observation registration
- source mappings
- runtime storage
- display formatting
- layout information
- navigation wiring
- screen construction

That approach worked while Telemetry had a small number of screens and a small number of observations.

It became increasingly expensive as the framework grew.

The architectural pressure was no longer just:

> **How do we add another screen?**

It became:

> **Why should creating an application screen require editing several unrelated C++ files when the application could describe what it wants in one place?**

That question exposed a deeper coupling.

The application definition was distributed across implementation code.

The framework had learned how to represent observations, repositories, sources and Screens independently, but the application still had to assemble those pieces manually.

The architecture therefore needed another boundary:

```text
Framework capabilities
        │
        ▼
Application composition
        │
        ▼
Target application
```

The new composition boundary is expressed declaratively.

---

# The Decision

Telemetry introduces a build-time application composition model in which `telemetry.yaml` becomes the single declarative source for application observations and Screen composition.

The build-time composer reads that definition and generates the runtime structures required by the firmware.

The important distinction is:

> **`telemetry.yaml` describes the application. The composer translates that description into runtime structures.**

The composer is not the architecture.

It is the translator between the declarative application model and the framework's runtime contracts.

---

# The New Model

The application definition now has two primary sections:

```text
telemetry.yaml
    │
    ├── observations
    │       ├── identity
    │       ├── meaning
    │       ├── canonical unit
    │       ├── presentation policy
    │       └── source
    │
    └── screens
            └── layout references
```

Observations declare what information exists and where it comes from.

Screens declare how those observations are composed into a presentation.

The framework then provides the runtime machinery.

---

# Observations Are the Application Vocabulary

An observation has a semantic identity:

```yaml
current_power_production:
  key: sensor.envoy_current_power_production
  type: power
  unit: W
  ...
```

The YAML alias:

```text
current_power_production
```

is the composition handle.

The semantic key:

```text
sensor.envoy_current_power_production
```

is the observation identity.

These remain distinct.

```text
YAML alias
    │
    │ composition handle
    ▼
ObservationDefinition
    │
    │ semantic identity
    ▼
ObservationKey
    │
    │ runtime identity
    ▼
ObservationHandle
    │
    ▼
SensorRepository
```

This preserves the separation established by the data architecture while giving the application a simple authoring vocabulary.

---

# Sources Belong to Observations

The source mechanism is declared alongside the observation that uses it:

```yaml
current_power_production:
  key: sensor.envoy_current_power_production
  type: power
  unit: W
  source:
    type: mqtt
    topic: ha/envoy/current_power_production
```

This is intentional.

There is no separate application-wide `sources:` section which must be kept synchronized with observation declarations.

The observation answers the question:

> **What information does the application need, and through which source mechanism is it currently obtained?**

The source remains a mechanism, not a product identity.

MQTT is a source mechanism.

Envoy is a product.

Home Assistant is an external system.

The composition model does not make the product the architecture.

---

# Presentation Policy Is Data, Not Domain Logic

The composer also carries presentation policy declared by the application:

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
```

The important architectural change is that the composer does not decide what an energy value means.

It simply translates the declaration.

Therefore the generated runtime model can carry generic presentation metadata without reintroducing domain-specific `SensorType` values.

The runtime `SensorTile` now contains:

```text
label
unit
value
min/max
trend
valid
presentation scales
```

but it does not contain:

```text
ENERGY_W
ENERGY_WH
TEMP
HUMIDITY
PRESSURE
```

That is a deliberate boundary.

---

# SensorTile Became Domain-Neutral

This milestone also completes an important part of the previous data-layer refactor.

The old model allowed a domain type enum to leak into the runtime presentation model.

That created a hidden dependency:

```text
Domain meaning
      │
      ▼
SensorType
      │
      ▼
SensorTile
      │
      ▼
Screen formatting
```

The new model is:

```text
Observation definition
      │
      ├── semantic meaning
      ├── canonical unit
      └── presentation policy

SensorTile
      │
      └── runtime presentation state
```

The runtime tile no longer needs to know which domain created it.

That allows generic presentation code to consume observations without carrying a hidden domain switch statement.

---

# Screens Become Composition

A Screen is now describable as a composition of observation aliases:

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

The screen definition does not contain:

- semantic ObservationKeys
- MQTT topics
- repository positions
- storage indices
- `SensorType` values
- display driver calls

It describes composition.

That is the essential shift.

---

# The Composer's Responsibility

The build-time composer is deliberately narrow.

It should:

1. read `telemetry.yaml`;
2. validate the declarative model;
3. normalise the definitions;
4. generate C++ structures describing those definitions;
5. generate the registration code required to bind them to the runtime framework;
6. fail the build when the declarative model is invalid.

It should not:

- know about Envoy;
- know about Weather;
- know about Solar;
- choose `SensorType` values;
- invent application-specific screen names;
- embed business logic;
- become a second source of application truth.

The rule is:

> **The composer translates the application model. It does not own the application model.**

---

# Build-Time, Not Runtime

The composer runs during the PlatformIO build.

It produces generated sources under:

```text
.pio/build/<environment>/generated/telemetry/
```

Those files are build artifacts.

They are not source-of-truth files.

The source of truth is:

```text
telemetry.yaml
```

This means the architecture remains deterministic:

```text
telemetry.yaml
      │
      ▼
composer
      │
      ▼
generated C++
      │
      ▼
compiler
      │
      ▼
firmware
```

`.pio/` remains disposable and ignored by Git.

---

# Generated Runtime Contract

The generated composition describes observations using a generic definition:

```text
ObservationDefinition
├── alias
├── key
├── type
├── unit
├── label
├── sourceType
├── sourceTopic
├── handle
├── displayScales
└── displayScaleCount
```

Screens are represented separately:

```text
ScreenDefinition
├── id
├── title
├── rows
├── rowCount
└── columns
```

The generated layer therefore becomes a bridge between the application description and the framework runtime.

---

# Dynamic Source Resolution

Because the generated observation definition carries source metadata, runtime source implementations can resolve an observation by source rather than maintaining a separate hand-written mapping table.

For MQTT this takes the form of the architectural relationship:

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

This removes a second source of truth for YAML-defined MQTT observations.

The source is still a runtime mechanism.

The application identity remains the observation key.

---

# One Source File, Many Generated Elements

The key architectural idea is not the YAML syntax itself.

It is the reduction of application definition sprawl.

A single observation declaration can provide the information required by multiple runtime elements:

```text
                        telemetry.yaml
                             │
                 ┌───────────┴───────────┐
                 ▼                       ▼
        Observation definition       Screen reference
                 │                       │
          ┌──────┼──────┐               │
          ▼      ▼      ▼               ▼
        identity source presentation   layout
          │      │      │               │
          └──────┼──────┴───────┬───────┘
                 ▼              ▼
          generated runtime   generated
          observation model   Screen model
```

The application author does not need to duplicate the same knowledge in multiple implementation files.

---

# What This Does Not Mean

This milestone does not mean that every existing Screen has already become a fully generic renderer.

The current migration still contains transitional code.

For example, the existing Energy Status implementation still contains knowledge of its six-tile arrangement, and Weather remains partly manually registered.

Those are migration steps, not arguments against the composition model.

The architectural direction is:

```text
Current
    YAML composition
          │
          ▼
    generated definitions
          │
          ▼
    transitional Screen consumers

Target
    YAML composition
          │
          ▼
    generated definitions
          │
          ▼
    generic runtime Screen composition
```

The remaining implementation work is to finish that migration rather than recreate application knowledge in C++.

---

# Provisioning a New Screen

The intended authoring experience is now:

### 1. Declare observations

Give each observation:

- an alias
- a semantic key
- a meaning/type
- a canonical unit
- optional display policy
- a source

### 2. Compose the Screen

Reference observation aliases in the Screen layout.

### 3. Build

The composer validates the definition and generates the runtime structures.

The author should not have to:

- assign a repository slot;
- assign a numeric sensor ID;
- add a second MQTT mapping table;
- add `SensorType` values;
- duplicate observation metadata in a Screen class;
- hand-edit generated C++.

That is the experience this architecture is designed to make possible.

---

# Failure as an Architectural Signal

The migration itself exposed several failures:

- generated code still referenced `SensorType` after the type had been removed;
- the composer temporarily generated malformed C++ by mixing definition data and registration statements;
- Screen consumers still expected the old named-handle API;
- legacy Weather registrations still depended on removed domain enums.

These failures were useful.

They demonstrated that the compiler was enforcing the new boundary.

The architecture is healthier when obsolete assumptions fail loudly instead of remaining silently compatible.

---

# Relationship to Previous Architecture

This milestone builds directly on earlier decisions.

```text
Display Boundary
       │
       ▼
Data Boundary
       │
       ▼
Semantic Observation Identity
       │
       ▼
Opaque Runtime Handles
       │
       ▼
Domain-Neutral SensorTile
       │
       ▼
Declarative Application Composition
```

Each step removes a different kind of accidental coupling.

The composition model is therefore not a replacement for the earlier architecture.

It is the next layer built on top of it.

---

# Architectural Ownership

The ownership model is now clearer:

```text
telemetry.yaml
    owns application composition

Telemetry Composer
    owns translation and validation

ObservationRegistry
    owns runtime observation identity

SensorRepository
    owns runtime storage

Data sources
    own acquisition mechanisms

Screens
    own presentation behaviour

ScreenManager
    owns Screen lifecycle and navigation
```

No single component owns the entire application.

Composition connects the pieces without collapsing their boundaries.

---

# The Guiding Principle

The original question was:

> "Why should adding a screen require editing several pieces of unrelated implementation code?"

The architectural answer is:

> **It shouldn't, when the information required to define that screen is application data rather than framework behaviour.**

Therefore:

```text
Application knowledge
        ↓
Declarative composition
        ↓
Build-time translation
        ↓
Framework runtime
```

The framework provides the machinery.

The application describes what it wants.

The composer connects the two.

> **Architecture precedes implementation. The implementation should make the architecture inevitable.**
