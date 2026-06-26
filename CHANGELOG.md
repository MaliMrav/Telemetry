# Changelog

All notable changes to this project will be documented in this file.

---

## [2.0.0] - 2026-06-26 - Telemetry Framework

### Project Rebrand

**ESP8266_TFT_MiniGrafx_Weather_UI** → **Telemetry**

The project has been rebranded to reflect its evolution from a weather-specific display into a general-purpose embedded telemetry dashboard framework.

### Major Architectural Changes

#### Complete Modular Refactor

- Transformed from monolithic Arduino sketch to modular PlatformIO project
- Introduced clear separation of concerns across 11 major subsystems
- Reduced `main.cpp` from hundreds of lines to ~60 lines of construction and delegation
- Implemented dependency injection throughout the codebase

#### Input System Redesign

- Created `InputManager` as central input hub
- Abstracted input sources via `IInputSource` interface
- Implemented `InputEvent` queuing system
- Built `TouchManager` to translate hardware touches into logical input events
- Made touch input fully optional via configuration

#### Screen Management

- Implemented `ScreenManager` for screen lifecycle and routing
- Created `Screen` base class with consistent interface
- Added screen transition support with enter/leave callbacks
- Introduced touch profile switching per screen

#### Data Layer

- Abstracted data sources via `IDataSource` interface
- Implemented `MqttDataSource` as concrete implementation
- Created `SensorRepository` as single source of truth
- Separated data acquisition from presentation

#### System Lifecycle

- Built `SystemManager` to orchestrate boot sequence
- Implemented structured boot progress reporting
- Added filesystem initialisation with fallback formatting
- Integrated all subsystems through a single entry point

### New Features

#### Screens

- **BootScreen** - Visual boot progress with status messages and progress bar
- **CalibrationScreen** - Four-point touch calibration workflow with visual feedback
- **WeatherScreen** - Enhanced with 12/24-hour toggle, color-coded tiles, trend arrows
- **StatusScreen** - Input handlers implemented (rendering in progress)

#### Calibration System

- Four-corner calibration workflow
- Automatic coefficient calculation
- LittleFS persistence
- Force calibration mode via config
- Auto-skip if valid calibration exists

#### Configuration System

- Centralised configuration in `config/` directory
- Display-specific configs in `config/display/`
- Secrets management via `secrets.h` (git-ignored)
- Override mechanism via `config_override.h`
- Hardware feature flags (touch enable/disable)

#### Input Features

- Multi-source input aggregation
- Event queuing (8-event FIFO)
- Touch zone detection
- Debouncing and validation
- Future-ready for encoders, buttons, remote input

### Technical Improvements

- Moved from EspMQTTClient to PubSubClient
- Implemented LittleFS for filesystem operations
- Added proper touch calibration storage
- Created reusable display driver abstraction
- Documented architecture with DATA_LAYER_ARCHITECTURE.md
- Documented display porting with DISPLAY_CONFIGURATION.md

### Breaking Changes

- Complete project structure reorganization
- Configuration moved from `settings.h` to `config/` directory
- Different MQTT library requires connection handling changes
- Touch calibration format changed
- Screen interface completely redesigned

---

## [1.0.0] - Original Arduino IDE Version

### Initial Implementation

- Single-file Arduino IDE sketch
- ESP8266 + ILI9341 display support
- MQTT weather data display
- WiFi management via WiFiManager
- OTA update support
- Time and date display
- Multiple sensor tile layout
- Basic trend indicators

### Original Features

- Kitchen and Pergola sensor monitoring
- Temperature, humidity, pressure display
- Daily min/max tracking (calculated by Home Assistant)
- WiFi signal strength indicator
- Auto-reconnect for WiFi and MQTT

### Libraries Used

- ESP8266 Arduino Core
- MiniGrafx
- EspMQTTClient
- WiFiManager
- ArduinoOTA

---

## Versioning Philosophy

This project uses semantic versioning:

- **MAJOR** - Architectural changes, breaking API changes
- **MINOR** - New features, new screens, backward-compatible additions
- **PATCH** - Bug fixes, documentation updates, minor improvements

The jump from 1.0 to 2.0 represents the complete architectural refactor and rebrand.
