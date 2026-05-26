#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "TZinfo.h"

// =====================================================
// System
// =====================================================
namespace System {
  constexpr int UPDATE_INTERVAL_SECS = 10 * 60;
  constexpr int SLEEP_INTERVAL_SECS  = 0;
  constexpr bool HARD_SLEEP          = false;

  constexpr const char* HOSTNAME = "ESP-07S-TFT-";
}

// =====================================================
// WiFi
// =====================================================
namespace WiFiConfig {
  constexpr const char* SSID     = "Lekic-HA";
  constexpr const char* PASSWORD = "1793248650";
}

// =====================================================
// MQTT
// =====================================================
#define MQTT_USE_AUTH 1

namespace MQTT {
  constexpr bool USE_AUTH = true;

  constexpr const char* SERVER   = "172.20.20.140";
  constexpr uint16_t    PORT     = 1883;
  constexpr const char* CLIENT   = "ESP8266_TFT";

  constexpr const char* USERNAME = "mqtt_user";
  constexpr const char* PASSWORD = "MaliMrav";
}

// =====================================================
// MQTT Topics (clean + scalable)
// =====================================================
namespace Topics {

  // ---------- Kitchen ----------
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

  // ---------- Pergola ----------
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

  constexpr const char* NTP_SERVERS = "pool.ntp.org";
  constexpr long NTP_MIN_VALID_EPOCH = 3988515000;

  const String WDAY_NAMES[]  = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  const String MONTH_NAMES[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                               "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

  constexpr bool IS_STYLE_12HR = false;
  constexpr bool IS_STYLE_HHMM = false;
}

// =====================================================
// Display Pins
// =====================================================
namespace Pins {
  constexpr int TFT_DC  = D4;
  constexpr int TFT_CS  = D2;
  constexpr int TFT_LED = D3;
  constexpr int TFT_RST = -1;

  constexpr int TOUCH_CS  = D0;
  constexpr int TOUCH_IRQ = D1;
}

#endif
