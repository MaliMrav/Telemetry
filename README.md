# Telemetry

**Embedded telemetry framework for ESP microcontrollers**

> An architecture-focused framework for building modular, maintainable MQTT-driven dashboards on ESP8266, ESP32, and compatible hardware.
>
> What began as a weather display has evolved into a reusable embedded UI framework that demonstrates clean separation of concerns, hardware abstraction, and extensible design patterns — all running on constrained microcontroller hardware.

---

## What This Is

Telemetry is a framework for building embedded dashboards that visualize sensor data, IoT telemetry, or any MQTT-published information.

It's designed to be:
- **Learned from** — clear architecture, documented patterns, teaching-focused code
- **Extended** — add new screens, data sources, or input devices without modifying core code
- **Ported** — works on ESP8266, ESP32, and integrated solutions like CYD displays
- **Maintained** — modular structure that scales beyond a single-file sketch

**Current reference implementation:** Weather dashboard on ESP8266 with ILI9341 TFT display and XPT2046 touch.

---

## Why Telemetry Exists

This project started with a practical problem.

I had multiple ESP8266-based BME280 sensor nodes around the house, each with a small OLED display. They worked, but OLED burn-in and continuous power use made them a poor long-term fit, especially for future battery-powered nodes.

The initial goal was simple:

- collect environmental data from multiple locations around the property
- publish the data via MQTT
- present it on a single dedicated display

The first monitored locations were:

- Kitchen (inside)
- Pergola (outside)

Over time, the project grew from a weather display into something larger: a reusable MQTT-driven embedded dashboard framework capable of showing many kinds of data.

---

## Development Journey

The first version was built in Arduino IDE as a single-file proof of concept.

As features accumulated, maintenance became harder. Display logic, MQTT handling, OTA updates, WiFi management, and configuration all lived together in one application file.

A conscious decision was made to modernise the project and move to:

- Visual Studio Code
- PlatformIO
- Modular architecture
- Source-control-friendly configuration
- Separation of concerns
- Reusable components

The goal was not just to reorganise files, but to build a codebase that could keep growing without becoming difficult to understand or maintain.

This version is the result of that refactor journey.

---

## Design Philosophy

A key architectural decision was made early:

**The display should remain a presentation device.**

Rather than doing complex calculations locally, the ESP8266 consumes already-processed information published to MQTT topics.

That keeps the firmware:

- lightweight
- efficient
- responsive
- easier to maintain

In practice, upstream systems such as Home Assistant provide much of the heavy lifting.

Examples include:

- daily minimum calculations
- daily maximum calculations
- trend determination
- topic publishing

So the display subscribes to values like:

```text
temperature
temperature_min
temperature_max
temperature_trend
```

instead of calculating them itself.

That means less RAM usage, lower CPU load, and simpler firmware.

---

## Supported Hardware

**Current reference platform:**
- **Microcontroller:** ESP8266 (ESP-12E)
- **Display:** 2.4" ILI9341 TFT (240×320, SPI)
- **Touch:** XPT2046 resistive touch controller
- **Data sources:** MQTT-connected BME280 sensor nodes

**Designed to support:**
- ESP32, ESP32-S2/S3/C3 (future)
- Integrated dev boards (CYD, M5Stack, LilyGO) (future)
- Alternative displays (SSD1306 OLED, ST7735 TFT) via driver config
- Multiple input devices (rotary encoder, physical buttons, capacitive touch)

See [docs/DISPLAY_CONFIGURATION.md](docs/DISPLAY_CONFIGURATION.md) for porting to different displays.

---

## Software Stack

Development environment:

- Visual Studio Code
- PlatformIO

Core libraries:

- ESP8266 Arduino Core (ESP32 compatible)
- MiniGrafx (display driver abstraction)
- PubSubClient (MQTT client)
- ArduinoOTA (wireless firmware updates)
- WiFiManager (captive portal WiFi setup)

Supporting pieces:

- custom timezone helper
- custom fonts
- weather icon resources

---

## Architecture

### Current flow

```text
TouchController
    ↓
TouchManager
    ↓
InputManager
    ↓
InputEvent
    ↓
ScreenManager
    ↓
Current Screen
    ↓
DisplayManager
```

### Why this matters

- `TouchController` talks to hardware
- `TouchManager` translates physical touch into input events
- `InputManager` aggregates input from multiple sources
- `ScreenManager` routes events to the active screen
- each screen decides what an input means in its own context

That separation makes it easy to add future input sources such as:

- rotary encoders
- physical buttons
- keyboards
- mouse-like devices
- remote or network-triggered actions

### Current project structure

```text
src/
├── config/          Configuration system
│   └── display/     Display driver configs
├── data/            Data source abstraction
├── display/         Display manager
├── input/           Input event pipeline
├── models/          Data models and repository
├── mqtt/            MQTT data source
├── ota/             OTA update manager
├── screens/         UI screens
├── system/          System lifecycle manager
├── touch/           Touch hardware controller
├── ui/              Screen manager and base classes
└── wifi/            WiFi connection manager
```

---

## Screens

### WeatherScreen

The main dashboard screen. It presents time, WiFi strength, and sensor tiles.

### CalibrationScreen

A guided touch calibration workflow. It collects four raw points, calculates calibration coefficients, and saves them for future boots.

### BootScreen

A boot progress screen that provides visual feedback while the firmware initialises.

---

## Configuration

Configuration is intentionally centralised to keep the project easy to adapt.

Examples include:

- MQTT broker configuration
- hostname prefix
- timezone settings
- NTP server
- display pins
- hardware feature flags

The goal is for the firmware to be easy to clone and adapt with minimal source changes.

### Hardware flags

The `config/HardwareConfig.h` file defines what the target hardware includes, for example:

- display present or not
- touch present or not
- buttons present or not
- encoder present or not
- capacitive vs resistive touch

This is what keeps the project reusable across different hardware combinations.

---

## OTA Updates

The display supports wireless firmware updates using ArduinoOTA.

Once connected to WiFi, PlatformIO can upload new firmware directly over the network without a USB connection, which makes ongoing development and maintenance much easier.

---

## How the Project Evolved

### Early version

```text
Arduino sketch
├─ WiFi
├─ MQTT
├─ display drawing
├─ OTA
└─ configuration
```

### Current version

```text
main.cpp
├─ SystemManager
├─ InputManager
├─ TouchManager
├─ ScreenManager
├─ DisplayManager
├─ WeatherScreen
├─ CalibrationScreen
└─ SensorRepository
```

That shift is the biggest change in the project so far.

It is now closer to a reusable embedded architecture than a single-purpose sketch.

---

## Roadmap

### Near-term

- finalise calibration workflow
- keep touch optional
- introduce additional input sources
- continue shrinking `main.cpp`
- improve screen switching and navigation

### Mid-term

- settings screen
- status screen
- 12/24-hour preferences
- forecast screen
- weather icons
- configurable layouts

### Long-term

- generic MQTT dashboard framework
- browser-based configuration
- remote status view
- additional display types
- battery-friendly sensor node ecosystem

---

## Future Sensor Nodes

One long-term goal is to replace older always-on display sensor nodes with low-power MQTT publishing devices.

The objective is to:

- eliminate continuous display power draw
- extend battery life
- reduce hardware complexity
- publish directly to MQTT

The display dashboard then becomes the main interface for the whole ecosystem.

---

## Contributing

Contributions, ideas, architectural discussions, and feature suggestions are welcome.

This project tries to balance embedded efficiency with maintainable software design while staying approachable for hobbyists and makers.

---

## Acknowledgements

Special thanks to the maintainers and contributors of:

- PlatformIO
- ESP8266 Arduino Core
- MiniGrafx
- WiFiManager
- ArduinoOTA
- EspMQTTClient

