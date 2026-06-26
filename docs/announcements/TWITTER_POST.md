# Twitter/X Thread

## Tweet 1 (Main)
🚀 Just released Telemetry v2.0 - an open-source MQTT dashboard framework for ESP8266/ESP32

Architecture-focused • Modular design • SOLID principles • Teaching-focused code

From 500-line sketch to 60-line main with dependency injection 🧵

[Photo of display]

GitHub: [URL]

## Tweet 2
Started as a weather display for my BME280 sensors. Grew into a reusable embedded framework that demonstrates clean architecture on constrained hardware (80MHz, 80KB RAM).

Previously: ESP8266_TFT_MiniGrafx_Weather_UI
Now: Telemetry - general-purpose telemetry framework

## Tweet 3
Key architecture wins:

✅ Data source abstraction (IDataSource)
✅ Input abstraction (multi-source support)
✅ Screen lifecycle management
✅ Dependency injection throughout
✅ No globals, testable components
✅ Touch optional

Swap MQTT→HTTP→Serial without touching screen code

## Tweet 4
Current features:
• Real-time sensor dashboard with trends
• Touch calibration with LittleFS persistence
• Boot progress screen
• 12/24-hour clock toggle
• OTA updates
• Works without touchscreen

[Photo of boot screen or calibration]

## Tweet 5
Built with:
• PlatformIO
• MiniGrafx (display abstraction)
• PubSubClient (MQTT)
• ESP8266/ESP32 Arduino Core
• WiFiManager
• ArduinoOTA

Designed for: ESP8266/32, CYD, M5Stack, various displays

## Tweet 6
Design philosophy: The display is a **presentation device**

It consumes pre-processed data from Home Assistant (min/max/trends) rather than calculating locally

Less RAM • Simpler code • Centralized logic • Easier debugging

## Tweet 7
Comprehensive docs:
📖 Architecture guide
📖 Display porting guide  
📖 Complete changelog
📖 Inline documentation

This is teaching material as much as functional code

Whether you're learning embedded dev or exploring ESP patterns - dive in!

## Tweet 8 (Final)
MIT licensed. Contributions welcome.

Happy to answer questions about:
• Architecture decisions
• Porting to different hardware
• Adding data sources
• Embedded design patterns
• Home Assistant integration

GitHub: [URL]

---

## Shorter Single Tweet Option

🚀 Telemetry v2.0: Open-source MQTT dashboard framework for ESP8266/ESP32

• Modular architecture with SOLID principles
• Data source abstraction (swap MQTT/HTTP/Serial)
• Touch calibration, OTA updates, boot progress
• 500-line sketch → 60-line main via dependency injection

MIT licensed 📖

[URL]
