# Actor — The Framework Developer

The framework developer protects the reusable architecture.

## Responsibility

Telemetry provides capabilities such as:

- observation registration;
- runtime identity;
- repositories;
- data-source contracts;
- display capabilities;
- input normalisation;
- navigation ownership;
- build-time composition.

## The Boundary

The framework should not acquire application-specific vocabulary merely because an application needs a feature.

A framework developer should ask:

> “Is this a reusable capability, or is this application meaning?”

## The Test

If adding a new application requires adding its domain to the framework, the boundary deserves examination.
