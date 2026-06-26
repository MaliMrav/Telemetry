# GitHub Release v2.0.0 - Telemetry Framework

## 🎉 Major Release: Project Rebrand and Complete Architectural Rewrite

**ESP8266_TFT_MiniGrafx_Weather_UI** → **Telemetry**

This release represents a complete transformation from a single-purpose weather display into a general-purpose embedded telemetry framework.

[**Add photo of display here**]

---

## 🚀 What's New

### Complete Modular Architecture

- ✅ **11 subsystems** with clear boundaries and responsibilities
- ✅ **Dependency injection** throughout - no globals, testable components
- ✅ **60-line main.cpp** - down from 500+ line monolithic sketch
- ✅ **SOLID principles** applied to embedded development

### New Screens

#### BootScreen
Visual boot progress with status messages and progress bar
- Shows current initialization stage
- Reports percentage complete
- Helps diagnose boot failures

#### CalibrationScreen  
Guided four-point touch calibration workflow
- Collects corner touch points
- Calculates transformation coefficients
- Saves to LittleFS for persistence
- Auto-transitions to WeatherScreen when complete

#### StatusScreen (In Progress)
Diagnostic and status information
- Input handlers implemented
- Rendering implementation next

#### WeatherScreen (Enhanced)
Main dashboard with new features
- 12/24-hour clock toggle (tap header)
- Color-coded sensor tiles
- Trend arrows (up/down/flat)
- Daily min/max display
- WiFi signal strength indicator

### Input System Redesign

- ✅ **InputManager** - central input hub with event queue
- ✅ **IInputSource** interface - abstraction for multiple input types
- ✅ **TouchManager** - translates hardware touches to logical events
- ✅ **Touch optional** - works without touchscreen via config flag
- ✅ **Future-ready** - prepared for rotary encoders, buttons, remote input

### Data Layer Architecture

- ✅ **IDataSource** interface - swap MQTT/HTTP/Serial without changing screens
- ✅ **MqttDataSource** implementation with auto-reconnect
- ✅ **SensorRepository** - single source of truth for all sensor data
- ✅ **TopicMappings** - declarative MQTT topic configuration

### Configuration System

- ✅ Centralized configuration in `src/config/`
- ✅ Display-specific configs in `src/config/display/`
- ✅ Secrets management via `secrets.h` (git-ignored)
- ✅ Override system via `config_override.h`
- ✅ Hardware feature flags

### System Management

- ✅ **SystemManager** orchestrates boot sequence
- ✅ **OtaManager** for wireless firmware updates
- ✅ **ScreenManager** handles screen lifecycle and routing
- ✅ **LittleFS** for filesystem operations
- ✅ **WiFiManager** captive portal setup

---

## 📚 Documentation

### New Documentation
- ✅ **CHANGELOG.md** - Complete project evolution history
- ✅ **DATA_LAYER_ARCHITECTURE.md** - Guide for adding data sources
- ✅ **DISPLAY_CONFIGURATION.md** - Guide for porting to different displays
- ✅ **README.md** - Comprehensive project documentation

### Announcement Materials
- Reddit post template
- Hackaday project submission
- Twitter/X thread
- Long-form blog post

---

## 🔧 Technical Stack

**Development:**
- Visual Studio Code
- PlatformIO

**Core Libraries:**
- ESP8266 Arduino Core (ESP32 compatible)
- MiniGrafx (display driver abstraction)
- PubSubClient (MQTT client)
- ArduinoOTA (wireless updates)
- WiFiManager (captive portal)
- LittleFS (filesystem)

---

## 🖥️ Supported Hardware

**Current Reference:**
- ESP8266 (ESP-12E)
- 2.4" ILI9341 TFT (240×320)
- XPT2046 touch controller
- MQTT-connected BME280 sensors

**Designed For:**
- ESP32 support (coming soon)
- CYD / M5Stack / LilyGO boards
- Alternative displays (OLED, other TFTs)
- Multiple input devices

---

## 💥 Breaking Changes

⚠️ This is a **complete rewrite**. Version 1.0 code is not compatible.

- Project structure completely reorganized
- Configuration system redesigned
- MQTT library changed (EspMQTTClient → PubSubClient)
- Screen interface redesigned
- Touch calibration format changed

**Migration:** Not supported. Treat as new installation.

---

## 🎯 Design Philosophy

**The display is a presentation device.**

Rather than calculating min/max/trends locally, the ESP8266 consumes pre-processed data from upstream systems (Home Assistant). This keeps the firmware:

- Lightweight (minimal RAM usage)
- Efficient (low CPU utilization)
- Responsive (fast updates)
- Simple (easier to maintain and debug)

---

## 🗺️ Roadmap

### Near-term
- Complete StatusScreen rendering
- Screen navigation (previous/next)
- Multi-page support
- Rotary encoder input
- Physical button support

### Mid-term
- SettingsScreen implementation
- Forecast screen with weather icons
- Configurable tile layouts
- Data staleness tracking
- Change notifications

### Long-term
- Generic MQTT dashboard framework
- Web configuration interface
- ESP32 support
- Additional display drivers
- Battery-powered sensor nodes

---

## 📦 Installation

1. Clone the repository
2. Copy `src/config/secrets.example.h` to `src/config/secrets.h`
3. Fill in WiFi/MQTT credentials in `secrets.h`
4. Configure timezone and pins in `src/config/config_override.h`
5. Build and flash using PlatformIO

See [DISPLAY_CONFIGURATION.md](docs/DISPLAY_CONFIGURATION.md) for hardware-specific setup.

---

## 🤝 Contributing

Contributions welcome! Whether you're:
- Learning embedded development
- Exploring ESP architecture patterns
- Adding hardware support
- Improving documentation

Your input is valued.

---

## 📄 License

MIT License - See LICENSE.md

---

## 🙏 Acknowledgements

Special thanks to the maintainers and contributors of:
- PlatformIO
- ESP8266 Arduino Core
- MiniGrafx
- WiFiManager
- ArduinoOTA
- PubSubClient

---

## 📸 Screenshots

[**Add screenshots here:**]
- WeatherScreen with live data
- BootScreen showing progress
- CalibrationScreen workflow
- Architecture diagram

---

**Full Changelog:** v1.0.0...v2.0.0
