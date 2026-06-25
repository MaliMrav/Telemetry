#include "SensorRepository.h"

// Each entry maps a SensorId to its display tile.
// Order here is the display order on screen.
static SensorTile sensorTiles[] = {
    { "Kitchen Temp",  "\u00b0C",  TEMP     },
    { "Pergola Temp",  "\u00b0C",  TEMP     },
    { "Kitchen Hum",   "%",   HUMIDITY },
    { "Pergola Hum",   "%",   HUMIDITY },
    { "Pressure",      "hPa", PRESSURE }
};

static_assert(
    sizeof(sensorTiles) / sizeof(sensorTiles[0]) == SENSOR_COUNT,
    "sensorTiles must have one entry per SensorId");

void SensorRepository::initialise()
{
    for (auto& tile : sensorTiles)
    {
        tile.value  = NAN;
        tile.minVal = NAN;
        tile.maxVal = NAN;
        tile.trend  = TREND_NONE;
        tile.valid  = false;
    }
}

uint8_t SensorRepository::getCount()
{
    return static_cast<uint8_t>(SENSOR_COUNT);
}

SensorTile* SensorRepository::getTiles()
{
    return sensorTiles;
}

void SensorRepository::setValue(SensorId id, float value)
{
    if (isnan(value)) return;
    sensorTiles[id].value = value;
    sensorTiles[id].valid = true;
}

void SensorRepository::setMin(SensorId id, float value)
{
    if (!isnan(value)) sensorTiles[id].minVal = value;
}

void SensorRepository::setMax(SensorId id, float value)
{
    if (!isnan(value)) sensorTiles[id].maxVal = value;
}

void SensorRepository::setTrend(SensorId id, TrendDirection trend)
{
    sensorTiles[id].trend = trend;
}