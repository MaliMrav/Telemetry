#include "SensorRepository.h"

SensorTile sensorTiles[] = {
  { "Kitchen Temp",  "°C",  TEMP },
  { "Pergola Temp",  "°C",  TEMP },
  { "Kitchen Hum",   "%",   HUMIDITY },
  { "Pergola Hum",   "%",   HUMIDITY },
  { "Pressure",      "hPa", PRESSURE }
};

static constexpr uint8_t SENSOR_COUNT_VALUE =
    sizeof(sensorTiles) / sizeof(sensorTiles[0]);

SensorTile* SensorRepository::getTiles() {
  return sensorTiles;
}

uint8_t SensorRepository::getCount() {
  return SENSOR_COUNT_VALUE;
}

void SensorRepository::initialise() {
  for (auto& tile : sensorTiles) {
    tile.value  = NAN;
    tile.minVal = NAN;
    tile.maxVal = NAN;
    tile.trend  = TREND_NONE;
    tile.valid  = false;
  }
}