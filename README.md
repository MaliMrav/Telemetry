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

At the same time, the codebase became a software engineering exercise in designing maintainable embedded systems using modern tooling and architectural practices.

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

This version is the result of that refactor journey — transforming from a single-purpose Arduino sketch into a reusable embedded architecture that balances efficiency with maintainability while remaining approachable for hobbyists and makers.

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

Advantages include:

- reduced RAM consumption
- lower CPU utilisation
- simpler firmware
- centralised business logic
- easier experimentation

As the project evolves, upstream systems may also provide weather forecasts, rain probability, multi-day outlooks, historical trend analysis, and sensor aggregation.

The long-term vision is to keep the display focused on presentation while upstream systems perform the computation.

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

### Input flow

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

The main dashboard screen. It presents:

- Current time with 12/24-hour toggle (tap header to switch)
- Date display
- WiFi signal strength indicator
- Sensor tile grid showing:
  - Current value
  - Daily min/max
  - Trend arrows (up/down/flat)
  - Color-coded by sensor type

WeatherScreen reads from `SensorRepository` only — it has no knowledge of where data came from (MQTT, HTTP, local sensor, etc.).

### CalibrationScreen

A guided touch calibration workflow. It:

- Collects four corner touch points
- Calculates calibration coefficients
- Saves coefficients to filesystem
- Provides visual feedback during the process
- Automatically transitions to WeatherScreen when complete

Calibration can be forced via `config_override.h` or triggered automatically if no saved calibration exists.

### BootScreen

A boot progress screen that provides visual feedback during firmware initialisation:

- Displays progress bar
- Shows current boot stage
- Reports percentage complete
- Helps diagnose boot failures

Boot stages include:
- Filesystem mount
- WiFi connection
- Time synchronisation
- Sensor initialisation
- OTA setup
- Data source connection

### StatusScreen

A work-in-progress status and diagnostics screen intended to show:

- WiFi signal strength details
- MQTT connection state
- Firmware version
- System uptime
- IP address
- Memory usage

Input handlers are wired; rendering implementation is next.

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

**To configure your own hardware:**
1. Copy `src/config/secrets.example.h` to `src/config/secrets.h` and fill in WiFi/MQTT/OTA credentials
2. Open `src/config/config_override.h` and set your timezone, display type, and pin assignments
3. Build and flash

See [docs/DISPLAY_CONFIGURATION.md](docs/DISPLAY_CONFIGURATION.md) for display-specific setup.
See [docs/DATA_LAYER_ARCHITECTURE.md](docs/DATA_LAYER_ARCHITECTURE.md) for adding new data sources.

---

## OTA Updates

The display supports wireless firmware updates using ArduinoOTA.

Once connected to WiFi, PlatformIO can upload new firmware directly over the network without a USB connection, which makes ongoing development and maintenance much easier.

---

## How the Project Evolved

### Early version (Arduino IDE)

```text
Single .ino file
├─ WiFi setup
├─ MQTT callbacks
├─ Display drawing
├─ OTA handling
└─ Configuration
```

Everything lived in one file. Adding features meant scrolling through hundreds of lines. Changing one thing risked breaking another.

### Current version (PlatformIO + modular architecture)

```text
main.cpp
├─ DisplayManager
├─ TouchController
├─ TouchManager
├─ ScreenManager
│   ├─ BootScreen
│   ├─ WeatherScreen
│   ├─ CalibrationScreen
│   └─ StatusScreen (in progress)
├─ InputManager
├─ SystemManager
├─ OtaManager
├─ MqttDataSource (implements IDataSource)
└─ SensorRepository
```

Each component has a single responsibility. `main.cpp` is now just 60 lines of object construction and delegation.

That shift represents the biggest change in the project:

- From a single-purpose sketch to a reusable embedded architecture
- From tightly coupled code to dependency injection and interfaces
- From implicit dependencies to explicit construction
- From global state to managed references

The architecture now demonstrates:

- **Single Responsibility** — each module owns one concern
- **Dependency Inversion** — `SystemManager` depends on `IDataSource`, not `MqttDataSource`
- **Open/Closed** — extend with new screens or data sources without modifying existing code
- **Testability** — no globals, explicit dependencies, mockable interfaces

---

## Roadmap

### Completed

- ✅ Modular architecture with clear separation of concerns
- ✅ Input abstraction layer supporting multiple sources
- ✅ Touch calibration workflow with filesystem persistence
- ✅ Boot progress screen with visual feedback
- ✅ Weather dashboard with sensor tiles, trends, and min/max
- ✅ 12/24-hour clock toggle
- ✅ Data source abstraction (IDataSource interface)
- ✅ MQTT data source implementation
- ✅ Touch input optional (can be disabled via config)
- ✅ OTA wireless updates

### Near-term

- Complete StatusScreen rendering (input handlers done)
- Implement screen navigation (previous/next)
- Add multi-page support for large sensor sets
- Introduce rotary encoder input source
- Add physical button input source

### Mid-term

- SettingsScreen implementation
- Configurable tile layouts
- Forecast screen with weather icons
- Runtime display preferences (save to filesystem)
- Data staleness tracking and indication
- Change notifications for efficient redraws

### Long-term

- Generic MQTT dashboard framework
- Browser-based configuration interface
- Web-based status and monitoring
- ESP32 support and testing
- Integrated dev board support (CYD, M5Stack, LilyGO)
- Alternative display drivers (SSD1306 OLED, ST7735 TFT)
- Battery-friendly sensor node ecosystem
- Multiple simultaneous data sources (MQTT + local I2C sensors)

---

## Future Sensor Nodes

One long-term goal is to replace older always-on display sensor nodes with low-power MQTT publishing devices.

The objective is to:

- eliminate continuous display power draw
- extend battery life dramatically (months or years on rechargeable batteries)
- reduce hardware complexity
- publish directly to MQTT

The display dashboard then becomes the main interface for the whole ecosystem, with sensor nodes operating at microamp-level power consumption while sleeping.

---

## Project History

This project was previously known as **ESP8266_TFT_MiniGrafx_Weather_UI**.

The rebrand to **Telemetry** reflects its evolution from a weather-specific display into a general-purpose embedded dashboard framework suitable for any MQTT-driven telemetry application.

See `README v1.0.md` for the original documentation from the earlier architecture.

---

## Contributing

Contributions, ideas, architectural discussions, and feature suggestions are welcome.

This project intentionally balances:

- **Embedded efficiency** — constrained hardware, minimal overhead
- **Maintainable design** — SOLID principles, clear boundaries
- **Approachability** — documented patterns, teaching-focused code

Whether you're a hobbyist learning embedded development or an experienced engineer exploring ESP architecture patterns, your input is valued.

---

## Acknowledgements

Special thanks to the maintainers and contributors of:

- PlatformIO
- ESP8266 Arduino Core
- MiniGrafx
- WiFiManager
- ArduinoOTA
- PubSubClient

Without these projects, this framework would not exist.

