# Reddit Posting Guide - Step by Step

## 📱 TONIGHT'S REDDIT POST - DETAILED INSTRUCTIONS

---

## Before You Start

### Prerequisites:
- ✅ GitHub repository is clean and public
- ✅ Release v2.0.0 is published
- ✅ Photos are ready (4 photos available)
- ✅ You have 1-2 hours to respond to comments
- ✅ You're logged into Reddit

---

## STEP 1: Choose Your Subreddit

**For first post tonight, start with:** r/esp8266

**Why this one first:**
- Most directly relevant to your hardware
- Active community (62k+ members)
- Appreciates technical projects
- Good engagement with DIY projects

**Save for later (24 hours apart):**
- r/homeassistant (post tomorrow night)
- r/homeautomation (post in 2 days)

---

## STEP 2: Prepare Your Post Content

### Open this file:
```
/Users/vladimir/Documents/GitHub/telemetry/docs/announcements/REDDIT_POST.md
```

### Modifications needed:

1. **Replace GitHub placeholder:**
   Change: `[Add your GitHub URL here]`
   To: `https://github.com/MaliMrav/Telemetry`

2. **Customize opening (optional but recommended):**
   Change: "I've just released"
   To: "I've spent the last few months building" (sounds more personal)

3. **Add your photos** (see Step 4)

---

## STEP 3: Navigate to r/esp8266

1. Go to: https://reddit.com/r/esp8266
2. Look for "Create Post" button (top right or sidebar)
3. Click "Create Post"

---

## STEP 4: Create the Post

### Title (Choose one):

**Option A - Technical Focus:**
```
Telemetry v2.0: MQTT Dashboard Framework with Modular Architecture for ESP8266/ESP32
```

**Option B - Story Focus:**
```
Built an MQTT Dashboard Framework for ESP8266 - From 500-Line Sketch to Modular Architecture
```

**Option C - Feature Focus:**
```
Open-Source MQTT Dashboard for ESP8266/ESP32 with Touch Calibration, OTA Updates, and Modular Design
```

**Recommendation:** Use Option B - it tells a story and appeals to the journey/learning aspect

### Post Type:
- Select "Image & Video" (if posting photos directly)
- OR select "Link" (if linking to GitHub with text post)
- OR select "Text" (if uploading photos within text)

**Recommendation:** Use "Image & Video" for maximum visibility

---

## STEP 5: Add Content

### If using "Image & Video" post:

1. **Add your main photo first:**
   - Upload `weatherscreen.jpg` as the primary image
   - This shows in thumbnail and at top of post

2. **Add post text:**
   - Copy the entire body from REDDIT_POST.md
   - Paste into the text box
   - Make sure GitHub links are clickable

3. **Add additional photos in post body:**
   - Click the image icon in the editor
   - Upload your other 3 photos
   - Place them where the placeholders are in the text

### If using "Text" post:

1. **Copy entire body from REDDIT_POST.md**
2. **Paste into text editor**
3. **Upload photos using Reddit's image upload:**
   - Click image icon in editor
   - Upload all 4 photos
   - Place them throughout the post where placeholders indicate

---

## STEP 6: Add Flair (Important!)

r/esp8266 flair options - choose one:
- **"Project"** ← BEST CHOICE
- "Help"
- "Discussion"
- Other (check what's available)

**How to add flair:**
1. Look for "FLAIR" dropdown or button
2. Select "Project" or most appropriate option
3. Flair helps categorize and increases visibility

---

## STEP 7: Final Post Preview

Before clicking "Post", verify:

- [ ] Title is compelling and clear
- [ ] Main photo (WeatherScreen) is visible
- [ ] GitHub link is correct: https://github.com/MaliMrav/Telemetry
- [ ] Text is formatted properly (no weird line breaks)
- [ ] Additional photos are in the right places
- [ ] Flair is selected
- [ ] No typos in title or first paragraph

---

## STEP 8: Post It! 🚀

1. Click "Post" button
2. Post will go live immediately
3. Copy the URL of your new post for reference

---

## STEP 9: Immediate Post-Post Actions (First 15 Minutes)

### Critical for visibility:

1. **Refresh and check formatting:**
   - View your post as other users see it
   - Make sure all photos loaded
   - Check that links are clickable

2. **Pin yourself to notifications:**
   - Turn on notifications for your post
   - Respond to first comment within 5-10 minutes

3. **Add a top comment (recommended):**
   Post a comment on your own post with:
   ```
   Additional info:
   
   GitHub: https://github.com/MaliMrav/Telemetry
   Release: https://github.com/MaliMrav/Telemetry/releases/tag/v2.0.0
   
   Happy to answer any questions about the architecture, porting to different 
   hardware, or adding new data sources!
   
   Full documentation including architecture guide and display porting guide 
   is in the repo.
   ```

4. **Engage early:**
   - First 15 minutes are critical for Reddit algorithm
   - Reply to any early comments immediately
   - Be friendly and helpful
   - Upvote questions (encourages discussion)

---

## STEP 10: First Hour Response Strategy

### Types of comments you'll likely get:

**1. "This is cool!" / "Nice work!"**
Response template:
```
Thanks! It was a fun journey from single-file sketch to modular architecture. 
Let me know if you have any questions about implementation!
```

**2. "Can I use this with [different hardware]?"**
Response template:
```
Absolutely! It's designed to be portable. Check out docs/DISPLAY_CONFIGURATION.md 
in the repo for porting guide. The main things you'd need to adjust are pin 
assignments and display driver config. Let me know if you run into any issues!
```

**3. "How does [technical detail] work?"**
- Answer directly and concisely
- Link to relevant doc file if detailed explanation exists
- Offer to add more docs if it's a common question

**4. "Cool but why not just [alternative approach]?"**
- Don't be defensive
- Explain your reasoning
- Acknowledge alternatives are valid
- Example: "That's definitely a valid approach! I went with [X] because [reasons]. 
  Both have trade-offs depending on your use case."

**5. Technical questions about implementation:**
- Answer with code examples if helpful
- Link to specific files in GitHub
- Be honest if something is "in progress" or "planned"

**6. "Where can I buy one?" / "Do you sell these?"**
Response:
```
This is an open-source project - all code and docs are on GitHub (link above). 
Hardware is standard ESP8266 dev boards and ILI9341 displays available on 
Amazon/AliExpress/eBay. Check the README for full hardware list!
```

---

## STEP 11: Ongoing Engagement (Next 24 Hours)

### Check post:
- Every 30 minutes for first 2 hours
- Every 2-3 hours for rest of day
- Before bed (respond to late comments)
- First thing next morning

### What to track:
- Upvote count (100+ is good engagement)
- Comment count (active discussion)
- Questions you should add to FAQ
- Feature requests (note for GitHub issues)

### If post gets low engagement:
- Don't worry - sometimes timing is everything
- Engage with every comment to boost activity
- Share on other platforms (Twitter) linking back
- Try again on different subreddit in 24 hours

---

## STEP 12: Cross-Promotion (Optional)

**After post has been live for 2-3 hours:**

1. **Tweet about it:**
   ```
   Just posted my Telemetry v2.0 project to r/esp8266!
   
   MQTT dashboard framework with modular architecture for ESP8266/ESP32
   
   Reddit: [your reddit post URL]
   GitHub: https://github.com/MaliMrav/Telemetry
   
   #ESP8266 #IoT #MQTT
   ```

2. **Share in Discord servers (if you're in any):**
   - Arduino Discord
   - ESP8266/ESP32 communities
   - Maker/IoT communities
   - Just drop the Reddit link, don't spam

---

## Common Issues and Solutions

### Issue: Post gets caught in spam filter
**Symptoms:** Post doesn't appear in "new" on subreddit
**Solution:** 
- Wait 10 minutes
- Message subreddit moderators politely
- Ask if post needs approval

### Issue: Formatting looks wrong
**Solution:**
- Edit post (click "..." → "Edit")
- Fix formatting
- Changes save automatically

### Issue: Photos don't show up
**Solution:**
- Edit post
- Re-upload photos
- Or use imgur links instead

### Issue: Getting negative comments
**Solution:**
- Stay professional and friendly
- Don't take it personally
- Address technical concerns factually
- Ignore trolls (don't feed them)

---

## Success Metrics for Tonight

**Good first post results:**
- 50+ upvotes in first 24 hours
- 10+ comments with engagement
- 2-3 GitHub stars
- Some technical discussion

**Great first post results:**
- 100+ upvotes
- 20+ comments
- 5-10 GitHub stars
- Feature requests or collaboration offers

**Viral (rare but possible):**
- 500+ upvotes
- Front page of subreddit
- 50+ GitHub stars
- Multiple collaboration offers

---

## Post-Posting Checklist

After posting, come back here and check:

- [ ] Post is live and visible on r/esp8266
- [ ] Photos are showing correctly
- [ ] Links are clickable
- [ ] Posted top-level comment with additional info
- [ ] Responded to first comment within 15 minutes
- [ ] Set up notifications
- [ ] Noted common questions for FAQ
- [ ] Checked GitHub for any new stars/issues
- [ ] Planned tomorrow's r/homeassistant post

---

## Tomorrow's Post (r/homeassistant)

**24 hours after first post:**

1. Use same content from REDDIT_POST.md
2. Adjust emphasis to Home Assistant integration:
   - Lead with "integrates with Home Assistant via MQTT"
   - Emphasize presentation-only philosophy (HA does calculations)
   - Show how it displays HA sensor data
3. Post to r/homeassistant
4. Use flair "Project Showcase" or similar
5. Repeat engagement strategy

---

## You've Got This! 💪

Remember:
- Be helpful and friendly
- Respond promptly to comments
- Don't stress about upvotes
- Focus on meaningful engagement
- Your project is solid - let it speak for itself

**Good luck with tonight's post!** 🚀

---

## Quick Reference

**Your Links:**
- GitHub: https://github.com/MaliMrav/Telemetry
- Release: https://github.com/MaliMrav/Telemetry/releases/tag/v2.0.0

**Post Title (recommended):**
Built an MQTT Dashboard Framework for ESP8266 - From 500-Line Sketch to Modular Architecture

**Subreddit:** r/esp8266

**Timing:** 7pm-9pm EST tonight (or 9am-12pm tomorrow for peak hours)

**Time commitment:** 1-2 hours for responses tonight
