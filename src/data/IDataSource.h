#pragma once

// IDataSource is the interface every data provider must implement.
//
// A data source is responsible for:
//   - establishing its own connection (begin)
//   - polling for new data and writing it to SensorRepository (loop)
//
// It has no knowledge of how data is displayed or stored.
// Examples: MqttDataSource, SerialDataSource, HttpDataSource.

class IDataSource
{
public:
    virtual ~IDataSource() = default;

    virtual void begin() = 0;
    virtual void loop() = 0;
};
