# 🚀 Deployment Guide

## 📦 Required Files (4 Total)

Place all these files in the **same folder**:

```
YourSketchFolder/
├── esp32_weather_advanced.ino          ← Updated code (DOWNLOAD BELOW)
├── lv_font_weather_symbols_36.c       ← You already have this
├── lv_font_weather_symbols_48.c       ← You already have this
└── lv_font_weather_symbols_64.c       ← You already have this
```

---


## 🖥️ What Your Display Will Look Like

```
┌────────────────────────────────────────────┐
│ Sebastopol, CA                   71°F     │
│ Tue, Nov 11, 1:54:08 PM    Humidity: 55%  │
│ Sunrise: 6:49AM  Sunset: 5:01PM           │
│                                            │
│                                            │
│                  ☁                         │  ← 64pt (LARGE!)
│               Overcast                     │  ← 20pt text
│                                            │
│ ┌────── Next 12 Hours ──────┐             │
│ │  1PM   3PM   5PM   7PM  ...│             │
│ │  73°F  65°F  54°F  53°F ...│             │
│ │  ☁    ☁    ☁    ☁   ...│ (36pt)       │
│ └─────────────────────────────┘            │
│                                            │
│ ┌──── 7-Day Forecast ────┐                │
│ │ Today  Wed   Thu   Fri ...│              │
│ │  ☁     ⛆    ⛆    ☁  ...│ (36pt)       │
│ │ H73°  H60°  H59°  H62° ...│              │
│ └─────────────────────────────┘            │
│                                            │
│ Last 24Hr: Rain: 0.00", H: 73°, L: 50°... │
└────────────────────────────────────────────┘
```



---

## 🔧 Upload Instructions

### Step 1: Organize Files
1. Create a new folder: `WeatherDisplay_64pt`
2. Copy the 4 required files into it

### Step 2: Open in Arduino IDE
1. Open `esp32_weather_symbols.ino`
2. Arduino IDE will automatically detect the 3 font files (.c)

### Step 3: Verify Compilation
1. Click the ✓ (Verify) button
2. Should compile without errors
3. Look for "Sketch uses XX bytes" message

### Step 4: Upload
1. Connect your ESP32-S3 display via USB
2. Select correct board and port
3. Click → (Upload) button
4. Wait for "Done uploading" message

### Step 5: Watch It Run!
1. Display will initialize
2. Connect to WiFi
3. Fetch weather data
4. Display beautiful 64pt symbols!

---

## ⚡ Quick Test Checklist

After uploading, verify:

- [ ] Display powers on and shows UI
- [ ] WiFi connects successfully
- [ ] Large 64pt weather symbol appears in center
- [ ] Text label appears below symbol ("Clear", "Overcast", etc.)
- [ ] Symbol color matches weather condition
- [ ] Temperature shows in top-right
- [ ] Hourly forecast shows 6 cards with symbols
- [ ] Daily forecast shows 7 cards with symbols
- [ ] Statistics appear at bottom
- [ ] Time updates every second

---

## 🎨 Customization Quick Reference

### Make Symbol Larger/Smaller
Not possible without creating new font - 64pt is maximum provided.

### Move Symbol Higher/Lower
```cpp
// Line 237 - Change Y value (currently 60)
lv_obj_align(currentCondLabel, LV_ALIGN_TOP_MID, 0, 60);
//                                                    ↑
// Try: 50 (higher) or 70 (lower)
```

### Move Text Label
```cpp
// Line 245 - Change Y value (currently 135)
lv_obj_align(currentCondTextLabel, LV_ALIGN_TOP_MID, 0, 135);
//                                                        ↑
// Rule: Symbol Y + 75 = good text position
```

### Change Text Size
```cpp
// Line 243 - Change font (currently montserrat_20)
lv_obj_set_style_text_font(currentCondTextLabel, &lv_font_montserrat_20, 0);
//                                                  ↑
// Try: lv_font_montserrat_16 (smaller)
// Or:  lv_font_montserrat_24 (larger)
```

### Hide Text Label
```cpp
// Line 543 - Comment out or remove:
// lv_label_set_text(currentCondTextLabel, getWeatherCondition(weather.currentCode).c_str());
```

### Change Symbol Color
```cpp
// Lines 47-51 - Modify colors:
#define COLOR_SUN        0xFFFF00  // Yellow instead of orange
#define COLOR_RAIN       0x0066FF  // Deeper blue
```

---

## 📊 Technical Specifications

### Font Sizes Used
| Size | Purpose | Element |
|------|---------|---------|
| 64pt | Main weather icon | Current condition symbol |
| 36pt | Card icons | Hourly/Daily symbols |
| 48pt | Temperature | Large temp display |
| 32pt | Location | Header |
| 20pt | Condition text | Current condition name |
| 16pt | Section headers | "Next 12 Hours" |
| 14pt | Sun times | Sunrise/Sunset |
| 12pt | Data labels | Humidity, pressure, etc. |
| 10pt | Details | Wind, precipitation |


### Display Positions
- Main symbol: Center X, Y=60
- Condition text: Center X, Y=135
- Temperature: Right-align X=-10, Y=10
- Hourly section: Y=155
- Daily section: Y=295

---

## 🐛 Troubleshooting

### Issue: Won't Compile
**Error:** "lv_font_weather_symbols_64 was not declared"

**Solution:**
1. Verify `lv_font_weather_symbols_64.c` is in same folder
2. Restart Arduino IDE
3. Try "Sketch → Show Sketch Folder" to confirm location

### Issue: Symbol Shows as "?"
**Cause:** Font file not loaded properly

**Solution:**
1. Check all 4 files are in same folder
2. Verify file names are exact (case-sensitive)
3. Re-upload sketch

### Issue: Text Label Not Visible
**Cause:** Position may be off-screen

**Solution:**
1. Check Y position (line 245) - should be 135
2. Try adjusting: change to 140 or 130
3. Verify text color is not same as background

### Issue: Symbol Too Large
**Note:** 64pt is the largest provided font

**Solution:**
1. Can revert to 48pt by changing line 237
2. Or adjust card layout to accommodate

### Issue: Display Frozen
**Cause:** Possible memory issue or WiFi problem

**Solution:**
1. Check Serial Monitor (115200 baud)
2. Verify WiFi credentials
3. Press reset button on ESP32

---

## 📞 Need Help?

If you encounter issues:

1. **Check Serial Monitor** - Most helpful for debugging
   - Set baud rate to 115200
   - Look for error messages
   - Check WiFi connection status

2. **Verify Files**
   - All 4 files in same folder?
   - Exact file names?
   - No extra spaces or typos?

3. **Test Original Code**
   - Does your old version still work?
   - If yes, issue is with new code
   - If no, hardware/library issue

4. **Review Guides**
   - Check LAYOUT_GUIDE.md for positioning
   - Review README_64pt.md for features
   - Consult SYMBOL_REFERENCE.md for symbols


---

## 🔄 Future Enhancements

Ideas for v2.0:

- [ ] Add animations (fade transitions)
- [ ] Touch controls for detailed views
- [ ] Weather alerts/warnings
- [ ] Historical weather charts
- [ ] Multiple location support
- [ ] Custom themes/color schemes
- [ ] Voice announcements
- [ ] Integration with smart home


