# 009 — Energy Composition Proving Slice

## Why a proving slice?

Sprint Eta established declarative application composition as the architectural direction.

The next temptation would have been to migrate everything immediately.

Instead, Telemetry deliberately took a smaller step:

> **Can one real Screen prove that the composition model works across different observations, sources, types and presentation policies?**

The Energy Status Screen became that proving slice.

The purpose was not to finish the whole framework.

The purpose was to expose whether the architecture survived contact with a real application.

---

## The Screen

The target presentation combined six observations:

```text
Current
├── Production
└── Consumption

Today
├── Production
└── Consumption

Battery
├── Charge
└── Power
```

The observations were deliberately not all the same kind of thing.

They included:

- power;
- energy;
- percentage;
- signed power.

They also came from different external resources.

That made the slice useful as an architectural test.

---

## What Was Proven

### 1. Screen layout is genuinely orchestrated by `telemetry.yaml`

The Screen no longer owns the complete list of observations and their row membership.

The composition model describes the Screen layout.

The runtime consumes the resulting `ScreenDefinition`.

This proves that YAML is doing more than configuring constants around hand-written application logic.

It is expressing application composition.

### 2. Orchestration is independent of observation type

The Screen composition does not need a different mechanism for:

```text
power
energy
percentage
signed power
```

The observation definition carries meaning and presentation metadata.

The Screen definition carries composition.

Those concerns remain separate.

### 3. One Screen can combine different resources and meanings

The Energy Screen became a useful demonstration that observations do not need to share a source or a domain type simply because they share a presentation.

The Screen consumes observations by identity.

It does not need to care whether one arrived from an Envoy-related MQTT publisher and another from AlphaESS.

### 4. MQTT topics are transport contracts

The AlphaESS entities use semantic Home Assistant identities such as:

```text
sensor.alphaess_instantaneous_battery_soc
sensor.alphaess_instantaneous_battery_i_o
```

while the ESP8266 transport uses intentionally generic topics:

```text
ha/battery/charge_percentage
ha/battery/power
```

This keeps transport naming from pretending to be domain identity.

The same observation can eventually be supplied through another transport on a more capable platform.

### 5. The old half-orchestrated Screen was exposed

The proving slice revealed an important weakness.

The YAML could describe the Screen, but the Screen implementation still contained application-specific assumptions.

That was not a failure of the composition model.

It was evidence of an incomplete migration.

The architecture had exposed exactly where the remaining knowledge lived.

### 6. Quadrant creation became observation-neutral

The Screen's quadrant rendering could operate on a generic `SensorTile` rather than knowing that a particular quadrant represented energy, power or percentage.

This reinforced the domain-neutral presentation boundary.

### 7. Signed flow became a presentation concept

Battery power introduced a useful distinction.

A signed value has a physical meaning, but the visual arrow is a presentation interpretation.

It was deliberately not modelled as `TREND_UP` or `TREND_DOWN`.

The resulting concept is:

```text
signed_flow
```

Positive values show energy leaving the battery.

Negative values show energy entering the battery.

Zero has no directional arrow.

This is a presentation policy, not a battery domain enum and not a trend measurement.

### 8. The ESP8266 resource boundary was discovered experimentally

A four-bit framebuffer experiment compiled and linked successfully but caused runtime reboot behaviour on the ESP8266.

The system therefore returned to the two-bit framebuffer.

The important architectural lesson was not simply “four-bit is too much.”

It was:

> **Platform capability must be established empirically.**

The ESP8266 remains a constrained reference profile.

The ESP32 family can legitimately provide a richer capability profile.

### 9. The two-bit palette became clearer

Two-bit colour does not mean one fixed set of four semantic colours.

It means four palette slots.

Those slots may be assigned to whichever four colours best serve the target.

The semantic names in the framework must match the actual palette meaning.

### 10. Composer responsibility became clearer

The proving slice confirmed that the composer is most useful when it remains narrow:

```text
YAML
  ↓
validation
  ↓
translation
  ↓
generated composition
```

The composer should not acquire Energy, Weather, Envoy or AlphaESS logic merely because those applications happen to use the framework.

---

## The Runtime Identity Chain

The proving slice also exercised the complete chain:

```text
YAML alias
    ↓
ObservationDefinition
    ↓
ObservationKey
    ↓
ObservationHandle
    ↓
SensorRepository
    ↓
SensorTile
    ↓
Screen
```

This is important because it proves that the declarative composition model did not replace the earlier identity architecture.

It sits on top of it.

---

## The Architectural Pressure It Created

The proving slice did not finish the problem.

It created the next useful pressure:

> **How much application knowledge should remain inside a specialised Screen implementation once composition is already declarative?**

That is a better problem than the one we started with.

We now have evidence that:

```text
telemetry.yaml
      ↓
composer
      ↓
generated composition
      ↓
runtime
```

works.

The next work is to decide which remaining Screen behaviour is genuinely behavioural and which is still duplicated application definition.

---

## What We Learned

The proving slice established more than the original success criteria.

We learned that:

1. composition can genuinely control Screen layout;
2. composition can remain independent of observation domain type;
3. different resources can coexist on one Screen;
4. transport identity can remain separate from semantic identity;
5. a partially declarative architecture exposes its remaining hard-coded knowledge;
6. generic rendering can handle observations with different presentation requirements;
7. presentation semantics such as signed flow deserve their own concepts;
8. platform resource limits are real architectural boundaries;
9. capability composition is a better response to platform differences than architectural forks;
10. the composer can remain a translator rather than becoming an application-specific rules engine.

---

## Why This Belongs in the Architecture History

The proving slice is important because it is not merely a feature milestone.

It changed our understanding of the architecture.

Before the slice, declarative composition was an architectural hypothesis.

After the slice, it had empirical support.

At the same time, the experiment exposed the next pressure.

That is what an architecture history should record:

```text
Hypothesis
    ↓
Experiment
    ↓
Evidence
    ↓
New understanding
    ↓
New pressure
```

The architecture is therefore not a finished diagram.

It is a record of learning.

> **Follow the pressure, not the fashion.**

> **Test the architecture where reality can disagree with it.**
