# Hackaday Post / Project Submission

## Title
Telemetry: Modular MQTT Dashboard Framework for ESP Microcontrollers

## Summary (Short)
Architecture-focused embedded framework for building MQTT-driven dashboards on ESP8266/ESP32. Clean separation of concerns, dependency injection, and extensible design - all on constrained hardware.

## Full Description

[**Hero photo of the display goes here**]

### The Problem

I had multiple ESP8266-based BME280 sensor nodes around the house, each with its own OLED display. The displays suffered from burn-in and weren't practical for always-on operation or battery-powered deployments.

The solution? A single, dedicated MQTT dashboard that consumes data from all sensors.

But rather than building just another weather display, I decided to treat it as a software architecture exercise: **How do you build maintainable, extensible embedded systems on 80MHz hardware with 80KB of RAM?**

### From Sketch to Framework

**Version 1.0** was a typical Arduino IDE single-file sketch. WiFi setup, MQTT callbacks, display rendering, OTA updates, and configuration - all in one file. It worked, but adding features became progressively harder.

**Version 2.0** is a complete architectural rewrite:

```
Before: 500+ line monolithic sketch
After: 11 modular subsystems, main.cpp = 60 lines
```

[**Code structure comparison photo/diagram goes here**]

### Architecture Highlights

**Separation of Concerns:**
- `DisplayManager` - hardware abstraction
- `TouchManager` - translates raw touch into logical events
- `InputManager` - aggregates multiple input sources
- `ScreenManager` - lifecycle and routing
- `SystemManager` - orchestrates the boot sequence
- `SensorRepository` - single source of truth for data
- `MqttDataSource` - implements `IDataSource` interface

**Key Design Decisions:**

1. **Dependency Injection** - No globals. All dependencies passed explicitly. Testable.

2. **Interface Segregation** - `IDataSource` abstraction means you can swap MQTT for HTTP, Serial, or local I2C sensors without touching screen code.

3. **Input Abstraction** - `InputManager` accepts events from any source. Touch works today. Rotary encoder support? Add a class that implements `IInputSource`. Done.

4. **Presentation-only Display** - The ESP8266 doesn't calculate daily min/max or trends. It consumes pre-processed data from Home Assistant via MQTT. Less RAM, simpler code, centralized logic.

### Current Features

- **WeatherScreen** - sensor tiles with values, min/max, trend arrows, 12/24-hour clock
- **BootScreen** - visual boot progress with status messages
- **CalibrationScreen** - four-point touch calibration with filesystem persistence
- **StatusScreen** - diagnostic screen (input handlers done, rendering next)

[**Calibration screen photo goes here**]

### Tech Stack

- **Platform:** PlatformIO (goodbye Arduino IDE)
- **Display:** MiniGrafx library (driver abstraction layer)
- **MQTT:** PubSubClient
- **Filesystem:** LittleFS
- **OTA:** ArduinoOTA
- **WiFi:** WiFiManager (captive portal setup)

### Hardware

**Reference build:**
- ESP8266 (ESP-12E)
- 2.4" ILI9341 TFT (240×320)
- XPT2046 touch controller

**Designed to support:**
- ESP32 variants
- CYD (Cheap Yellow Display)
- M5Stack / LilyGO boards
- OLED displays (SSD1306)
- Alternative TFTs (ST7735)
- Touch-free operation

### Configuration

Everything centralised in `src/config/`:
- `config_override.h` - timezone, pins, display type
- `secrets.h` - WiFi/MQTT credentials (git-ignored)
- `display/ILI9341_config.h` - driver-specific settings

Clone, configure three files, flash. Done.

### Documentation

This isn't just code - it's teaching material.

- **DATA_LAYER_ARCHITECTURE.md** - how to add new data sources
- **DISPLAY_CONFIGURATION.md** - porting to different displays
- **CHANGELOG.md** - complete evolution history
- **Inline documentation** - every module explains its purpose

### Roadmap

**Near-term:**
- Complete StatusScreen rendering
- Multi-page navigation
- Rotary encoder input
- Physical button support

**Mid-term:**
- SettingsScreen for runtime configuration
- Weather forecast screen
- Configurable tile layouts
- Data staleness indicators

**Long-term:**
- Generic MQTT dashboard (not just weather)
- Web-based configuration interface
- ESP32 support and testing
- Battery-powered sensor node ecosystem

### Project Evolution

Originally named **ESP8266_TFT_MiniGrafx_Weather_UI** - rebranded to **Telemetry** to reflect its evolution from weather-specific to general-purpose telemetry framework.

### Why This Matters

Most ESP8266 projects are proof-of-concepts that never escape the single-file sketch phase. This project demonstrates that you **can** apply professional software engineering practices to embedded systems without sacrificing efficiency.

It's a learning resource as much as it is a functional framework.

### Open Source

**License:** MIT  
**GitHub:** [Add your URL here]  
**Contributions:** Welcome!

Whether you're learning embedded development, exploring architecture patterns, or just need an MQTT dashboard - the code is yours to use, learn from, and improve.

---

### Build Photos

[**Add multiple photos here:**]
- Display running with live data
- Boot sequence
- Calibration workflow
- Hardware assembly (optional)
- Architecture diagram

---

**Tags:** ESP8266, ESP32, MQTT, PlatformIO, Home Assistant, IoT, Embedded Systems, Software Architecture, TFT Display
