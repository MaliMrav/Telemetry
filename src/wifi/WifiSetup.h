#pragma once

class WifiSetup {
public:
  // Blocks until WiFi is connected.
  // Uses saved credentials, falls back to captive portal AP.
  static void begin(const char* hostname);
};
