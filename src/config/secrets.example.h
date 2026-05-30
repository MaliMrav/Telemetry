#pragma once

namespace WiFiConfig {
  constexpr char SSID[] = "MyWifi";
  constexpr char PASSWORD[] = "SuperSecretPassword";
}

namespace MQTT {
  constexpr char SERVER[] = "192.168.1.100";
  constexpr uint16_t PORT = 1883;

  constexpr char CLIENT[] = "WeatherDisplay";

  constexpr char USERNAME[] = "mqttuser";
  constexpr char PASSWORD[] = "mqttpassword";
}

namespace OTA {
  constexpr char PASSWORD[] = "T0pserv1ce";

  // Optional if you prefer hashes
  constexpr char PASSWORD_HASH[] =
      "44a31bf445aa918be705789e9aaa5e70";
}