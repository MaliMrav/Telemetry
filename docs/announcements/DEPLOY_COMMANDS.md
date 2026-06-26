# Quick Deployment Commands

Copy and paste these commands in order to get everything ready for tonight's post.

---

## STEP 1: Add Your Photos

First, copy your 4 photos to the images directory:

```bash
cd /Users/vladimir/Documents/GitHub/telemetry

# Create images directory if it doesn't exist
mkdir -p docs/announcements/images

# Now manually copy your 4 photos to docs/announcements/images/
# Rename them to:
# - weatherscreen.jpg (or .png)
# - bootscreen.jpg (or .png)
# - calibration.jpg (or .png) 
# - hardware.jpg (or architecture.png)

# Example: If your photos are in ~/Desktop/
# cp ~/Desktop/photo1.jpg docs/announcements/images/weatherscreen.jpg
# cp ~/Desktop/photo2.jpg docs/announcements/images/bootscreen.jpg
# cp ~/Desktop/photo3.jpg docs/announcements/images/calibration.jpg
# cp ~/Desktop/photo4.jpg docs/announcements/images/hardware.jpg
```

**STOP HERE and copy your photos first!**

---

## STEP 2: Verify No Secrets Leaked

Run this security check:

```bash
cd /Users/vladimir/Documents/GitHub/telemetry

# Check for any real credentials
grep -r "password\|secret" src/ --include="*.h" --include="*.cpp" | grep -v "example" | grep -v "SECRET" | grep -v "//"

# Verify secrets.h is gitignored
cat .gitignore | grep secrets.h
```

**If any real passwords show up - STOP and remove them!**

---

## STEP 3: Check Git Status

```bash
cd /Users/vladimir/Documents/GitHub/telemetry

git status
```

You should see:
- Untracked: docs/announcements/
- Modified: (possibly README.md, CHANGELOG.md if not committed yet)

---

## STEP 4: Stage All Changes

```bash
cd /Users/vladimir/Documents/GitHub/telemetry

# Add announcement materials
git add docs/announcements/

# Add any other modified files
git add README.md CHANGELOG.md LICENSE.md
```

---

## STEP 5: Commit Everything

```bash
cd /Users/vladimir/Documents/GitHub/telemetry

git commit -m "Release v2.0.0: Telemetry Framework

- Complete documentation overhaul
- Add comprehensive announcement materials  
- Add launch photos and guides
- Rebrand from ESP8266_TFT_MiniGrafx_Weather_UI to Telemetry
- Ready for public launch"
```

---

## STEP 6: Push to GitHub

```bash
cd /Users/vladimir/Documents/GitHub/telemetry

git push origin main
```

Wait for push to complete, then verify at:
https://github.com/MaliMrav/Telemetry

---

## STEP 7: Create GitHub Release

**Can't be done via command line easily - do this in browser:**

1. Go to: https://github.com/MaliMrav/Telemetry/releases/new

2. Tag version: `v2.0.0` (type it and select "Create new tag")

3. Release title: `v2.0.0 - Telemetry Framework`

4. Description: Copy from `docs/announcements/GITHUB_RELEASE.md`

5. Attach your 4 photos:
   - Drag and drop all 4 photos into the description
   - Position them where the placeholders indicate

6. Click "Publish release"

---

## STEP 8: Update GitHub About Section

**Also browser-based:**

1. Go to: https://github.com/MaliMrav/Telemetry

2. Click ⚙️ gear icon next to "About" (right sidebar)

3. **Description:**
   ```
   Architecture-focused MQTT dashboard framework for ESP8266/ESP32. Modular design with SOLID principles on constrained hardware.
   ```

4. **Topics (add these tags):**
   ```
   esp8266
   esp32
   mqtt
   iot
   platformio
   embedded-systems
   home-assistant
   dashboard
   framework
   tft-display
   arduino
   ```

5. Check ✅ Releases

6. Click "Save changes"

---

## STEP 9: Final Verification

Check these URLs to make sure everything looks good:

```bash
# Open in browser:
open https://github.com/MaliMrav/Telemetry
open https://github.com/MaliMrav/Telemetry/releases
open https://github.com/MaliMrav/Telemetry/blob/main/README.md
open https://github.com/MaliMrav/Telemetry/blob/main/CHANGELOG.md
```

**Verify:**
- [ ] README displays correctly
- [ ] About section is filled in with topics
- [ ] Release v2.0.0 exists with photos
- [ ] Photos are visible in release
- [ ] CHANGELOG is accessible
- [ ] License shows MIT

---

## STEP 10: Prepare Reddit Post

1. Open: `docs/announcements/REDDIT_POST.md`

2. Replace: `[Add your GitHub URL here]`
   With: `https://github.com/MaliMrav/Telemetry`

3. Save the file (or just remember to use the correct URL)

4. Have your 4 photos ready to upload to Reddit

5. Follow: `docs/announcements/REDDIT_POSTING_GUIDE.md`

---

## All-In-One Script (If you want to run it all at once)

⚠️ **Only run this after you've copied your photos!**

```bash
#!/bin/bash

cd /Users/vladimir/Documents/GitHub/telemetry

# Security check
echo "🔒 Security check..."
if grep -r "password\|secret" src/ --include="*.h" --include="*.cpp" | grep -v "example" | grep -v "SECRET" | grep -v "//" ; then
    echo "❌ Found potential credentials! Please review before continuing."
    exit 1
fi

# Check if photos exist
if [ ! -d "docs/announcements/images" ] || [ -z "$(ls -A docs/announcements/images)" ]; then
    echo "❌ No photos found in docs/announcements/images/"
    echo "Please copy your photos there first!"
    exit 1
fi

echo "✅ Security check passed"
echo "✅ Photos found"

# Git operations
echo "📦 Staging changes..."
git add docs/announcements/
git add README.md CHANGELOG.md LICENSE.md

echo "💾 Committing..."
git commit -m "Release v2.0.0: Telemetry Framework

- Complete documentation overhaul
- Add comprehensive announcement materials  
- Add launch photos and guides
- Rebrand from ESP8266_TFT_MiniGrafx_Weather_UI to Telemetry
- Ready for public launch"

echo "🚀 Pushing to GitHub..."
git push origin main

echo ""
echo "✅ DONE! Repository updated."
echo ""
echo "Next steps:"
echo "1. Create GitHub release: https://github.com/MaliMrav/Telemetry/releases/new"
echo "2. Update About section: https://github.com/MaliMrav/Telemetry"
echo "3. Follow docs/announcements/REDDIT_POSTING_GUIDE.md"
echo ""
echo "You're ready to post to Reddit tonight! 🎉"
```

Save this as `deploy.sh`, make it executable, and run it:

```bash
chmod +x deploy.sh
./deploy.sh
```

---

## Quick Reference

**Your GitHub:** https://github.com/MaliMrav/Telemetry

**Files you need:**
- `docs/announcements/PRE_LAUNCH_CLEANUP.md` - Pre-launch checklist
- `docs/announcements/REDDIT_POSTING_GUIDE.md` - Step-by-step Reddit guide
- `docs/announcements/REDDIT_POST.md` - Post content

**After pushing, do:**
1. Create GitHub release v2.0.0
2. Update About section
3. Post to Reddit using the guide

---

**You're almost there! 🚀**
