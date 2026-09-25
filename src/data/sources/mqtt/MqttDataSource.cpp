#include "MqttDataSource.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "../../../config/config.h"
#include "../../../data/ObservationRegistry.h"
#include "../../../models/SensorRepository.h"

#include "TelemetryComposition.h"
#include "TopicMappings.h"


static WiFiClient   wifiClient;
static PubSubClient mqttClient(wifiClient);

static unsigned long lastReconnectAttempt = 0;
static bool mqttWasConnected = false;

static String mqttClientId;


static TrendDirection parseTrend(
    const String& payload)
{
    String s = payload;

    s.trim();
    s.toLowerCase();

    if (s == "up")
    {
        return TREND_UP;
    }

    if (s == "down")
    {
        return TREND_DOWN;
    }

    if (s == "flat")
    {
        return TREND_FLAT;
    }

    return TREND_NONE;
}


static void applyManualMapping(
    const TopicMapping& mapping,
    const String& payload)
{
    const ObservationHandle handle =
        ObservationRegistry::resolve(
            mapping.observation);

    if (!handle.isValid())
    {
        Serial.println(
            "[MQTT] Observation key is not registered");

        return;
    }


    switch (mapping.field)
    {
        case TopicMapping::VALUE:

            SensorRepository::setValue(
                handle,
                payload.toFloat());

            break;


        case TopicMapping::MIN:

            SensorRepository::setMin(
                handle,
                payload.toFloat());

            break;


        case TopicMapping::MAX:

            SensorRepository::setMax(
                handle,
                payload.toFloat());

            break;


        case TopicMapping::TREND:

            SensorRepository::setTrend(
                handle,
                parseTrend(payload));

            break;
    }
}


static void onMessage(
    char* topic,
    byte* payload,
    unsigned int length)
{
    const String topicStr =
        String(topic);


    String payloadStr;

    payloadStr.reserve(
        length + 1);


    for (unsigned int i = 0;
         i < length;
         ++i)
    {
        payloadStr +=
            static_cast<char>(payload[i]);
    }


    // -------------------------------------------------------------------------
    // Application composition
    // -------------------------------------------------------------------------
    //
    // Observations declared in telemetry.yaml are resolved by their source.
    //
    // MQTT is therefore treated as a transport boundary rather than a
    // separately maintained mapping table.
    //

    const TelemetryComposition::ObservationDefinition*
        observation =
            TelemetryComposition::findObservationBySource(
                "mqtt",
                topic);


    if (observation)
    {
        if (!SensorRepository::setValue(
                observation->handle,
                payloadStr.toFloat()))
        {
            Serial.println(
                "[MQTT] Generated observation update failed");
        }

        return;
    }


    // -------------------------------------------------------------------------
    // Legacy/manual Weather mappings
    // -------------------------------------------------------------------------
    //
    // Weather remains outside telemetry.yaml temporarily.
    // This path will disappear when Weather is migrated into the composition.
    //

    for (uint8_t i = 0;
         i < TOPIC_COUNT;
         ++i)
    {
        const TopicMapping& mapping =
            topicMappings[i];

        if (topicStr != mapping.topic)
        {
            continue;
        }

        applyManualMapping(
            mapping,
            payloadStr);

        return;
    }
}


static void subscribeComposedMqttTopics()
{
    const TelemetryComposition::ObservationDefinition*
        observations =
            TelemetryComposition::observations();


    const uint8_t count =
        TelemetryComposition::observationCount();


    for (uint8_t i = 0;
         i < count;
         ++i)
    {
        const auto& observation =
            observations[i];


        if (strcmp(
                observation.sourceType,
                "mqtt") != 0)
        {
            continue;
        }


        mqttClient.subscribe(
            observation.sourceTopic);
    }
}


static void subscribeManualTopics()
{
    for (uint8_t i = 0;
         i < TOPIC_COUNT;
         ++i)
    {
        mqttClient.subscribe(
            topicMappings[i].topic);
    }
}


static bool reconnect()
{
    if (!mqttClient.connect(
            mqttClientId.c_str(),
            MQTT::USERNAME,
            MQTT::PASSWORD))
    {
        return false;
    }


    // -------------------------------------------------------------------------
    // Application-defined MQTT observations
    // -------------------------------------------------------------------------

    subscribeComposedMqttTopics();


    // -------------------------------------------------------------------------
    // Existing manual Weather mappings
    // -------------------------------------------------------------------------

    subscribeManualTopics();


    return true;
}


void MqttDataSource::begin()
{
    mqttClient.setServer(
        MQTT::SERVER,
        MQTT::PORT);


    mqttClient.setCallback(
        onMessage);


    mqttClient.setKeepAlive(
        MQTT::KEEPALIVE);


    mqttWasConnected = false;


    mqttClientId =
        String("telemetry-") +
        String(ESP.getChipId(), HEX);
}


void MqttDataSource::loop()
{
    const bool connected =
        mqttClient.connected();


    if (connected)
    {
        if (!mqttWasConnected)
        {
            Serial.print(
                "[MQTT] Connected as ");

            Serial.println(
                mqttClientId);

            mqttWasConnected = true;
        }

        mqttClient.loop();

        return;
    }


    if (mqttWasConnected)
    {
        Serial.println(
            "[MQTT] Disconnected");

        mqttWasConnected = false;
    }


    const unsigned long now =
        millis();


    if (now - lastReconnectAttempt <
        MQTT::RECONNECT_MS)
    {
        return;
    }


    lastReconnectAttempt = now;


    if (reconnect())
    {
        lastReconnectAttempt = 0;
    }
}
