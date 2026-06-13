#pragma once

#include "../system/StatusCallback.h"

class WifiSetup
{
public:

    static void begin(
        const char* hostname,
        StatusCallback status = nullptr);
};