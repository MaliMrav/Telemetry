# Learning — From YAML to Runtime

Telemetry's declarative composition model turns application knowledge into generated runtime structures during the build.

```text
telemetry.yaml
      ↓
composer
      ↓
validation
      ↓
generated composition
      ↓
compiler
      ↓
firmware
      ↓
runtime
```

## The Authoring Model

The application author declares observations with identity, meaning, presentation metadata and source information.

Then Screens reference those observations by composition alias.

The author does not assign repository storage positions or maintain a second generated source mapping.

## The Composer

The composer is a build-time translator.

It should:

1. read the declarative model;
2. validate it;
3. normalise it;
4. generate runtime definitions;
5. generate registration structures;
6. fail the build when the model is invalid.

It should not become a second source of application semantics.

## The Generated Layer

Generated `ObservationDefinition` data forms a bridge between YAML and runtime contracts.

Conceptually:

```text
telemetry.yaml
     │
     ├── observation identity
     ├── presentation policy
     ├── source mechanism
     └── Screen composition
             │
             ▼
     generated definitions
             │
             ▼
     Telemetry runtime
```

## Why Build-Time?

Build-time composition gives us deterministic firmware.

The YAML remains the source of truth.

Generated files under `.pio/build/` are disposable implementation artifacts.

The runtime does not need a YAML parser merely to reconstruct the application definition.

## What the Model Proves

The Energy Composition Proving Slice demonstrates that one Screen can combine:

- power;
- energy;
- percentage;
- signed power;
- different MQTT topics;
- different presentation policies.

That is stronger evidence than a generic example because the same composition model is exercised by a real application slice.

## The Architectural Lesson

The important feature is not YAML.

The important feature is that application knowledge has a single authoritative home and can be translated into the structures the framework needs.

> **The application declares knowledge once; the build system provisions the implementation structures that express it.**
