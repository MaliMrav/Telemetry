# Design Principles

These principles are not commandments written before the project began.

They are lessons that survived repeated contact with real code, real hardware and real architectural pressure.

## Good architecture should feel inevitable

Architecture should follow from responsibility, evidence and constraints.

Cleverness is not the goal.

## Architecture precedes implementation

Decide what the system means before deciding which class or file should contain it.

Implementation should express the architecture rather than accidentally become the architecture.

## The application declares knowledge once

Application meaning should not be copied across unrelated runtime files.

`telemetry.yaml` exists to express application composition once.

## The composer translates; it does not own meaning

The build-time composer validates and translates application declarations.

It must not become a second application language containing knowledge of Energy, Weather, Envoy or AlphaESS.

## Identity describes meaning; storage describes implementation

`ObservationKey` is semantic.

`ObservationHandle` is runtime identity.

Repository storage is implementation.

They should not be the same concept.

## Separate meaning from transport

A Home Assistant entity identity and an MQTT topic are not interchangeable.

One describes what is known.

The other describes how it currently arrives.

## Separate presentation from domain

A `SensorTile` should not need to know whether its value represents power, humidity or pressure merely to render it.

Domain meaning belongs in observation definitions.

Generic presentation metadata belongs at the presentation boundary.

## Composition before duplication

If the same application knowledge is required by multiple runtime structures, prefer a single source from which those structures can be composed.

## Capabilities before hardware

Describe what the framework can do before describing which board happens to provide it.

Platform composition selects capabilities for a target.

## Constraints are evidence

A reboot, memory failure, timing problem or unavailable peripheral is architectural evidence.

Do not dismiss it as an inconvenience.

## Small proving slices beat large speculative refactors

Prove an architectural claim with the smallest useful experiment.

Then use the evidence to decide what should happen next.

## Context interprets; scope owns

Input produces semantic interaction.

The active context interprets it.

The component responsible for the resulting scope executes it.

## The framework provides capabilities; applications provide meaning

This boundary protects both sides.

The framework remains reusable.

The application remains expressive.

## Observability is an architectural capability

A system that cannot explain what it is doing is difficult to maintain.

When direct observability is unavailable, design another useful observation path rather than guessing.

## Preserve the journey

A decision is easier to understand when its pressure is preserved.

The history is part of the architecture.

## Knowledge before Wisdom

We document facts, decisions, evidence and reasoning because those are the material from which readers can form their own judgement.

The ambition is not merely to transfer instructions.

It is to pass on understanding.

> **Create Knowledge that can pass on Wisdom.**
