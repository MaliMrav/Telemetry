# Display Configuration Guide

## Current Hardware

This firmware is configured for a **2.4" ILI9341 TFT display** (240×320 resolution, SPI interface).

Common sources:
- ESP8266 + ILI9341 development kits
- Adafruit ILI9341 breakouts
- Generic 2.4" TFT modules with XPT2046 touch

---

## Using a Different Display

If you're using different display hardware, follow these steps:

### 1. Set the display driver

Open `src/config/config_override.h` and uncomment/set:

```cpp
#undef CFG_DISPLAY_DRIVER
#define CFG_DISPLAY_DRIVER ILI9341  // or SSD1306, ST7735, etc.
```

### 2. Set the resolution

Uncomment and adjust to match your hardware:

```cpp
#undef CFG_SCREEN_WIDTH
#define CFG_SCREEN_WIDTH 240

#undef CFG_SCREEN_HEIGHT
#define CFG_SCREEN_HEIGHT 320
```

Common resolutions:
- **240×320** — ILI9341 2.4" TFT (portrait)
- **320×240** — ILI9341 2.4" TFT (landscape, swap width/height)
- **128×64** — SSD1306 0.96" OLED
- **128×160** — ST7735 1.8" TFT

### 3. Verify pin assignments

Check that the pin definitions match your wiring:

```cpp
#undef CFG_TFT_DC
#define CFG_TFT_DC D4   // Data/Command pin

#undef CFG_TFT_CS
#define CFG_TFT_CS D2   // Chip Select pin

#undef CFG_TFT_LED
#define CFG_TFT_LED D3  // Backlight control (set to -1 if always-on)
```

ESP8266 pin reference:
- **D0** = GPIO16
- **D1** = GPIO5 (I2C SCL)
- **D2** = GPIO4 (I2C SDA)
- **D3** = GPIO0
- **D4** = GPIO2
- **D5** = GPIO14 (SPI SCK)
- **D6** = GPIO12 (SPI MISO)
- **D7** = GPIO13 (SPI MOSI)
- **D8** = GPIO15 (SPI CS)

### 4. Configure driver-specific settings

Each display driver has its own config file in `src/config/display/`.

For example, `ILI9341_config.h` defines:
- Color palette (4 colors for 2-bit framebuffer mode)
- Bits per pixel
- SPI clock frequency

Copy the existing `ILI9341_config.h` as a template if adding a new driver.

### 5. Update layout constants (optional)

Screen resolution changes may require adjusting tile layout.

Edit `src/screens/ScreenConfig.h`:
- `TOP_MARGIN` — space below the header before tiles start
- `SIDE_MARGIN` — left/right padding
- `TILE_GAP` — spacing between tiles
- `COLUMNS` — number of tile columns (2 works well for 240px width)

---

## Supported Display Drivers

### ILI9341 (Current)

- **Hardware**: 2.4" 240×320 TFT LCD
- **Interface**: SPI
- **Status**: ✅ Fully configured
- **Config**: `src/config/display/ILI9341_config.h`

### SSD1306 (Future)

- **Hardware**: 0.96" 128×64 OLED
- **Interface**: I2C or SPI
- **Status**: ⚠️ Not yet implemented
- **Notes**: Requires porting MiniGrafx SSD1306 driver and updating DisplayManager

### ST7735 (Future)

- **Hardware**: 1.8" 128×160 TFT LCD
- **Interface**: SPI
- **Status**: ⚠️ Not yet implemented
- **Notes**: MiniGrafx supports ST7735; needs config file and testing

---

## Adding a New Display Driver

If your display isn't supported yet:

1. **Check MiniGrafx compatibility**  
   Visit https://github.com/ThingPulse/minigrafx to see if your driver exists.

2. **Create a config file**  
   Copy `src/config/display/ILI9341_config.h` to `YourDriver_config.h`.  
   Update palette, bit depth, and SPI frequency for your hardware.

3. **Update `config.h`**  
   Add your driver to the conditional includes:
   ```cpp
   #elif CFG_DISPLAY_DRIVER == YOUR_DRIVER
     #include "display/YourDriver_config.h"
   ```

4. **Update `DisplayManager` if needed**  
   If the driver API differs from ILI9341, add conditional compilation in `DisplayManager.cpp`.

5. **Test and adjust**  
   Build, flash, and verify. Adjust layout constants in `ScreenConfig.h` as needed.

6. **Contribute back** (optional)  
   Open a pull request with your new driver config so others can benefit.

---

## Pin Conflict Reference

Common ESP8266 pin conflicts to watch for:

- **GPIO0 (D3)** — boot mode selection (must be HIGH at boot)
- **GPIO2 (D4)** — boot mode selection, onboard LED (must be HIGH at boot)
- **GPIO15 (D8)** — boot mode selection (must be LOW at boot)
- **GPIO16 (D0)** — used for deep sleep wakeup, no I2C/PWM support

If your display wiring conflicts with boot requirements, you may need to add pull-up/pull-down resistors or remap pins.

---

## Troubleshooting

**Display is blank after boot:**
- Verify pin assignments in `config_override.h`
- Check physical wiring matches the config
- Confirm backlight pin (CFG_TFT_LED) is correct — some boards are active-HIGH, others active-LOW
- Try setting `CFG_TFT_LED` to `-1` to disable backlight control

**Garbled or flickering display:**
- Lower `DISPLAY_SPI_FREQUENCY` in the driver config (try 20 MHz or 13.5 MHz)
- Check for loose wiring or breadboard contact issues
- Verify power supply can handle display + ESP8266 (recommend 500mA+)

**Compile errors:**
- Ensure `CFG_DISPLAY_DRIVER` is set in `config_override.h`
- Check that the corresponding driver config exists in `src/config/display/`
- Verify MiniGrafx library is installed (should auto-install via PlatformIO)
