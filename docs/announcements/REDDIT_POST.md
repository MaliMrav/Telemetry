# Reddit Post - r/esp8266, r/homeassistant, r/homeautomation

## Title
Telemetry: Open-source MQTT dashboard framework for ESP8266/ESP32 with modular architecture

## Body

I've just released **Telemetry v2.0** - an open-source framework for building MQTT-driven dashboards on ESP8266/ESP32 microcontrollers.

[**Photo of WeatherScreen goes here**]

### What It Is

Telemetry started as a simple weather display for my BME280 sensor nodes but evolved into a reusable embedded framework that demonstrates clean architecture on constrained hardware.

It's designed to be:
- **Learned from** - documented patterns, teaching-focused code
- **Extended** - add screens or data sources without modifying core code  
- **Ported** - works on ESP8266, ESP32, and various displays
- **Maintained** - modular structure that scales beyond single-file sketches

### Key Features

**Current implementation:**
- 📊 Real-time sensor dashboard with trend indicators
- 🎯 Touch calibration workflow with filesystem persistence
- 🔄 Boot progress screen with visual feedback
- ⏰ 12/24-hour clock toggle
- 📡 OTA wireless updates
- 🔌 Optional touch support (works without touchscreen)

**Architecture highlights:**
- Modular design - 11 subsystems with clear boundaries
- Data source abstraction - swap MQTT for HTTP/Serial/I2C without changing screens
- Input abstraction - ready for rotary encoders, buttons, remote control
- Dependency injection throughout
- SOLID principles applied to embedded code

### Design Philosophy

The display is intentionally a **presentation device**. Rather than calculating min/max/trends locally, it consumes pre-processed data from upstream systems (like Home Assistant). This keeps the ESP8266 lightweight and focused on what it does best: displaying information reliably.

### Hardware

**Current reference:**
- ESP8266 (ESP-12E)
- 2.4" ILI9341 TFT (240×320)
- XPT2046 touch controller
- MQTT-connected BME280 sensors

**Designed for:**
- ESP32 support (coming soon)
- CYD/M5Stack/LilyGO boards
- Alternative displays (OLED, other TFTs)
- Multiple input devices

### From 500 Lines to 60

The original version was a single Arduino IDE sketch - everything in one file. As features accumulated, maintenance became difficult.

Version 2.0 is a complete rewrite:
- Migrated from Arduino IDE → PlatformIO
- Modular architecture with dependency injection
- `main.cpp` is now just 60 lines
- Each component has a single, well-defined responsibility

[**Photo of architecture diagram or code structure goes here**]

### Tech Stack

- PlatformIO + VSCode
- ESP8266/ESP32 Arduino Core  
- MiniGrafx (display abstraction)
- PubSubClient (MQTT)
- ArduinoOTA
- WiFiManager

### Documentation

The project includes:
- Architecture guide (DATA_LAYER_ARCHITECTURE.md)
- Display porting guide (DISPLAY_CONFIGURATION.md)
- Complete changelog documenting the evolution

### Get Involved

Whether you're learning embedded development or exploring ESP architecture patterns, contributions and discussions are welcome!

**GitHub:** [Add your GitHub URL here]

**License:** MIT

---

### Comments Welcome

I'm happy to answer questions about:
- Architecture decisions and trade-offs
- Porting to different hardware
- Adding new data sources
- Embedded design patterns
- Home Assistant integration

---

*Previously known as ESP8266_TFT_MiniGrafx_Weather_UI - rebranded to reflect its evolution into a general-purpose telemetry framework.*
