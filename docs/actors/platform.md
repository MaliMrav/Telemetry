# Actor — The Platform Engineer

The platform engineer deals with what the hardware can actually sustain.

## Platform Is Evidence

A framework may describe a capability, while a particular platform may be unable to instantiate it within its resource budget.

That is not necessarily a framework failure.

It may simply be a composition boundary.

## ESP8266 and ESP32/CYD

Telemetry deliberately treats these as useful reference profiles:

- **ESP8266** — constrained, observation-oriented.
- **ESP32/CYD** — expanded resources and richer observation-and-control possibilities.

The framework architecture remains shared.

PlatformIO composition decides which capabilities are instantiated.

## The Colour-Depth Experiment

Moving from a two-bit to a four-bit framebuffer compiled successfully but caused runtime reboot behaviour on the ESP8266.

That experiment established an empirical resource boundary.

The lesson is simple:

> **Test the platform you actually intend to ship on.**
