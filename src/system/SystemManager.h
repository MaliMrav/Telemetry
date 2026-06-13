#pragma once
#include <Arduino.h>

namespace SystemManager {
  
  bool mountFileSystem();

  String getHostname();
}
