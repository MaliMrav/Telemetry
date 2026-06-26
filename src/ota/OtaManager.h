#pragma once

// OtaManager wraps ArduinoOTA to provide wireless firmware updates.
//
// Responsibilities:
//   - register the device hostname with the OTA service
//   - set authentication credentials
//   - call ArduinoOTA.handle() each loop to service incoming update requests
//
// OTA must be initialised after WiFi is connected. begin() is called
// once during boot; loop() is called every iteration of the main loop.
// It has no knowledge of display, screens, or sensors.

class OtaManager {
public:
  void begin(const char* hostname);
  void loop();
};
