#pragma once

namespace BootProgress {

constexpr uint8_t DISPLAY_INIT = 5;
constexpr uint8_t TOUCH_INIT   = 15;
constexpr uint8_t FS_MOUNT     = 30;
constexpr uint8_t WIFI_CONNECT = 45;
constexpr uint8_t TIME_SYNC    = 60;
constexpr uint8_t SENSORS_INIT = 75;
constexpr uint8_t OTA_INIT     = 85;
constexpr uint8_t MQTT_INIT    = 95;
constexpr uint8_t READY        = 100;

}