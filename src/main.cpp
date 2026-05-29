#include <Arduino.h>
#include <LittleFS.h>
#define SPIFFS LittleFS

#include "settings.h"

#include <time.h>
#include <SPI.h>

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <EspMQTTClient.h>

#include <MiniGrafx.h>
#include <ILI9341_SPI.h>
#include "ArialRounded.h"

// =====================================================
// Display
// =====================================================
#define MINI_BLACK  0
#define MINI_WHITE  1
#define MINI_YELLOW 2
#define MINI_BLUE   3

uint16_t palette[] = {
  ILI9341_BLACK,
  ILI9341_WHITE,
  ILI9341_YELLOW,
  0x7E3C
};

ILI9341_SPI tft = ILI9341_SPI(Pins::TFT_CS, Pins::TFT_DC);

MiniGrafx gfx = MiniGrafx(&tft, 2, palette);

// =====================================================
// Models
// =====================================================
enum SensorType {
  TEMP,
  HUMIDITY,
  PRESSURE
};

enum TrendDirection {
  TREND_NONE,
  TREND_UP,
  TREND_DOWN,
  TREND_FLAT
};

struct SensorTile {
  const char *label;
  const char *topic;
  const char *unit;
  SensorType type;

  const char *minTopic;
  const char *maxTopic;
  const char *trendTopic;

  float value;
  float minVal;
  float maxVal;

  TrendDirection trend;
  bool valid;
};

// =====================================================
// Sensor Configuration
// =====================================================
SensorTile sensorTiles[] = {

  { "Kitchen Temp",  Topics::Kitchen::Temp::value, "°C", TEMP,
    Topics::Kitchen::Temp::min, Topics::Kitchen::Temp::max,
    Topics::Kitchen::Temp::trend },

  { "Pergola Temp",  Topics::Pergola::Temp::value, "°C", TEMP,
    Topics::Pergola::Temp::min, Topics::Pergola::Temp::max,
    Topics::Pergola::Temp::trend },

  { "Kitchen Hum",   Topics::Kitchen::Hum::value, "%", HUMIDITY,
    Topics::Kitchen::Hum::min, Topics::Kitchen::Hum::max,
    Topics::Kitchen::Hum::trend },

  { "Pergola Hum",   Topics::Pergola::Hum::value, "%", HUMIDITY,
    Topics::Pergola::Hum::min, Topics::Pergola::Hum::max,
    Topics::Pergola::Hum::trend },

  { "Pressure",      Topics::Pergola::Pressure::value, "hPa", PRESSURE,
    Topics::Pergola::Pressure::min, Topics::Pergola::Pressure::max,
    Topics::Pergola::Pressure::trend }
};

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
const uint8_t SENSOR_COUNT = ARRAY_SIZE(sensorTiles);

// =====================================================
// MQTT
// =====================================================
#if MQTT_USE_AUTH
EspMQTTClient client(
  WiFiConfig::SSID,
  WiFiConfig::PASSWORD,
  MQTT::SERVER,
  MQTT::USERNAME,
  MQTT::PASSWORD,
  MQTT::CLIENT,
  MQTT::PORT
);
#else
EspMQTTClient client(
  WiFiConfig::SSID,
  WiFiConfig::PASSWORD,
  MQTT::SERVER,
  MQTT::CLIENT,
  MQTT::PORT
);
#endif

unsigned long lastRedraw = 0;

void drawScreen();



// =====================================================
// Setup / Loop
// =====================================================
void setup() {
  Serial.begin(115200);

  SPIFFS.begin();

  pinMode(Pins::TFT_LED, OUTPUT);
  digitalWrite(Pins::TFT_LED, LOW);

  gfx.init();
  gfx.fillBuffer(MINI_BLACK);
  gfx.commit();

  configTime(TimeConfig::TIMEZONE.c_str(), TimeConfig::NTP_SERVERS);

  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    sensorTiles[i].value = NAN;
    sensorTiles[i].minVal = NAN;
    sensorTiles[i].maxVal = NAN;
    sensorTiles[i].trend = TREND_NONE;
    sensorTiles[i].valid = false;
  }

  String hostname(System::HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  WiFi.hostname(hostname);

  ArduinoOTA.setHostname(hostname.c_str());
  ArduinoOTA.setPasswordHash("44a31bf445aa918be705789e9aaa5e70");
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  client.loop();

  if (millis() - lastRedraw > 1000) {
    drawScreen();
    lastRedraw = millis();
  }
}





void drawScreen() {
  gfx.fillBuffer(MINI_BLACK);
  drawHeader();
  drawWifiQuality();
  drawSensorGrid();
  gfx.commit();
}
