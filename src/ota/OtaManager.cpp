#include "OtaManager.h"
#include <ArduinoOTA.h>
#include "../config/secrets.h"

void OtaManager::begin(const char* hostname) {
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.setPasswordHash(OtaSecrets::PASSWORD_HASH);
  ArduinoOTA.begin();
}

void OtaManager::loop() {
  ArduinoOTA.handle();
}