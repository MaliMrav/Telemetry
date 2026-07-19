# Sprint Gamma — Establish the Control Panel

## Objective

Introduce a structured control area without allowing the application to become a collection of unrelated screens and special cases.

## Architectural Pressure

The Weather screen was the first application experience. But a framework needs more than one screen.

The moment the project needed system information, connectivity information, configuration, diagnostics, and future capabilities, the question became:

> How should related functionality be organised?

## The Control Panel

```text
ControlPanelScreen
│
├── ControlMenuPage
│
├── information/
│   ├── AboutPage
│   └── SystemPage
│
└── capabilities/
    └── ConnectivityPage
```

The Control Panel is a screen-level container. Its pages are local destinations within that screen.

## Control Page Vocabulary

```cpp
enum class ControlPageKind
{
    About,
    Connectivity
};
```

The menu expresses which control page the user selected. It does not know about global screen transitions.

## What This Sprint Did Not Do

Sprint Gamma deliberately did **not** complete Weather → Control Panel navigation.

That work was deferred because Telemetry first needed to answer:

> Who decides that an interaction means "leave this screen and show another one"?

That question became the subject of Sprint Delta.

## Breadcrumb

```text
Input
  ↓
Intent
  ↓
Screen-level navigation?
  ├── Yes → ScreenManager owns the transition
  └── No  → Active Screen interprets the event locally
                    ↓
                 Active Page
```

## Principles Exercised

- Architecture Before Implementation
- One Responsibility Per Capability
- Composition Before Inheritance
- Dependencies Flow Downward
- Good Architecture Should Feel Inevitable
- Architecture is User Experience

## Outcome

**Complete.**

The Control Panel exists as a structured application area. Navigation into it remains intentionally deferred to Sprint Delta's architectural decisions.
