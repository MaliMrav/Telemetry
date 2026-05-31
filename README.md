# ESP8266 MQTT Weather Display

A lightweight MQTT-powered weather dashboard built around the ESP8266, an ILI9341 TFT display, and a modular architecture designed for extensibility, maintainability, and low resource usage.

---

## Why This Project Exists

This project started with a simple goal:

Display temperature, humidity, and pressure readings from multiple MQTT-connected BME280 sensors around my home.

At the time, each sensor node had its own small OLED display. While functional, those displays suffered from burn-in and had limited flexibility. They were also not ideal for future battery-powered deployments.

The idea was to centralise environmental data into a single dedicated display that could be viewed from anywhere in the house while allowing the sensor nodes themselves to become simpler, lower-power devices focused solely on data collection and MQTT publishing.

What began as a simple weather dashboard has since evolved into a reusable MQTT dashboard framework capable of displaying any MQTT-driven data source.

---

## Project Goals

### Current Goals

* Display MQTT sensor data in a clean, readable UI
* Support multiple sensor locations
* Visualise trends and min/max values
* Maintain low memory usage on ESP8266 hardware
* Provide reliable OTA firmware updates

### Future Goals

* Multiple touchscreen-navigable screens
* Configurable dashboards
* System status and diagnostics pages
* Web-based status interface
* Dynamic tile layouts
* Battery-powered ultra-low-power sensor nodes
* Additional data sources beyond weather metrics

---

## Development Journey

The original prototype was developed in Arduino IDE as a single-file proof of concept.

As the project grew, it became increasingly difficult to maintain and extend. At that point a decision was made to migrate the project to:

* Visual Studio Code
* PlatformIO
* Modular architecture
* Version-controlled configuration
* Reusable components

The goal of the refactor was not simply to organise files differently, but to create a codebase that follows modern embedded software engineering practices while remaining approachable to hobbyists and makers.

The project now emphasises:

* Separation of concerns
* Clear module ownership
* Low coupling between components
* Reusability
* Extensibility
* Maintainability

---

# Hardware

Current display hardware:

* ESP8266 (ESP-12E)
* 2.4" ILI9341 TFT display
* XPT2046 touch controller (future use)
* MQTT-connected BME280 sensor nodes

Current display resolution:

* 240 × 320 (portrait)

---

# Software Stack

* PlatformIO
* Visual Studio Code
* ESP8266 Arduino Framework
* MiniGrafx
* ArduinoOTA
* WiFiManager
* EspMQTTClient

---

# Architecture

## Current Structure

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
│   ├── MqttManager.h
│   └── MqttManager.cpp

├── wifi/
│   ├── WifiSetup.h
│   └── WifiSetup.cpp

├── ota/
│   ├── OtaManager.h
│   └── OtaManager.cpp

└── system/
    ├── SystemManager.h
    └── SystemManager.cpp
```

## Architectural Status

At this point the application flow is:

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

### Responsibilities

| Module           | Responsibility                                   |
| ---------------- | ------------------------------------------------ |
| main.cpp         | Application orchestration only                   |
| SensorRepository | Owns sensor definitions and runtime sensor state |
| MqttManager      | Receives MQTT messages and populates sensor data |
| WeatherScreen    | Presentation and UI logic                        |
| DisplayManager   | Display hardware abstraction                     |
| WifiSetup        | WiFi lifecycle management                        |
| OtaManager       | OTA update lifecycle                             |
| SystemManager    | Hostname and system helper functions             |

This architecture is a significant improvement over the original single-file implementation and provides a solid foundation for future expansion.

---

# Configuration

Most user-customisable settings are intentionally centralised.

Examples include:

* MQTT topics
* Timezone settings
* NTP servers
* Device hostname prefix
* Display behaviour
* Feature toggles

The long-term goal is to make adapting the project to a different MQTT environment require minimal code changes.

---

# OTA Updates

The display supports wireless firmware updates using ArduinoOTA.

Once connected to WiFi, PlatformIO can upload new firmware directly over the network without requiring a USB connection.

---

# Roadmap

## Near-Term

* Move MQTT topics into dedicated configuration modules
* Introduce screen configuration abstraction
* Improve sensor definition flexibility
* Add diagnostic/status screen
* Add touchscreen navigation

## Mid-Term

* Multiple screen support
* Dashboard layouts
* User-configurable display options
* Runtime configuration improvements

## Long-Term

* Generic MQTT dashboard framework
* Web-based status interface
* Low-power sensor ecosystem
* Battery-powered remote nodes
* Additional sensor types and data sources

---

# Contributing

Contributions, suggestions, and architectural discussions are always welcome.

The project intentionally balances embedded efficiency with maintainable software design and aims to remain approachable for hobbyists while following sound engineering practices.

---

# Acknowledgements

This project builds upon the excellent work of the open-source community, particularly the maintainers of:

* PlatformIO
* ESP8266 Arduino Core
* MiniGrafx
* WiFiManager
* ArduinoOTA
* EspMQTTClient

Thank you to everyone who contributes to these projects and helps make DIY embedded development accessible.
