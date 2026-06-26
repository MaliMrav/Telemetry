#pragma once

// WifiSetup manages WiFi connectivity using WiFiManager.
//
// Responsibilities:
//   - attempt connection using stored credentials
//   - if no credentials are stored (or connection fails), launch a
//     captive portal access point so the user can configure the network
//   - block until a connection is established, then return
//   - report status at each stage via the optional StatusCallback
//
// This is the only place in the firmware that touches WiFi. All other
// components assume WiFi is already connected when they are initialised.

#include "../system/StatusCallback.h"

class WifiSetup
{
public:

    static void begin(
        const char* hostname,
        StatusCallback status = nullptr);
};