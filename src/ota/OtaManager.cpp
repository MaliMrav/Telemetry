#include "OtaManager.h"
#include <ArduinoOTA.h>

void OtaManager::begin(const char* hostname) {
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.setPasswordHash("44a31bf445aa918be705789e9aaa5e70");
  ArduinoOTA.begin();
}

void OtaManager::loop() {
  ArduinoOTA.handle();
}