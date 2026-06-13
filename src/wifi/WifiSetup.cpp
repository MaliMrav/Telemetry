#include "WifiSetup.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include "../config/secrets.h"

namespace
{
    StatusCallback g_statusCallback = nullptr;

    void apModeCallback(
        WiFiManager* wm)
    {
        if (g_statusCallback)
        {
            g_statusCallback(
                "WiFi setup required - connect to AP",
                45);
        }
    }
}

/// This function initializes the WiFi connection. It first tries to connect using saved credentials. If that fails, it starts a captive portal for the user to connect to and configure WiFi settings. The function blocks until a connection is established.
void WifiSetup::begin(
    const char* hostname,
    StatusCallback status)
{
    g_statusCallback = status;

    if (status)
    {
        status(
            "Connecting to WiFi",
            45);
    }

    WiFiManager wm;

    wm.setAPCallback(
        apModeCallback);

    bool connected =
        wm.autoConnect(hostname);

    if (!connected)
    {
        if (status)
        {
            status(
                "WiFi connection failed",
                45);
        }

        ESP.restart();
    }

    if (status)
    {
        status(
            "WiFi connected",
            50);
    }
}