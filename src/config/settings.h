#pragma once

#include <Arduino.h>
#include "secrets.h"
#include "TZinfo.h"

// =====================================================
// System
// =====================================================
namespace System {
  constexpr const char* HOSTNAME = "ESP-07S-TFT-";
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
// MQTT Topics
// =====================================================
namespace Topics {

  namespace Kitchen {
    namespace Temp {
      constexpr const char* value = "Kitchen_Enviro/Environment/Temperature";
      constexpr const char* min   = "ha/kitchen/temp/min";
      constexpr const char* max   = "ha/kitchen/temp/max";
      constexpr const char* trend = "ha/kitchen/temp/trend";
    }
    namespace Hum {
      constexpr const char* value = "Kitchen_Enviro/Environment/Humidity";
      constexpr const char* min   = "ha/kitchen/hum/min";
      constexpr const char* max   = "ha/kitchen/hum/max";
      constexpr const char* trend = "ha/kitchen/hum/trend";
    }
    namespace Pressure {
      constexpr const char* value = "Kitchen_Enviro/Environment/Pressure";
      constexpr const char* min   = "ha/kitchen/pressure/min";
      constexpr const char* max   = "ha/kitchen/pressure/max";
      constexpr const char* trend = "ha/kitchen/pressure/trend";
    }
  }

  namespace Pergola {
    namespace Temp {
      constexpr const char* value = "Pergola_Enviro/Environment/Temperature";
      constexpr const char* min   = "ha/pergola/temp/min";
      constexpr const char* max   = "ha/pergola/temp/max";
      constexpr const char* trend = "ha/pergola/temp/trend";
    }
    namespace Hum {
      constexpr const char* value = "Pergola_Enviro/Environment/Humidity";
      constexpr const char* min   = "ha/pergola/hum/min";
      constexpr const char* max   = "ha/pergola/hum/max";
      constexpr const char* trend = "ha/pergola/hum/trend";
    }
    namespace Pressure {
      constexpr const char* value = "Pergola_Enviro/Environment/Pressure";
      constexpr const char* min   = "ha/pergola/pressure/min";
      constexpr const char* max   = "ha/pergola/pressure/max";
      constexpr const char* trend = "ha/pergola/pressure/trend";
    }
  }
}

// =====================================================
// Time / Locale
// =====================================================
namespace TimeConfig {
  static String TIMEZONE = getTzInfo("Australia/Sydney");

  constexpr const char* NTP_SERVER = "pool.ntp.org";

  const String WDAY_NAMES[]  = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
  const String MONTH_NAMES[] = {"JAN","FEB","MAR","APR","MAY","JUN",
                                "JUL","AUG","SEP","OCT","NOV","DEC"};
}

// =====================================================
// Display Pins
// =====================================================
namespace Pins {
  constexpr int TFT_DC  = D4;
  constexpr int TFT_CS  = D2;
  constexpr int TFT_LED = D3;
}
