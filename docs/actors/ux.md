# Actor — The UX Designer

The UX designer is concerned with human interpretation.

## UX Is Not Decoration

Colour, spacing, hierarchy, labels, arrows, interaction zones and timing all influence whether information is understandable.

They are therefore part of the system's presentation semantics.

## The Energy Example

A signed battery power value contains useful information:

```text
positive → energy leaving the battery
negative → energy entering the battery
```

The user does not need the sign convention explained every time. A directional arrow can make the meaning visible at a glance.

## Architectural Consequence

Presentation semantics should be explicit enough to be reused, but should not be confused with domain identity.

`signed_flow` is a presentation concept.

It is not a battery type and it is not a trend direction.
