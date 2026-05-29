// MqttManager.cpp
//
// Move:
//
// client init
// subscriptions
// handlers
//
// This part is mostly copy/paste with minor wrapping.

// =====================================================
// MQTT Handlers
// =====================================================
void MqttManager::updateValue(uint8_t i, float v) {
  if (isnan(v)) return;
  sensorTiles[i].value = v;
  sensorTiles[i].valid = true;
}

void MqttManager::updateMin(uint8_t i, float v) {
  if (!isnan(v)) sensorTiles[i].minVal = v;
}

void MqttManager::updateMax(uint8_t i, float v) {
  if (!isnan(v)) sensorTiles[i].maxVal = v;
}

void MqttManager::updateTrend(uint8_t i, const String &p) {
  sensorTiles[i].trend = parseTrend(p);
}

void MqttManager::onConnectionEstablished() {
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
// Helpers
// =====================================================
TrendDirection MqttManager::parseTrend(const String &p) {
  String s = p;
  s.trim();
  s.toLowerCase();

  if (s == "up")   return TREND_UP;
  if (s == "down") return TREND_DOWN;
  if (s == "flat") return TREND_NONE; // previously if (s == "flat") return TREND_FLAT;
  return TREND_NONE;
}