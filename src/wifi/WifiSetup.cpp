#include "WifiSetup.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include "../config/secrets.h"

void WifiSetup::begin(const char* hostname) {
  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostname);

  WiFiManager wm;
  bool connected = wm.autoConnect("AutoConnectAP", WiFiSecrets::PASSWORD);

  if (!connected) {
    Serial.println("WiFi failed, restarting...");
    ESP.restart();
  }

  Serial.println("WiFi connected");
}
