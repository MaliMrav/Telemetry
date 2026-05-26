#pragma once

class OtaManager {
public:
  void begin(const char* hostname);
  void loop();
};
