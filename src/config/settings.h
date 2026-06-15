#pragma once

#include <Arduino.h>
#include "secrets.h"

// =====================================================
// System
// =====================================================
namespace System {
  constexpr const char* HOSTNAME = "Weather-Display-";
}

// =====================================================
// WiFi
// =====================================================
namespace WiFiConfig {
  constexpr const char* SSID     = WiFiSecrets::SSID;
  constexpr const char* PASSWORD = WiFiSecrets::PASSWORD;
}

// =====================================================
// MQTT
// =====================================================
namespace MQTT {
  constexpr const char* SERVER   = MqttSecrets::SERVER;
  constexpr uint16_t    PORT     = 1883;
  constexpr const char* CLIENT   = MqttSecrets::CLIENT;
  constexpr const char* USERNAME = MqttSecrets::USERNAME;
  constexpr const char* PASSWORD = MqttSecrets::PASSWORD;
}

// =====================================================
// Time / Locale
// =====================================================
namespace TimeConfig {
  extern const String TIMEZONE;

  constexpr const char* NTP_SERVER = "pool.ntp.org";

  extern const String WDAY_NAMES[7];
  extern const String MONTH_NAMES[12];
}

// =====================================================
// Display Pins
// =====================================================
namespace Pins {
  constexpr int TFT_DC  = D4;
  constexpr int TFT_CS  = D2;
  constexpr int TFT_LED = D3;
}
