# Actor — The Application Developer

The application developer knows what the application means.

They should not need to know how every framework capability is implemented.

## Responsibility

The application developer declares:

- which observations matter;
- what those observations mean;
- how they are sourced;
- how they are composed into presentations.

## Telemetry's Promise

The developer should be able to express application knowledge once:

```text
telemetry.yaml
```

and allow the build system to provision the framework structures needed to execute that description.

## Boundary

The application should not own:

- repository storage slots;
- generated source mappings;
- driver details;
- platform-specific implementation choices.

The application owns meaning and composition.
