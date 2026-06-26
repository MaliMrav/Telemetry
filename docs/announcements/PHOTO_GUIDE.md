# Photo Guide for Announcements

This guide lists all the photos needed for the announcement posts and where they should be used.

---

## 📸 Essential Photos (Must Have)

### 1. Hero Shot - WeatherScreen Running
**Description:** The main dashboard displaying live sensor data with tiles, clock, and WiFi indicator

**Where used:**
- Reddit post (main image)
- Hackaday post (hero image)
- Twitter/X (first tweet)
- Blog post (hero image)
- GitHub release

**Capture details:**
- Full display showing WeatherScreen
- Live data visible in tiles
- Clock showing time
- WiFi indicator visible
- All sensor tiles showing values, min/max, trends
- Good lighting, clear screen
- Consider taking both 12-hour and 24-hour clock versions

**Tips:**
- Take photo when display has interesting data (varied temps, visible trends)
- Avoid glare/reflections on screen
- Show the full display, not cropped

---

### 2. BootScreen Progress
**Description:** The boot sequence screen showing progress bar and status message

**Where used:**
- Reddit post
- Hackaday post
- Twitter/X thread
- Blog post
- GitHub release

**Capture details:**
- Progress bar visible
- Status message readable (e.g. "Starting WiFi 30%")
- Clear progress indicator
- Good screen contrast

**Tips:**
- Capture at an interesting stage (30-70%) rather than beginning or end
- Make sure status text is readable
- Could capture multiple stages and create a composite image

---

### 3. CalibrationScreen
**Description:** Touch calibration in progress with crosshair target visible

**Where used:**
- Hackaday post
- Blog post
- GitHub release

**Capture details:**
- Crosshair target visible in corner
- Instruction text visible if present
- Shows the calibration workflow in action

**Tips:**
- Capture when crosshair is in one of the four corners
- Could show multiple stages (different corners) in separate photos
- Optional: Show "Calibration saved!" completion screen

---

## 📊 Recommended Photos (Should Have)

### 4. Hardware Assembly
**Description:** Physical hardware showing ESP8266 + display + connections

**Where used:**
- Hackaday post (build photos section)
- Blog post
- GitHub release

**Capture details:**
- Clear view of ESP8266 board
- Display module visible
- Wiring visible (if applicable)
- Clean assembly

**Tips:**
- Use good lighting
- Consider multiple angles
- Label components if creating a build guide
- Show both front (display) and back (wiring) views

---

### 5. Architecture Diagram
**Description:** Visual representation of the modular architecture

**Where used:**
- Reddit post
- Hackaday post
- Twitter/X thread
- Blog post

**Options:**
1. Screenshot of the ASCII diagram from README.md
2. Create a visual flowchart/diagram using a tool like draw.io
3. Photo of whiteboard/paper sketch showing module relationships
4. Screenshot of directory structure with annotations

**Content should show:**
```
main.cpp
├─ DisplayManager
├─ TouchController
├─ TouchManager
├─ ScreenManager
│   ├─ BootScreen
│   ├─ WeatherScreen
│   ├─ CalibrationScreen
│   └─ StatusScreen
├─ InputManager
├─ SystemManager
├─ OtaManager
├─ MqttDataSource
└─ SensorRepository
```

---

### 6. Code Structure Comparison
**Description:** Before/after comparison of code organization

**Where used:**
- Reddit post
- Hackaday post
- Blog post

**Options:**
1. Side-by-side screenshots of old vs new `main.cpp`
2. Visual showing "500 lines" vs "60 lines"
3. Directory tree comparison (flat vs modular)

**Content:**
```
Before: Single .ino file (500+ lines)
After: 11 modules, main.cpp = 60 lines
```

---

## 🎨 Optional Photos (Nice to Have)

### 7. Display in Use (Environmental Shot)
**Description:** The display in its actual home environment

**Shows the real-world use case**
- Display mounted/positioned in your home
- In context (kitchen, office, etc.)
- Gives personality to the project

---

### 8. Multiple Screens Side-by-Side
**Description:** Photo showing transition between different screens

**Could show:**
- Boot → Calibration → Weather sequence
- Different states of WeatherScreen (12hr vs 24hr)
- Before/after calibration

---

### 9. Sensor Nodes
**Description:** The BME280 sensor nodes that publish to MQTT

**Shows the ecosystem:**
- ESP8266 + BME280 sensors
- Kitchen node
- Pergola node
- Helps tell the complete story

---

### 10. Development Setup
**Description:** Your development environment

**Shows:**
- PlatformIO in VS Code
- Display connected during development
- Serial monitor showing debug output
- OTA update in progress

**Appeals to:** Developers interested in the tooling/workflow

---

## 📐 Photo Specifications

### Format
- **Aspect ratio:** 16:9 or 4:3 preferred for most platforms
- **Resolution:** At least 1920×1080 for high-quality sharing
- **Format:** JPG or PNG

### Display Photos
- **Orientation:** Portrait for display itself (240×320), but photograph in landscape with context
- **Lighting:** Bright, even lighting - avoid direct overhead lights that cause glare
- **Focus:** Sharp focus on screen content
- **Background:** Clean, uncluttered background preferred

### Best Practices
- ✅ Use natural lighting when possible
- ✅ Steady hands or tripod for sharp images
- ✅ Multiple shots - pick the best later
- ✅ Check screen readability before finalizing
- ❌ Avoid glare/reflections on display
- ❌ Avoid motion blur
- ❌ Don't over-compress images

---

## 📝 Photo Placement Checklist

Before posting, verify you have photos for:

**Reddit:**
- [ ] WeatherScreen hero shot
- [ ] Architecture diagram or code structure

**Hackaday:**
- [ ] WeatherScreen hero shot
- [ ] Code structure comparison
- [ ] CalibrationScreen
- [ ] BootScreen
- [ ] Hardware assembly (optional)

**Twitter/X:**
- [ ] WeatherScreen hero shot
- [ ] BootScreen or CalibrationScreen

**Blog Post:**
- [ ] WeatherScreen hero shot
- [ ] BootScreen
- [ ] CalibrationScreen
- [ ] Code structure comparison
- [ ] Architecture diagram
- [ ] Hardware assembly (optional)

**GitHub Release:**
- [ ] WeatherScreen hero shot
- [ ] BootScreen
- [ ] CalibrationScreen
- [ ] Architecture diagram

---

## 🖼️ Where to Store Photos

Create a directory structure:
```
docs/
└── announcements/
    └── images/
        ├── hero-weatherscreen.jpg
        ├── bootscreen-progress.jpg
        ├── calibration-workflow.jpg
        ├── hardware-assembly.jpg
        ├── architecture-diagram.png
        ├── code-comparison.png
        └── README.md (this file)
```

**Recommendation:**
- Keep original high-resolution versions
- Create web-optimized versions for posting (max 2MB)
- Name files descriptively
- Add to `.gitignore` if files are large (>5MB)

---

## 🎬 Alternative: Video Content

Consider creating short videos (optional):
- 30-second clip showing boot sequence
- Touch calibration workflow
- Screen transitions
- Time-lapse of display running over 24 hours showing data changes

Video platforms: YouTube, Imgur (GIF), Twitter/X (native video)

---

## Quick Start Checklist

**Minimum viable announcement photos:**
1. ✅ WeatherScreen running with live data
2. ✅ BootScreen showing progress
3. ✅ Simple architecture diagram (even ASCII art as PNG)

**With these 3 photos, you can post to all platforms.**

Everything else enhances the announcement but isn't strictly required.
