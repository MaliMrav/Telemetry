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
// Helpers
// =====================================================
TrendDirection parseTrend(const String &p) {
  String s = p;
  s.trim();
  s.toLowerCase();

  if (s == "up")   return TREND_UP;
  if (s == "down") return TREND_DOWN;
  if (s == "flat") return TREND_NONE; // previously if (s == "flat") return TREND_FLAT;
  return TREND_NONE;
}

String formatValue(SensorType type, float v) {
  if (isnan(v)) return "--";

  switch (type) {
    case TEMP:     return String(v, 1);
    case HUMIDITY:
    case PRESSURE: return String((int)round(v));
  }
  return "--";
}

uint16_t getColor(SensorType t) {
  if (t == HUMIDITY) return MINI_BLUE;
  if (t == PRESSURE) return MINI_YELLOW;
  return MINI_WHITE;
}

// =====================================================
// MQTT Handlers
// =====================================================
void updateValue(uint8_t i, float v) {
  if (isnan(v)) return;
  sensorTiles[i].value = v;
  sensorTiles[i].valid = true;
}

void updateMin(uint8_t i, float v) {
  if (!isnan(v)) sensorTiles[i].minVal = v;
}

void updateMax(uint8_t i, float v) {
  if (!isnan(v)) sensorTiles[i].maxVal = v;
}

void updateTrend(uint8_t i, const String &p) {
  sensorTiles[i].trend = parseTrend(p);
}

void onConnectionEstablished() {
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {

    client.subscribe(sensorTiles[i].topic,
      [i](const String &p) { updateValue(i, p.toFloat()); });

    client.subscribe(sensorTiles[i].minTopic,
      [i](const String &p) { updateMin(i, p.toFloat()); });

    client.subscribe(sensorTiles[i].maxTopic,
      [i](const String &p) { updateMax(i, p.toFloat()); });

    client.subscribe(sensorTiles[i].trendTopic,
      [i](const String &p) { updateTrend(i, p); });
  }
}

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

// =====================================================
// Drawing
// =====================================================
void drawTrendArrow(int x, int y, TrendDirection t) {

  auto thick = [&](int x1, int y1, int x2, int y2) {
    gfx.drawLine(x1, y1, x2, y2);
    gfx.drawLine(x1+1, y1, x2+1, y2);
  };

  switch (t) {
    case TREND_UP:
      thick(x, y+6, x, y-6);
      thick(x, y-6, x-3, y-2);
      thick(x, y-6, x+3, y-2);
      break;

    case TREND_DOWN:
      thick(x, y-6, x, y+6);
      thick(x, y+6, x-3, y+2);
      thick(x, y+6, x+3, y+2);
      break;

    case TREND_FLAT:
      thick(x-3, y, x+3, y);
      break;

    default:
      break;
  }
}

void drawHeader() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(MINI_WHITE);

  if (!t) return;

  gfx.setFont(ArialRoundedMTBold_14);
  String date = TimeConfig::WDAY_NAMES[t->tm_wday] + " " +
                TimeConfig::MONTH_NAMES[t->tm_mon] + " " +
                String(t->tm_mday);
  gfx.drawString(120, 6, date);

  gfx.setFont(ArialRoundedMTBold_36);
  char buf[10];
  sprintf(buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
  gfx.drawString(120, 20, buf);
}

// *************************
//    Get WiFi Quality
// *************************
int8_t getWifiQuality() {
  int32_t dbm = WiFi.RSSI();
  if (dbm <= -100) return 0;
  if (dbm >= -50) return 100;
  return 2 * (dbm + 100);
}
// **************************************************
//                Draw WiFi Quality
// **************************************************
void drawWifiQuality() {
  int8_t q = getWifiQuality();

  gfx.setTextAlignment(TEXT_ALIGN_RIGHT);
  gfx.setFont(ArialMT_Plain_10);
  gfx.setColor(MINI_WHITE);
  gfx.drawString(228, 9, String(q) + "%");

  for (int8_t i = 0; i < 4; i++) {
    for (int8_t j = 0; j < 2 * (i + 1); j++) {
      if (q > i * 25 || j == 0) {
        gfx.setPixel(230 + 2 * i, 18 - j);
      }
    }
  }
}

void drawSensorGrid() {

  const int margin = 8;
  const int gap = 8;
  const int topY = 60;

  const int cols = 2;
  const int rows = (SENSOR_COUNT + 1) / 2;

  const int tileW = (gfx.getWidth() - (2*margin) - gap) / 2;
  const int tileH = (gfx.getHeight() - topY - (rows*gap)) / rows;

  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {

    uint8_t row = i / cols;
    uint8_t col = i % cols;

    bool full = (i == SENSOR_COUNT - 1);

    int x = full ? margin : margin + col * (tileW + gap);
    int w = full ? gfx.getWidth() - (2*margin) : tileW;
    int y = topY + row * (tileH + gap);

    SensorTile &s = sensorTiles[i];

    gfx.setColor(MINI_WHITE);
    gfx.drawRect(x, y, w, tileH);

    // Label
    gfx.setFont(ArialRoundedMTBold_14);
    gfx.setColor(getColor(s.type));
    gfx.setTextAlignment(TEXT_ALIGN_CENTER);
    gfx.drawString(x + w/2, y + 10, s.label);

    // Value + arrow
    gfx.setFont(ArialMT_Plain_24);
    
    String valueStr = formatValue(s.type, s.value) + s.unit;
    int valueWidth = gfx.getStringWidth(valueStr.c_str(), valueStr.length());
    
    // --- Layout constants ---
    const int arrowOffset = 12;   // arrow column
    const int arrowWidth  = 10;
    const int spacing     = 6;
    const int rightPad    = 6;
    
    // Fixed arrow position
    int arrowCenterX = x + arrowOffset;

    // Define content area (to the right of arrow)
    int contentLeft  = x + arrowOffset + arrowWidth + spacing;
    int contentRight = x + w - rightPad;
    int contentWidth = contentRight - contentLeft;
    
    // Center value inside content area
    int cy = y + tileH/2 - 8;
    int valueStartX = contentLeft + (contentWidth - valueWidth) / 2;
     
    if (s.trend != TREND_NONE) {
      gfx.setColor(
        s.trend == TREND_UP   ? MINI_YELLOW :
        s.trend == TREND_DOWN ? MINI_BLUE   :
                                MINI_WHITE
      );

      drawTrendArrow(arrowCenterX, cy + 10, s.trend);
    }

    gfx.setColor(MINI_WHITE);
    gfx.setTextAlignment(TEXT_ALIGN_LEFT);
    gfx.drawString(valueStartX, cy, valueStr);

    // restore for rest of tile
    gfx.setTextAlignment(TEXT_ALIGN_CENTER);
    
    // Min/Max
    gfx.setFont(ArialMT_Plain_10);
    gfx.setColor(MINI_BLUE);

    if (!isnan(s.minVal) && !isnan(s.maxVal)) {
      String mm = formatValue(s.type, s.minVal) + " / " +
                  formatValue(s.type, s.maxVal);
      gfx.drawString(x + w/2, y + tileH - 18, mm);
    } else {
      gfx.drawString(x + w/2, y + tileH - 18, "-- / --");
    }
  }
}

void drawScreen() {
  gfx.fillBuffer(MINI_BLACK);
  drawHeader();
  drawWifiQuality();
  drawSensorGrid();
  gfx.commit();
}
