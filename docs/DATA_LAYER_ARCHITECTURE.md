# Data Layer Architecture

## Overview

The data layer decouples data providers from the repository and UI. Any data source — MQTT, HTTP, Serial, local I2C sensor — implements the same interface and writes through the same repository API. Screens are never aware of where data came from.

---

## Flow

```
┌──────────────────┐
│   IDataSource    │  interface: begin(), loop()
└────────┬─────────┘
         │ implemented by
         ↓
┌──────────────────┐
│ MqttDataSource   │  knows: MQTT broker, topics, PubSubClient
│                  │  does not know: storage, display, WiFi setup
└────────┬─────────┘
         │ writes via typed API
         ↓
┌──────────────────────────────┐
│      SensorRepository        │
│  setValue(SensorId, float)   │
│  setMin  (SensorId, float)   │
│  setMax  (SensorId, float)   │
│  setTrend(SensorId, Trend)   │
└──────────┬───────────────────┘
           │ read by
           ↓
┌──────────────────┐
│  WeatherScreen   │  calls getTiles(), renders data
└──────────────────┘
```

---

## Components

### IDataSource — `src/data/IDataSource.h`

The interface every data provider implements. `SystemManager` holds a reference to `IDataSource` — it never knows what kind of source is behind it.

### MqttDataSource — `src/mqtt/MqttDataSource.h`

The MQTT implementation. Connects via PubSubClient, subscribes to topics defined in `TopicMappings`, parses payloads, and writes to `SensorRepository`. Reconnects automatically. Has no knowledge of WiFi, display, or screens.

### SensorRepository — `src/models/SensorRepository.h`

The single source of truth for all sensor data. Exposes a typed write API keyed by `SensorId` so data sources never touch internal array indices directly. Screens read via `getTiles()`.

### TopicMappings — `src/mqtt/TopicMappings.h`

A flat table binding MQTT topic strings to a `SensorId` and field type (`VALUE`, `MIN`, `MAX`, `TREND`). `MqttDataSource` iterates this on every incoming message.

**To add a new MQTT sensor — only three files change:**
1. Add a `SensorId` to `models/SensorIds.h`
2. Add its display tile to `models/SensorRepository.cpp`
3. Add its topic rows to `mqtt/TopicMappings.cpp`

`MqttDataSource` itself does not change.

---

## Adding a New Data Source

Implement `IDataSource`, write to `SensorRepository`, pass to `SystemManager::begin()`.

```cpp
// src/data/HttpDataSource.h
#pragma once
#include "IDataSource.h"

class HttpDataSource : public IDataSource
{
public:
    void begin() override;
    void loop()  override;
private:
    unsigned long lastFetch_ = 0;
};
```

```cpp
// src/data/HttpDataSource.cpp
#include "HttpDataSource.h"
#include "../models/SensorRepository.h"
#include "../models/SensorIds.h"

void HttpDataSource::begin() { /* configure client */ }

void HttpDataSource::loop()
{
    if (millis() - lastFetch_ < 60000) return;
    lastFetch_ = millis();

    // fetch + parse
    SensorRepository::setValue(SENSOR_KITCHEN_TEMP, parsedTemp);
}
```

```cpp
// main.cpp
HttpDataSource httpData;

SystemManager::begin(
    display, touchController, touchManager,
    screenManager, bootScreen, weatherScreen,
    calibrationScreen, ota,
    httpData);   // swap data source here
```

No changes to `SensorRepository`, `WeatherScreen`, or `SystemManager`.

---

## Design Principles Applied

- **Single Responsibility** — each component has one job; `MqttDataSource` knows MQTT, `SensorRepository` owns storage, screens own display
- **Open/Closed** — extend with new data sources without modifying existing code
- **Dependency Inversion** — `SystemManager` depends on the `IDataSource` abstraction, not a concrete implementation

---

## Future Extensions

- **DataSourceManager** — register and loop multiple sources simultaneously (MQTT + local sensor on same display)
- **Staleness tracking** — repository marks tiles as stale if not updated within a timeout
- **Change notifications** — notify screens when specific sensors update, avoiding full redraws
