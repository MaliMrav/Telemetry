#pragma once

// Topics defines all MQTT topic strings used by this firmware.
//
// Topics are organised by location and measurement type using nested
// namespaces. This makes the mapping between physical sensors and
// their broker topics explicit and easy to audit.
//
// These strings are referenced only from TopicMappings.cpp.
// Changing a topic requires a change here only.

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