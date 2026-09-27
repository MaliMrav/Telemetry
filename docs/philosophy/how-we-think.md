# How We Think

Architecture is not primarily the ability to draw boxes.

It is the ability to notice pressure, understand who experiences it, identify who owns the knowledge involved, and choose a boundary that makes the next decision easier rather than harder.

Telemetry uses the following habits.

## 1. Start With Pressure

We do not begin with:

> “Which pattern should we use?”

We begin with:

> “What has become difficult, and why?”

The architecture history therefore starts with pressure rather than technology.

A useful pressure statement names the difficulty without prematurely naming the solution.

## 2. Walk the Boundary Before Defining It

Before creating an abstraction, follow the information across the system.

Ask:

- Who creates the knowledge?
- Who transforms it?
- Who owns it?
- Who consumes it?
- Which parts are implementation details?
- Which parts must remain stable when implementation changes?

Only then do we decide where a boundary belongs.

## 3. Ask Who Owns the Knowledge

Ownership is more useful than proximity.

The code nearest a piece of information is not necessarily the code that should own its meaning.

For example:

```text
telemetry.yaml
    owns application composition

composer
    owns translation and validation

ObservationRegistry
    owns runtime identity

SensorRepository
    owns runtime storage

source
    owns acquisition

Screen
    owns presentation behaviour

ScreenManager
    owns navigation
```

Clear ownership reduces accidental coupling.

## 4. Separate Meaning From Mechanism

An observation has meaning.

MQTT has a mechanism.

A topic is transport detail.

The application should be able to say what it knows without making the transport mechanism part of that knowledge.

This is why Telemetry treats:

```text
sensor.alphaess_instantaneous_battery_i_o
```

and:

```text
ha/battery/power
```

as different kinds of information.

The first is semantic identity.

The second is transport location.

## 5. Separate Identity From Storage

A semantic identity should not secretly be an array position.

Telemetry therefore uses:

```text
ObservationKey
    ↓
ObservationHandle
    ↓
SensorRepository
```

Identity describes meaning.

Storage describes implementation.

## 6. Follow the Pressure, Not the Fashion

We do not add abstraction because abstraction is fashionable.

We add it when a boundary reduces a real kind of complexity.

Likewise, we do not avoid abstraction merely because it adds code.

The question is always:

> **What pressure does this abstraction relieve?**

## 7. Test Hardware Assumptions

Hardware is an authority that eventually wins every argument.

The ESP8266 four-colour framebuffer experiment is a good example. A richer colour depth compiled and linked successfully, but the running system rebooted. The experiment established a real resource boundary.

That evidence is more valuable than a theoretical argument about available RAM.

## 8. Treat Constraints as Teachers

A constrained platform can expose architectural truths that a larger platform allows us to ignore.

The ESP8266 teaches resource discipline.

The ESP32/CYD can provide a richer capability profile.

Neither platform needs to define the architecture by itself.

## 9. Prefer the Smallest Proof

When a large refactor is uncertain, prove one architectural claim with the smallest useful slice.

The Energy Composition Proving Slice did exactly that.

It demonstrated that one Screen could contain observations with different meanings, different sources and different presentation requirements while being composed from `telemetry.yaml`.

The slice was deliberately small enough that failure would teach us something rather than merely producing a large amount of churn.

## 10. Let Architecture Become Inevitable

The strongest architecture is often the one that becomes obvious after the pressure is understood.

> **Good architecture should feel inevitable.**

That does not mean there is only one possible design.

It means the chosen design follows naturally from the responsibilities, constraints and evidence we have uncovered.

## 11. Preserve the Reasoning

A final implementation is not enough.

Without the reasoning, a future maintainer may remove an apparently unnecessary boundary and recreate the original problem.

Architecture history therefore records:

- what we wanted;
- what became difficult;
- who experienced the difficulty;
- what alternatives existed;
- what we chose;
- why we chose it;
- what capability that created;
- what new pressure it exposed.

## 12. Teach Without Pretending the Journey Was Straight

A learning repository should not erase its mistakes.

Malformed generated C++, obsolete `SensorType` dependencies, transitional Weather code and hardware experiments are useful evidence.

The goal is not to pretend that good architecture appears fully formed.

The goal is to show how an architect recognises the pressure and learns from the evidence.
