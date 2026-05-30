#pragma once

#include "secrets.h"

// Pins
namespace Pins {
  constexpr uint8_t TFT_CS  = D8;
  constexpr uint8_t TFT_DC  = D3;
  constexpr uint8_t TFT_LED = D4;
}

// System
namespace System {
  constexpr char HOSTNAME[] = "weather-display-";
}

// Time
namespace TimeConfig {

  constexpr char TIMEZONE[] =
      "AEST-10AEDT,M10.1.0,M4.1.0/3";

  constexpr const char* NTP_SERVERS[] = {
      "au.pool.ntp.org",
      "time.nist.gov",
      nullptr
  };

  const String WDAY_NAMES[] = {
      "Sun","Mon","Tue","Wed","Thu","Fri","Sat"
  };

  const String MONTH_NAMES[] = {
      "Jan","Feb","Mar","Apr","May","Jun",
      "Jul","Aug","Sep","Oct","Nov","Dec"
  };
}

// MQTT Topics
namespace Topics {

namespace Kitchen {

namespace Temp {
  constexpr char value[] = "home/kitchen/temp";
  constexpr char min[]   = "home/kitchen/temp_min";
  constexpr char max[]   = "home/kitchen/temp_max";
  constexpr char trend[] = "home/kitchen/temp_trend";
}

namespace Hum {
  constexpr char value[] = "home/kitchen/hum";
  constexpr char min[]   = "home/kitchen/hum_min";
  constexpr char max[]   = "home/kitchen/hum_max";
  constexpr char trend[] = "home/kitchen/hum_trend";
}

}

}