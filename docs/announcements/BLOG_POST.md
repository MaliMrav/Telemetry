# Blog Post / Long-form Article

## Title
Telemetry v2.0: Building a Maintainable Embedded Framework on ESP8266

## Subtitle
How a weather display sketch evolved into an architecture-focused IoT framework that demonstrates SOLID principles on constrained hardware

---

[**Hero image: Display running with live data**]

## Introduction

When you search for ESP8266 projects online, you'll find thousands of Arduino sketches. Most are single-file proofs of concept - WiFi setup, sensor reading, display rendering, all mixed together in `loop()` and `setup()`. They work, but they don't scale.

What happens when you want to add a new screen? A different data source? Support for hardware without touch input? You're usually rewriting significant portions of tightly coupled code.

**Telemetry v2.0** is my answer to that problem. It's an embedded dashboard framework that treats maintainability, extensibility, and clean architecture as first-class concerns - without sacrificing the efficiency required for microcontroller hardware.

## The Origin Story

### The Problem

I had multiple ESP8266-based BME280 sensor nodes around my house, each with a small OLED display showing temperature, humidity, and pressure. They worked fine initially, but two problems emerged:

1. **OLED burn-in** - Displaying the same information 24/7 caused visible degradation
2. **Power consumption** - Always-on displays made battery operation impractical

The solution seemed obvious: centralize the display. All sensors publish to MQTT, one dedicated display consumes the data.

### Version 1.0: The Arduino Sketch

The first version was a typical Arduino IDE sketch. One `.ino` file containing:

- WiFi setup and reconnection logic
- MQTT connection handling
- Subscription management
- Message parsing
- Display initialization
- Screen rendering
- Time synchronization
- OTA update handling
- Configuration constants

It was about 500 lines of code. It worked. But as I added features, maintenance became progressively harder.

Want to add a calibration screen? Scroll through global state to figure out what affects what.

Want to swap MQTT for HTTP polling? Good luck untangling the callbacks from the display logic.

Want to support operation without touch? Better add more conditionals throughout the code.

### The Decision to Refactor

I made a conscious decision: treat this as a **software engineering exercise**.

The question wasn't "how do I add feature X?" but rather "how do I build an embedded system that can grow without collapsing under its own complexity?"

## Architecture Philosophy

### Principle 1: Separation of Concerns

Each module should have **one job** and do it well.

In Version 1.0, everything was everyone's problem. The MQTT callback modified display state. The display rendering function checked WiFi status. Configuration was scattered across the file.

In Version 2.0, boundaries are explicit:

- `DisplayManager` owns the hardware framebuffer and rendering
- `MqttDataSource` handles broker connection and topic subscriptions
- `SensorRepository` is the single source of truth for sensor data
- `ScreenManager` manages screen lifecycle and routing
- `SystemManager` orchestrates the boot sequence

No module reaches into another's internals.

### Principle 2: Dependency Inversion

High-level modules should not depend on low-level modules. Both should depend on abstractions.

The key abstraction is `IDataSource`:

```cpp
class IDataSource {
public:
    virtual void begin() = 0;
    virtual void loop()  = 0;
};
```

`SystemManager` doesn't know about MQTT, HTTP, or Serial. It holds a reference to `IDataSource`. The concrete implementation (`MqttDataSource`) is injected at construction.

Want to add HTTP polling? Implement `IDataSource`. Want local I2C sensors? Implement `IDataSource`. Want both simultaneously? Register multiple sources with a future `DataSourceManager`.

The screens never change. They read from `SensorRepository`. They don't care where the data came from.

### Principle 3: Open/Closed

Software entities should be open for extension, closed for modification.

Adding a new screen means:
1. Create a class that inherits from `Screen`
2. Implement `update()` and `onInput()`
3. Pass it to `ScreenManager`

You don't modify `ScreenManager`. You don't modify existing screens. You extend the system by adding code, not changing it.

### Principle 4: Explicit Dependencies

No globals. No singletons. Every dependency is passed explicitly.

`main.cpp` constructs all objects and wires them together:

```cpp
DisplayManager display;
TouchController touchController(/*...*/);
TouchManager touchManager(touchController);
ScreenManager screenManager;
OtaManager ota;
MqttDataSource mqttData;

SystemManager::begin(
    display,
    touchController,
    touchManager,
    screenManager,
    bootScreen,
    weatherScreen,
    calibrationScreen,
    ota,
    mqttData);
```

Dependency injection in 60 lines. No hidden coupling. Construction order is explicit and controlled.

## The Module Structure

### Input Pipeline

```
TouchController → TouchManager → InputManager → ScreenManager → Screen
```

**TouchController** talks to XPT2046 hardware. It knows about SPI and raw ADC values. Nothing else.

**TouchManager** translates raw touch coordinates into logical `InputEvent` objects (TAP, LONG_PRESS, SWIPE). It implements `IInputSource`.

**InputManager** maintains a registry of input sources and an event queue. Touch is just one source. Future sources: rotary encoder, physical buttons, remote commands via MQTT.

**ScreenManager** routes events to the active screen. Each screen interprets events in its own context. TAP on WeatherScreen = toggle 12/24 hour. TAP on CalibrationScreen = register calibration point.

This separation makes it trivial to add new input devices. No screen code changes required.

### Data Pipeline

```
MqttDataSource → SensorRepository → WeatherScreen
```

**MqttDataSource** connects to the broker, subscribes to topics, parses payloads, writes to `SensorRepository` via a typed API.

**SensorRepository** owns the array of `SensorTile` objects. It provides setValue(), setMin(), setMax(), setTrend() methods keyed by `SensorId`.

**WeatherScreen** reads via `getTiles()` and renders. It has no knowledge of MQTT, topics, or connection state.

Want to add a new sensor? Change three files:
1. Add `SensorId` enum value
2. Add tile definition to repository
3. Add topic mapping to `TopicMappings`

`MqttDataSource` itself doesn't change.

### Boot Sequence

`SystemManager::begin()` orchestrates initialization:

1. Mount filesystem (LittleFS)
2. Initialize display hardware
3. Initialize touch hardware (if enabled)
4. Show BootScreen
5. Connect WiFi (via WiFiManager captive portal)
6. Sync time (NTP)
7. Initialize sensor repository
8. Start OTA service
9. Start data source (MQTT connection)
10. Load touch calibration or enter calibration mode
11. Transition to WeatherScreen

Each stage reports progress to `BootScreen`, which displays a progress bar and status message. If WiFi fails, you see "Starting WiFi" stuck at 30%. Immediate feedback for debugging.

[**Photo of BootScreen goes here**]

## Current Features

### WeatherScreen

The main dashboard. Displays:

- Current time (tap header to toggle 12/24-hour format)
- Current date with day-of-week
- WiFi signal strength indicator
- Grid of sensor tiles

Each tile shows:
- Sensor label and type-based color coding
- Current value with unit
- Trend arrow (up/down/flat)
- Daily min/max values

The trend arrows are color-coded: yellow for rising, blue for falling, white for stable.

[**Photo of WeatherScreen goes here**]

### CalibrationScreen

Four-point touch calibration workflow. The screen displays crosshairs at each corner sequentially. Touch each target, and the screen calculates transformation coefficients.

Coefficients are saved to LittleFS. On next boot, they're loaded automatically. No re-calibration needed unless you force it via `config_override.h`.

After completion, the screen displays "Calibration saved!" and automatically transitions to WeatherScreen after 2 seconds.

[**Photo of CalibrationScreen goes here**]

### BootScreen

Visual boot feedback. Shows:
- Progress bar (0-100%)
- Current stage name
- Percentage complete

Stages:
- "Mounting filesystem" (10%)
- "Starting WiFi" (30%)
- "Synchronising time" (50%)
- "Loading sensors" (60%)
- "Starting OTA" (70%)
- "Starting data source" (80%)
- "Ready" (100%)

If boot hangs, you know exactly where. No more staring at a blank screen wondering if it crashed or just hasn't connected to WiFi yet.

### StatusScreen (In Progress)

Input handlers are implemented. Rendering is next. Intended to show:
- WiFi signal details
- MQTT connection state
- Firmware version
- System uptime
- IP address
- Free heap memory

## Design Decisions

### Why PlatformIO?

Arduino IDE is great for getting started, but it doesn't scale:
- No IntelliSense / code completion
- No real build system
- No library dependency management
- Difficult to version control

PlatformIO gives you:
- Professional IDE (VS Code)
- Proper build configuration (`platformio.ini`)
- Library dependency management
- Upload via USB or OTA
- Serial monitor integration
- Multiple build environments (debug/serial builds)

### Why MiniGrafx?

Direct TFT library calls are verbose and hardware-specific. MiniGrafx provides:
- Hardware abstraction (swap ILI9341 for ST7735 via config)
- Framebuffer rendering (double-buffering)
- Color palette mode (2-bit color for reduced RAM)
- Consistent API across display types

### Why PubSubClient instead of EspMQTTClient?

EspMQTTClient handles WiFi and MQTT together. Convenient, but opinionated. It owns the connection lifecycle.

PubSubClient is lower-level, which gives me control:
- Separate WiFi management (WiFiManager captive portal)
- Custom reconnection logic
- Manual subscription handling
- Easier to debug connection issues

Trade-off: more code. Benefit: more control and clarity.

### Why Presentation-Only Display?

The ESP8266 could calculate daily min/max values by storing every reading and iterating through them. But that means:
- More RAM consumed by history buffers
- More CPU cycles spent on calculations
- More complex code
- Duplicated logic if multiple displays exist

Instead, Home Assistant does the work:
- Maintains history database
- Calculates statistics
- Publishes to MQTT topics

The display subscribes to:
```
sensor/kitchen/temperature
sensor/kitchen/temperature_min
sensor/kitchen/temperature_max
sensor/kitchen/temperature_trend
```

Result: The ESP8266 RAM usage stays low, the code stays simple, and all displays show consistent data.

## Configuration

Everything is centralized in `src/config/`:

**config_override.h:**
```cpp
#define CFG_TIMEZONE "AEST-10AEDT,M10.1.0,M4.1.0/3"
#define CFG_DISPLAY_DRIVER ILI9341
#define CFG_TFT_DC D4
#define CFG_TFT_CS D2
```

**secrets.h** (git-ignored):
```cpp
#define WIFI_SSID "YourNetwork"
#define WIFI_PASSWORD "YourPassword"
#define MQTT_BROKER "192.168.1.100"
#define MQTT_USERNAME "mqtt_user"
#define MQTT_PASSWORD "mqtt_pass"
```

Clone the repo, copy `secrets.example.h` to `secrets.h`, fill in credentials, flash. Done.

## Roadmap

### Near-term
- Complete StatusScreen rendering
- Implement screen navigation (previous/next)
- Multi-page support for large sensor sets
- Rotary encoder input source
- Physical button input source

### Mid-term
- SettingsScreen for runtime configuration
- Forecast screen with weather icons
- Configurable tile layouts (JSON or YAML config)
- Data staleness tracking (gray out stale sensors)
- Change notifications (redraw only when data changes)

### Long-term
- Generic MQTT dashboard (not just weather/sensors)
- Web-based configuration interface
- ESP32 support and testing
- CYD / M5Stack / LilyGO board support
- OLED display drivers (SSD1306)
- Battery-powered sensor node ecosystem
- Multiple simultaneous data sources

## Lessons Learned

### 1. Refactoring is Worth It

The temptation is always to just "add one more feature" to the existing sketch. But technical debt compounds. The refactor took time, but the result is code I can maintain for years.

### 2. Embedded Doesn't Mean Messy

You can apply professional software engineering practices to microcontrollers. Dependency injection, interface segregation, single responsibility - they all work, even on 80MHz hardware with 80KB RAM.

### 3. Documentation is Part of the Product

Code without documentation is a write-only artifact. I've documented the architecture, the porting process, the data layer, and the evolution. Future me (and anyone else) will thank present me.

### 4. Interfaces Enable Flexibility

The `IDataSource` abstraction was one of the best decisions. It decoupled data acquisition from presentation completely. Adding HTTP polling or local sensors won't require touching any screen code.

### 5. Explicit is Better Than Implicit

No globals means every dependency is visible in the constructor. Testing? Mock the dependencies. Debugging? Trace the reference chain. Refactoring? The compiler tells you exactly what breaks.

## Get Involved

The project is MIT licensed and open to contributions.

**GitHub:** [Your URL here]

Whether you're:
- A hobbyist learning embedded development
- An engineer exploring ESP architecture patterns
- Someone who just needs an MQTT dashboard

The code is yours to use, learn from, and improve.

I'm happy to discuss:
- Architecture decisions and trade-offs
- Porting to different hardware
- Adding new data sources or screens
- Embedded design patterns
- Home Assistant integration strategies

## Conclusion

Telemetry started as a practical solution to a display burn-in problem. It evolved into an exercise in building maintainable embedded systems.

The result is a framework that:
- Demonstrates clean architecture on constrained hardware
- Serves as teaching material for embedded development
- Provides a reusable foundation for MQTT-driven dashboards
- Balances efficiency with maintainability

If you're tired of single-file sketches that collapse under their own weight, take a look at what's possible with a bit of upfront architectural thinking.

**Telemetry v2.0** - from sketch to framework.

---

[**Final photo gallery: Hardware, screens, code structure**]

---

**Previously:** ESP8266_TFT_MiniGrafx_Weather_UI  
**Now:** Telemetry - General-purpose embedded telemetry framework

**License:** MIT  
**GitHub:** [URL]  
**Documentation:** Architecture guides, porting guides, complete changelog
