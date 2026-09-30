# Sprint Eta — Declarative Application Composition

## Objective

> **Establish a single application definition that is good enough for both machines and humans: a definition from which Telemetry can validate and generate the runtime structures required to compose observations and Screens, while preserving the existing framework boundaries and making platform capability an explicit architectural boundary.**

Sprint Zeta separated observation identity, runtime handles, repository storage and source mechanisms.

That solved an important data-layer problem.

It also exposed the next pressure.

The framework now had reusable pieces, but the application still assembled them manually across multiple implementation files.

Adding or changing a Screen could require changes to:

- observation declarations
- source mappings
- repository registration
- Screen code
- display formatting
- navigation
- application construction
- layout details

The next architectural question was therefore:

> **Can the application describe its knowledge and presentation intent once, and let the build system provision the framework structures required to express it?**

Sprint Eta establishes the declarative composition model that answers that question.

It also establishes an important constraint on that answer:

> **Generic composition must not mean unlimited runtime complexity.**

Telemetry must be able to describe a rich application while still respecting the capabilities of the target platform. The ESP8266 has already demonstrated that there are practical resource boundaries. Those boundaries are part of the architecture, not an implementation inconvenience to be hidden later.

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

> **The framework remains independent of application domains such as Weather, Solar or AlphaESS.**

> **Presentation is a framework capability; composition selects which capabilities an application uses.**

> **Logical layout is independent of physical display pixels.**

> **Platform limits may constrain presentation capabilities without changing application meaning.**

> **The application definition must be suitable for both machine consumption and human authoring.**

> **The Telemetry Application Definition Studio is an authoring interface, not a second source of truth.**

> **The Studio must preserve the relationships among observations, sources, Screens, Canvas composition, Cards and Build configuration rather than exposing them as disconnected records.**

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

The YAML alias is a composition handle.

The semantic key is the application's stable identity for the observation.

This establishes an important separation:

```text
composition handle
        ↓
semantic identity
        ↓
runtime identity
```

The application names what it knows without needing to know how the runtime stores it.

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
AlphaESS
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

The composer therefore knows about the structure of a declaration, the capabilities it can translate, and the validation rules required to produce valid generated code.

It does not decide what the application means.

---

# Phase 3 — Generate Generic Observation Definitions

**Status: Complete**

The composer generates a generic `ObservationDefinition` table containing the information required to bridge application composition to runtime structures:

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

The important architectural relationship is:

```text
telemetry.yaml
      ↓
ObservationDefinition
      ↓
ObservationKey
      ↓
ObservationHandle
      ↓
SensorRepository
```

The generated definition is an implementation bridge, not a second source of truth.

---

# Phase 4 — Make `SensorTile` Domain-Neutral

**Status: Complete**

The previous `SensorType` dependency has been removed from `SensorTile`.

The runtime tile now carries generic presentation state and generic display-scale metadata.

Domain meaning remains in observation definitions rather than becoming a runtime enum.

This allows the same runtime data model to represent observations from completely different domains.

The framework therefore does not need to know that one value is solar production while another is temperature merely to store or display it.

---

# Phase 5 — Generate Screen Definitions

**Status: Complete**

`telemetry.yaml` now supports declarative Screen composition.

The proving model introduced structures such as:

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

This was the first important step toward Screen provisioning from a single declarative source.

It also produced the Energy Composition Proving Slice.

The proving slice demonstrated that the Screen's membership and ordering could come from YAML rather than from hardcoded observation knowledge inside the Screen implementation.

That was deliberately a proving step.

It is not the final Screen architecture.

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

This removes duplicated application knowledge and reinforces the principle that the application should declare an observation once.

---

# Phase 7 — Generic Screen Composition

**Status: In Progress**

## The Pressure

The Energy Composition Proving Slice has achieved an important goal: the Screen can consume generated observation composition.

But it also exposed a new problem.

The Screen still interprets that composition through an implementation concept of rows, columns and six fixed quadrants.

That means the proving mechanism has started to become the abstraction itself.

The architecture must not make that mistake.

The six-tile Energy Status layout is a proving implementation, not the architectural destination.

The real pressure is:

> **Can a Screen become a generic canvas onto which the application composes presentation components, without the Screen implementation knowing which observations or layout happen to exist?**

This is a composition problem, not an Energy Screen problem.

## The Application Definition Studio — The New Architect in the Room

Phase 7 also welcomes a new architectural participant: the **Telemetry Application Definition Studio**.

The Studio exists for the person who wants to create or modify a Telemetry application without needing to understand YAML syntax, generated C++ or the internal runtime structures used by the framework.

Its purpose is not to replace the Application Definition.

Its purpose is to make that definition understandable and editable by humans while preserving the same machine-readable source of truth.

The conceptual relationship is:

```
Telemetry Application
│
├── Observations
│   ├── Current Power Production
│   ├── Current Power Consumption
│   ├── Battery Charge
│   └── Battery Power
│
├── Screens
│   ├── Energy Status
│   │   ├── Chrome
│   │   └── Canvas
│   │       ├── Battery Card
│   │       ├── Gauge Card
│   │       └── Value Card
│   │
│   └── Weather
│
└── Build
```

The Studio should understand and maintain the relationships represented by that model.

For example, an author should be able to:

- add, edit or remove an Observation;
- define or change its Source;
- create or modify a Screen;
- compose a Canvas;
- add, remove, resize and rearrange Cards;
- select which Observations a Card references;
- configure presentation properties appropriate to the selected Card;
- validate or build the Application Definition;
- load an existing `telemetry.yaml` and continue editing it.

The Studio should make invalid or incomplete relationships visible at the authoring boundary rather than allowing the author to discover them later through compiler errors.

This is particularly important for the transition from:

```
human intent
      ↓
application definition
```

to:

```
application definition
      ↓
ScreenDefinition
      ↓
observation alias
      ↓
ObservationHandle
      ↓
SensorRepository
```

The Studio does not bypass that pipeline.

It makes the first step less painful and less error-prone.

### The Studio Is Not the Source of Truth

The authoritative model remains:

`telemetry.yaml`

The supported authoring paths therefore become:

```
human author
    │
    ├── Application Definition Studio
    │          │
    │          ▼
    │     telemetry.yaml
    │
    └── text editor
               │
               ▼
          telemetry.yaml
```

Both paths feed the same build-time composer.

The Studio must not generate a parallel proprietary project format that becomes more authoritative than the YAML definition.

Its implementation technology is intentionally not frozen by Phase 7. A desktop application, a local web interface, a Python-based tool or another appropriate implementation may be considered later. The architectural contract comes first.

### The Studio as an Architectural Forcing Function

The Studio is also a design tool for the architects and framework developers.

If a person cannot reasonably create a composition through the Studio model, that may indicate that the underlying Application Definition itself is too fragmented, ambiguous or implementation-oriented.

The Studio should therefore force uncomfortable questions such as:

- Where does a Card get its Observation?
- Where does an Entities Card define ordering?
- Where does a Gauge obtain its minimum and maximum?
- Which presentation properties belong to the Observation and which belong to the Card?
- How are Sources related to Observations?
- What happens when an Observation is removed while a Card still references it?
- Which Card types are available for the selected platform?
- How does the author discover an invalid or unsupported composition before building?

These are not merely user-interface questions.

They are architecture questions.

The Studio therefore becomes another way to test whether the boundaries of Observation, Source, Screen, Canvas, Card and Platform are actually coherent.

## The Target

The intended model is:

```text
Application Definition
    │
    ├── observations
    │
    └── screens
          │
          ├── chrome
          │
          └── canvas
                │
                └── cards
                      │
                      └── observation references
```

At runtime, the relationship becomes:

```text
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
SensorRepository
      ↓
DisplayManager
```

The Screen implementation provides the runtime machinery required to render a declarative composition.

It should not contain knowledge of:

- which observations exist;
- how many cards exist;
- which observations belong together;
- whether the layout uses two columns, three columns or another grid;
- whether a card is a value, gauge, battery, entities list, graph or another supported capability;
- the application-specific meaning of those observations.

## Screen Chrome and Canvas

The current display already has a useful conceptual separation.

The upper area contains Screen chrome such as:

- title
- date
- time
- status indicators
- Wi-Fi state

The remaining display real estate can be treated as the Screen's Canvas.

Initially, this does **not** require making the chrome fully declarative.

The important architectural move is to establish the Canvas boundary first:

```text
┌───────────────────────────────────────┐
│              Screen Chrome            │
│         title / date / time           │
├───────────────────────────────────────┤
│                                       │
│                Canvas                 │
│                                       │
│   Card       Card        Card         │
│                                       │
│        Card                Card       │
│                                       │
└───────────────────────────────────────┘
```

If architectural pressure later demonstrates that chrome should also become composable, that can be introduced deliberately rather than assumed in advance.

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

Or:

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

This preserves three separate concepts instead of collapsing them into one object.

```text
Observation = knowledge
Card        = presentation
Canvas      = composition + geometry
Screen      = runtime context
```

## Initial Card Vocabulary

The initial framework vocabulary should remain deliberately small.

Candidate proving capabilities are:

- `value`
- `gauge`
- `battery`
- `entities`

These are examples of framework capabilities, not a promise to reproduce Home Assistant's card catalogue.

Home Assistant may be an inspiration for the user experience, but Telemetry's architectural model remains its own.

Telemetry models **Observations**, not an application-specific entity system.

Additional Card types should be introduced when real architectural pressure justifies them.

Possible future examples include:

- graph
- trend
- weather
- image
- button
- switch
- progress
- statistics

No future Card type should be introduced merely because it is fashionable or because another UI system has it.

## The Battery Example

A battery observation illustrates why presentation must remain separate from domain knowledge.

For example, the application may declare:

```yaml
battery_charge_percentage:
  key: sensor.alphaess_instantaneous_battery_soc
  type: percentage
  unit: "%"
```

That observation could later be presented as:

```text
Battery Card
┌─────────────────────────────┐
│ ███████████████░░░░   82 %  │
└─────────────────────────────┘
```

The Card could map semantic threshold roles such as:

```yaml
thresholds:
  - below: 15
    role: critical
  - below: 50
    role: warning
  - role: normal
```

The application expresses the meaning of the thresholds.

The platform decides how those semantic roles map onto its available visual capabilities.

For example, the ESP8266 may have only a small palette available, while a stronger ESP32 target may support a richer colour representation.

The application should not need to encode the physical palette in order to say:

```text
critical
warning
normal
```

This is an important separation between application semantics and platform expression.

## The Canvas Is a Logical Grid

The Canvas should describe logical geometry rather than physical pixels.

For example:

```yaml
canvas:
  columns: 12
```

A Card occupies a region of that logical grid:

```yaml
grid:
  column: 1
  row: 1
  columns: 4
  rows: 3
```

The layout system translates this logical geometry into physical display coordinates.

This keeps application composition independent of:

- display resolution
- screen orientation
- pixel offsets
- driver implementation
- device-specific margins

The application therefore describes:

```text
where a thing belongs conceptually
```

rather than:

```text
which pixels happen to render it today
```

## Deterministic Runtime Behaviour

Generic composition must not automatically imply dynamic allocation or a large runtime UI framework.

Telemetry targets resource-constrained microcontrollers.

The logical Canvas and Card model should therefore be capable of compiling into deterministic runtime structures such as:

```text
static ScreenDefinition
static CanvasDefinition
static CardDefinition[]
static observation references
```

The build-time composer can validate:

- card type availability
- grid bounds
- overlapping regions, where prohibited
- observation references
- Card-specific configuration
- platform capability constraints
- total composition capacity

The runtime should execute the validated result rather than constructing an arbitrary UI system dynamically.

That keeps the architecture generic without requiring the runtime to become heavyweight.

## Resource Pressure Is an Architectural Concern

This phase explicitly recognises a question raised by the ESP8266 platform boundary:

> **At what point does generic composition itself become too expensive for the target?**

The answer must not be guessed from desktop intuition.

It should be discovered empirically.

The earlier framebuffer experiment already established this pattern. Increasing display depth compiled successfully but caused the ESP8266 to enter a runtime reboot loop. The lesson was not merely that a particular experiment failed.

The lesson was:

> **Platform resource limits are architectural knowledge.**

The same reasoning applies to Screen composition.

A generic Screen could consume resources through:

- framebuffer size
- Card metadata
- generated code size
- RAM used by runtime structures
- stack usage
- renderer complexity
- temporary buffers
- font assets
- graphing or image data
- rendering time

The architecture therefore needs a capability model in which a target can say:

```text
ValueCard       available
GaugeCard       available
BatteryCard     available
EntitiesCard    available
GraphCard       unavailable
RichChartCard   unavailable
```

without changing what the application means by its observations.

This is not a failure of generic composition.

It is the architecture discovering the actual capability boundary of the platform.

## Definition of Done

Phase 7 is complete when:

- [ ] Energy Status no longer assumes a six-quadrant layout.
- [ ] The Screen concept contains a logical Canvas.
- [ ] Cards can occupy configurable logical grid regions.
- [ ] Cards reference observations by composition alias.
- [ ] Observation identity remains independent of presentation.
- [ ] The Screen renderer contains no application-specific observation aliases.
- [ ] Layout is independent of physical display pixels.
- [ ] Screen chrome is conceptually separated from Canvas composition.
- [ ] Composition can be represented by deterministic generated structures.
- [ ] The design has an explicit place for platform capability constraints.
- [ ] The Application Definition model can be represented coherently through a human-facing authoring interface.
- [ ] The Studio can load and preserve an existing `telemetry.yaml` definition.
- [ ] The Studio model maintains relationships among Observations, Sources, Screens, Canvas, Cards and Build configuration.
- [ ] The Studio does not become a second source of truth or proprietary runtime model.

---

# Phase 8 — Generic Card Rendering

**Status: Planned**

Phase 7 establishes what a generic Screen is.

Phase 8 establishes how the framework renders the generic Card capabilities used by that Screen.

The distinction is deliberate.

Phase 7 is primarily an architectural and composition problem.

Phase 8 is primarily a framework rendering problem.

## The Target

The framework should provide reusable Card renderers behind a stable capability boundary.

Conceptually:

```text
CardDefinition
      ↓
Card renderer selection
      ↓
Card renderer
      ↓
Observation runtime state
      ↓
DisplayManager
```

A renderer should not know which application declared the Card.

For example, `BatteryCard` should know how to render a battery representation from an observation.

It should not know that the observation came from AlphaESS.

Similarly, `GaugeCard` should know how to render a gauge.

It should not know that the value represents solar production.

## Initial Renderer Set

The initial proving set should remain deliberately small:

```text
ValueCard
GaugeCard
BatteryCard
EntitiesCard
```

The implementation should prove that multiple Card types can consume the same generic observation model without introducing domain-specific runtime dependencies.

## Card-Specific Configuration

Card-level configuration belongs to the Card rather than being forced into the Observation.

For example, a gauge may need:

- minimum
- maximum
- scale
- display mode

A battery may need:

- threshold roles
- fill behaviour
- orientation

An entities card may need:

- a list of observations
- ordering
- optional per-item labels

These are presentation concerns.

The Observation remains responsible for what is being observed and how its value is identified and sourced.

## Resource Budget

Phase 8 must treat resource usage as a first-class implementation requirement.

A generic renderer architecture is only successful if it remains viable on the target platforms for which the capability is declared.

The implementation should therefore prefer:

- fixed-capacity structures
- static generated definitions
- deterministic renderer state
- predictable memory use
- shared assets where practical
- reuse of existing DisplayManager primitives
- build-time validation of unsupported combinations

The framework should not introduce heap-heavy general-purpose UI mechanisms merely to make the composition API look elegant.

## Definition of Done

Phase 8 is complete when:

- [ ] At least two Card renderer types exist behind a generic Card interface or equivalent capability boundary.
- [ ] Card renderers consume generic Observation runtime state.
- [ ] No Card renderer contains application-specific observation aliases.
- [ ] Card-specific configuration is separated from Observation identity.
- [ ] The Canvas layout can invoke multiple Card types in one Screen.
- [ ] Rendering remains deterministic on supported platforms.
- [ ] Resource usage is measured rather than assumed.
- [ ] Unsupported Card/platform combinations are rejected or excluded at composition time.

---

# Phase 9 — Migrate Weather into Composition

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

The Weather Screen should then consume the same generated Screen and Card composition model as Energy.

The result should be that Weather becomes another application composition example rather than a special architectural case.

## Why Weather Comes After Generic Cards

Weather is an especially useful proving domain because it can exercise several forms of presentation:

- current value
- trend
- grouped observations
- multiple units
- future graphical presentation

Migrating Weather after the generic Screen and Card model exists prevents Weather-specific rendering requirements from accidentally defining the framework abstraction.

The framework should discover what Weather actually requires instead of assuming that an existing Weather Screen is itself the generic model.

## Definition of Done

Phase 9 is complete when:

- [ ] Weather observations are declared in `telemetry.yaml`.
- [ ] Weather observation sources are co-located with those observations.
- [ ] Weather observations resolve through generated composition.
- [ ] Weather no longer requires a parallel domain-specific registration path.
- [ ] Weather Screen composition is represented by the same Canvas/Card model as Energy.
- [ ] Weather-specific behaviour remains isolated where it is genuinely specialised.

---

# Phase 10 — Platform-Aware Presentation Capabilities

**Status: Planned**

By this stage Telemetry will have a generic composition model and a generic set of Card renderers.

The next pressure is to make the relationship between those capabilities and real hardware, explicit.

The framework should not pretend that every target can render every Card equally cheaply.

## Capability Model

Conceptually:

```text
Framework capability
        ↓
Card type
        ↓
Platform support
```

For example:

```text
                     ESP8266        ESP32

ValueCard               ✓             ✓
GaugeCard               ✓             ✓
BatteryCard             ✓             ✓
EntitiesCard            ✓             ✓
GraphCard               ?             ✓
RichChartCard           ✗             ✓
ImageCard               ✗             ✓
```

These symbols are placeholders for measured capability, not permanent promises.

The important architectural rule is:

> **The application expresses meaning and presentation intent. The target platform determines which implementation capabilities are available.**

## Semantic Roles and Physical Expression

The same principle applies to visual resources.

An application may express:

```text
critical
warning
normal
```

The ESP8266 may map those roles to its available 2-bit palette.

A stronger target may map them to richer colours.

Likewise:

```text
upward flow

downward flow
```

can remain semantic presentation intent while the platform chooses the actual glyph, colour or available visual primitive.

The application should not need to understand the physical framebuffer implementation merely to express those meanings.

## Resource Budgets as Capabilities

Platform awareness should eventually include measurable constraints such as:

- available framebuffer memory
- available RAM
- maximum generated Card count
- maximum Canvas complexity
- renderer availability
- asset memory requirements
- execution-time constraints

These limits should be treated as capabilities that the composer can validate where practical.

A build targeting an ESP8266 should therefore be able to reject a composition that requires a capability the target does not provide, rather than discovering the problem only after deployment.

## Definition of Done

Phase 10 is complete when:

- [ ] Card capability availability is represented explicitly per platform.
- [ ] Platform-specific limitations do not leak into application observation semantics.
- [ ] The composer can validate unsupported presentation capabilities where practical.
- [ ] Resource-sensitive limits are documented and measured.
- [ ] ESP8266 remains a supported constrained platform without becoming the architectural lowest common denominator for all future targets.
- [ ] ESP32-family capabilities can grow without forcing a redesign of the application composition model.

---

# Sprint Eta Definition of Done

Sprint Eta is complete when:

- [x] `telemetry.yaml` is established as the application composition source of truth.
- [x] Observation declarations contain identity, meaning, unit, presentation metadata and source.
- [x] Observation sources are co-located with observations.
- [x] The composer is domain-neutral.
- [x] Generic `ObservationDefinition` structures are generated.
- [x] Generic Screen definitions are generated.
- [x] `SensorTile` contains no domain-specific `SensorType`.
- [x] YAML-defined MQTT observations resolve through generated composition metadata.
- [ ] Energy Status no longer defines application composition through a fixed six-quadrant implementation model.
- [ ] A logical Canvas and Card composition model exists.
- [ ] Generic Card renderers exist for the initial capability set.
- [ ] Weather observations are migrated into `telemetry.yaml`.
- [ ] Weather consumes the same generic Screen composition model.
- [ ] Platform presentation capabilities are explicit and composable.
- [ ] Resource constraints are treated as architectural capability boundaries.
- [ ] New Screens can be composed without duplicating application-definition knowledge across runtime files.
- [ ] Generated composition remains deterministic and suitable for constrained targets.
- [ ] The Application Definition model is suitable for both machine consumption and human authoring.
- [ ] A Telemetry Application Definition Studio can author and modify the same application definition consumed by the composer.
- [ ] Human authoring does not require duplication of application knowledge into a separate proprietary source format.

---

# Architectural Result

Sprint Eta establishes three major axes alongside the existing framework boundaries:

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
 ObservationHandle    Screen / Canvas /    ESP8266 / ESP32
          │            Card composition       capabilities
      Repository             │                  │
          │             generated model         │
          └──────────────────┼──────────────────┘
                             │
                             ▼
                       Runtime system
```

The framework therefore moves toward a clearer division:

```text
Framework
    provides capabilities

Application composition
    declares what this application uses

Build-time composer
    validates and provisions the implementation structures

Platform composition
    selects what this target can instantiate
```

The application's knowledge should survive changes in:

- transport
- repository storage
- Screen layout
- Card implementation
- display resolution
- display palette
- microcontroller generation

That is a strong indication that the right boundaries are being preserved.

---

# The Architectural Lesson

The pressure looked like a Screen-creation problem.

It was actually a composition problem.

The first answer was to move application knowledge into `telemetry.yaml`.

The Energy Composition Proving Slice proved that this could work.

Then the proving slice exposed its own limitation.

A generated Screen definition is not enough if the Screen still thinks in terms of a fixed six-quadrant implementation.

The next answer is therefore not:

> **Make the six quadrants generic.**

It is:

> **Discover a generic Screen model based on a Canvas and Cards.**

That distinction matters.

A generic framework should not preserve today's layout merely because today's layout happened to be the first successful consumer of the composition system.

The application should be able to decide whether its real estate contains:

- two large gauges;
- a battery Card beside a value Card;
- an Entities Card spanning the Canvas;
- several small Cards in a grid;
- a mixture of Cards with different logical sizes;
- or another composition supported by the target platform.

The architecture should make those choices declarative.

But it should also remain honest about hardware.

Generic composition does not mean that every target can render every composition.

The ESP8266 has already taught us that a platform can compile code that is not actually viable at runtime. That lesson must remain part of the architecture:

> **Test platform assumptions instead of theorising about them.**

The resource boundary is therefore not something the architecture works around after the fact.

It is something the architecture should expose.

The goal is not to make every platform identical.

The goal is to make application intent independent of the accidental limitations of one particular implementation while allowing the target to declare what it can actually provide.

The same discipline applies to the human boundary.

The goal is not to hide the Application Definition from developers who want to edit YAML directly. The goal is to make that definition accessible to people who should be able to compose an application without first learning every implementation detail behind it.

That makes the Telemetry Application Definition Studio a useful architectural participant as well as a usability tool.

This leads to a more mature composition model:

```text
Application knowledge
        ↓
telemetry.yaml
        ↓
Build-time composer
        ↓
ScreenDefinition
        ↓
Canvas
        ↓
Card composition
        ↓
Platform capability selection
        ↓
Generated runtime structures
        ↓
Telemetry framework
```

The source file describes knowledge and presentation intent.

The Application Definition Studio provides a human-facing way to create and maintain that definition without becoming a second source of truth.

The composer validates it and translates it.

The platform determines which capabilities are available.

The runtime executes the result.

And the architectural boundary becomes visible rather than accidental.

> **The Publisher is concerned with knowledge, not files.**

For Telemetry, the equivalent rule is:

> **The application declares knowledge once; the build system provisions the implementation structures that express it.**

And the next refinement is:

> **The application declares what it wants to express; the platform determines what it can afford to render.**
