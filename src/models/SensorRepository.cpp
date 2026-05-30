#include "SensorRepository.h"
#include "../config/settings.h"

SensorTile sensorTiles[] = {

  { "Kitchen Temp",
    Topics::Kitchen::Temp::value,
    "°C",
    TEMP,
    Topics::Kitchen::Temp::min,
    Topics::Kitchen::Temp::max,
    Topics::Kitchen::Temp::trend },

  { "Pergola Temp",
    Topics::Pergola::Temp::value,
    "°C",
    TEMP,
    Topics::Pergola::Temp::min,
    Topics::Pergola::Temp::max,
    Topics::Pergola::Temp::trend },

  { "Kitchen Hum",
    Topics::Kitchen::Hum::value,
    "%",
    HUMIDITY,
    Topics::Kitchen::Hum::min,
    Topics::Kitchen::Hum::max,
    Topics::Kitchen::Hum::trend },

  { "Pergola Hum",
    Topics::Pergola::Hum::value,
    "%",
    HUMIDITY,
    Topics::Pergola::Hum::min,
    Topics::Pergola::Hum::max,
    Topics::Pergola::Hum::trend },

  { "Pressure",
    Topics::Pergola::Pressure::value,
    "hPa",
    PRESSURE,
    Topics::Pergola::Pressure::min,
    Topics::Pergola::Pressure::max,
    Topics::Pergola::Pressure::trend }
};

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static constexpr uint8_t SENSOR_COUNT_VALUE =
    ARRAY_SIZE(sensorTiles);

SensorTile* SensorRepository::getTiles() {
    return sensorTiles;
}

uint8_t SensorRepository::getCount() {
    return SENSOR_COUNT_VALUE;
}