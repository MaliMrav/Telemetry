# ESP8266 MQTT Weather Display

> **This project is as much a software architecture exercise as it is a weather display.**
>
> What began as a simple MQTT-powered environmental dashboard has evolved into a reusable embedded framework that explores modular architecture, separation of concerns, maintainability, and efficient use of constrained ESP8266 resources.
>
> The display node intentionally performs very little computation itself. Wherever practical, data processing is delegated to upstream systems such as Home Assistant, allowing the ESP8266 to focus on what it does best: consuming data, presenting information, and remaining reliable.

---

## Why This Project Exists

This project started with a very practical problem.

I had multiple ESP8266-based BME280 sensor nodes deployed around the house, each with its own small OLED display. While functional, the OLEDs suffered from burn-in and were not ideal for long-term operation. They also complicated future plans for battery-powered sensor nodes due to their continuous power consumption.

The initial goal was simple:

* Collect environmental data from multiple locations around the property.
* Publish the data via MQTT.
* Present everything on a single dedicated display.

The first two monitored locations were:

* Kitchen (inside)
* Pergola (outside)

Over time, the project evolved from a weather display into something larger: a reusable MQTT-driven dashboard framework capable of displaying any MQTT-based data source.

At the same time, the codebase itself became a software engineering exercise in designing maintainable embedded systems using modern tooling and architectural practices.

---

# Development Journey

The original version of this project was developed in Arduino IDE as a single-file proof of concept.

As functionality increased, maintaining the code became progressively more difficult. Features, display logic, MQTT handling, OTA updates, WiFi management, and configuration all lived together in a single application file.

A conscious decision was made to modernise the project and migrate to:

* Visual Studio Code
* PlatformIO
* Modular architecture
* Source control friendly configuration
* Separation of concerns
* Reusable components

The goal was not simply to reorganise files, but to create a codebase that could continue growing without becoming difficult to understand or maintain.

Today the project is structured around clearly defined responsibilities and module ownership, while remaining approachable for hobbyists and makers.

---

# Design Philosophy

A key architectural decision was made early in the refactoring process:

**The display should remain a presentation device.**

Rather than performing complex calculations locally, the ESP8266 consumes already-processed information published to MQTT topics.

This allows the display firmware to remain:

* Lightweight
* Efficient
* Responsive
* Easier to maintain

Currently, Home Assistant performs much of the heavy lifting.

Examples include:

* Daily minimum calculations
* Daily maximum calculations
* Trend determination
* MQTT topic publishing

This means the display simply subscribes to:

```text
temperature
temperature_min
temperature_max
temperature_trend
```

rather than calculating these values itself.

Advantages include:

* Reduced RAM consumption
* Lower CPU utilisation
* Simpler firmware
* Centralised business logic
* Easier experimentation

As the project evolves, Home Assistant may also provide:

* Weather forecasts
* Rain probability
* Multi-day outlooks
* Historical trend analysis
* Sensor aggregation

The long-term vision is to keep the display focused on presentation while upstream systems perform the computation.

---

# Hardware

Current hardware platform:

* ESP8266 (ESP-12E)
* 2.4" ILI9341 TFT display
* XPT2046 touch controller
* MQTT-connected BME280 sensor nodes

Display resolution:

* 240 × 320 (portrait)

The touchscreen is currently unused but has been intentionally retained to support future navigation and configuration features.

---

# Software Stack

Development Environment:

* Visual Studio Code
* PlatformIO

Core Libraries:

* ESP8266 Arduino Core
* MiniGrafx
* ArduinoOTA
* WiFiManager
* EspMQTTClient

Supporting Libraries:

* Custom timezone helper library
* Custom font resources
* Weather icon resources

---

# Architecture

## Current Project Structure

```text
src/

├── main.cpp

├── config/
│   ├── settings.h
│   └── secrets.h

├── models/
│   ├── SensorTile.h
│   ├── SensorRepository.h
│   └── SensorRepository.cpp

├── display/
│   ├── DisplayManager.h
│   └── DisplayManager.cpp

├── screens/
│   ├── WeatherScreen.h
│   └── WeatherScreen.cpp

├── mqtt/
│   ├── Topics.h
│   ├── MqttManager.h
│   └── MqttManager.cpp

├── wifi/
│   ├── WifiSetup.h
│   └── WifiSetup.cpp

├── ota/
│   ├── OtaManager.h
│   └── OtaManager.cpp

├── system/
│   ├── SystemManager.h
│   └── SystemManager.cpp
```

---

## Application Flow

```text
main
 ├─ WifiSetup
 ├─ SystemManager
 ├─ SensorRepository
 ├─ OtaManager
 ├─ MqttManager
 ├─ DisplayManager
 └─ WeatherScreen
```

---

## Responsibilities

| Module           | Responsibility                                   |
| ---------------- | ------------------------------------------------ |
| main.cpp         | Application orchestration only                   |
| WifiSetup        | WiFi lifecycle management                        |
| SystemManager    | Hostname and system helper functions             |
| SensorRepository | Owns sensor definitions and runtime sensor state |
| MqttManager      | Receives MQTT data and populates sensor state    |
| DisplayManager   | TFT hardware abstraction                         |
| WeatherScreen    | Presentation and UI logic                        |
| OtaManager       | OTA lifecycle management                         |

This architecture represents a significant improvement over the original monolithic implementation and provides a solid foundation for future growth.

---

# Configuration

Configuration is intentionally centralised to make the project easy to adapt.

Examples include:

* MQTT broker configuration
* Hostname prefix
* Timezone settings
* NTP servers
* Display options
* Feature flags

The long-term goal is to allow users to adapt the dashboard to their own MQTT environments with minimal code changes.

---

# OTA Updates

The display supports wireless firmware updates using ArduinoOTA.

Once connected to WiFi, PlatformIO can upload new firmware directly over the network without requiring a USB connection.

This significantly simplifies ongoing development and maintenance.

---

# Future Weather Forecasting

The current display focuses on presenting actual measurements gathered from local sensors.

Future versions may incorporate weather forecasts generated by Home Assistant and displayed using the included weather icon framework.

The idea originated after a discussion with my son when a weather application confidently predicted a cloudy day, only for him to get thoroughly drenched on his way to an early-morning gym session.

If we're going to display forecasts, we might as well see if we can do better.

---

# Roadmap

## Near-Term

* Move MQTT topics into dedicated configuration modules
* Improve tile configuration flexibility
* Add diagnostic/status screen
* Add touchscreen navigation
* Introduce multiple screen support
* Improve sensor definition abstraction

## Mid-Term

* Configurable dashboard layouts
* Runtime display preferences
* 12/24-hour clock selection
* Forecast screen
* Weather icon integration
* Status and health monitoring

## Long-Term

* Generic MQTT dashboard framework
* Web-based status interface
* Browser-based OTA updates
* Remote configuration
* Ultra-low-power sensor ecosystem
* Battery-powered environmental nodes
* Additional sensor and telemetry types

---

# Future Sensor Nodes

One of the long-term goals of this project is to replace the original display-equipped sensor nodes with ultra-low-power MQTT publishing devices.

The objective is to:

* Eliminate always-on displays
* Extend battery life dramatically
* Reduce hardware complexity
* Publish sensor data directly to MQTT

The display dashboard then becomes the primary user interface for the entire sensor ecosystem.

Future sensor nodes may operate with power consumption measured in microamps while sleeping, enabling months or potentially years of operation from rechargeable battery systems.

---

# Contributing

Contributions, ideas, architectural discussions, and feature suggestions are always welcome.

This project intentionally balances embedded efficiency with maintainable software design and aims to remain approachable for hobbyists while applying sound engineering principles.

---

# Acknowledgements

This project builds upon the excellent work of the open-source community.

Special thanks to the maintainers and contributors of:

* PlatformIO
* ESP8266 Arduino Core
* MiniGrafx
* WiFiManager
* ArduinoOTA
* EspMQTTClient

Without these projects, this dashboard would not exist.
