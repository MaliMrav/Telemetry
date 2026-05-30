#include "SystemManager.h"
#include <ESP8266WiFi.h>
#include "../config/settings.h"

namespace SystemManager {
  String getHostname() {
    return String(System::HOSTNAME) + String(ESP.getChipId(), HEX);
  }
}
