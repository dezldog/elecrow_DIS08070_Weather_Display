# Weather Symbol Reference Guide

## Complete Symbol Chart

This document shows all weather symbols available in your fonts and their Unicode codes.

---

## Weather Symbols Used in Your Display

### Primary Condition Symbols

| Symbol | Unicode | Hex | Decimal | Description | Used For |
|--------|---------|-----|---------|-------------|----------|
| ☀ | U+2600 | \u2600 | 9728 | Black Sun | Alternative for clear |
| ☁ | U+2601 | \u2601 | 9729 | Cloud | Overcast, Fog |
| ☂ | U+2602 | \u2602 | 9730 | Umbrella | Drizzle |
| ☃ | U+2603 | \u2603 | 9731 | Snowman | Snow |
| ☼ | U+263C | \u263C | 9788 | White Sun with Rays | Clear, Mostly Clear |
| ⛅ | U+26C5 | \u26C5 | 9925 | Sun Behind Cloud | Partly Cloudy |
| ⛆ | U+26C6 | \u26C6 | 9926 | Rain | Rain, Showers |
| ⛈ | U+26C8 | \u26C8 | 9928 | Thunder Cloud & Rain | Thunderstorm |

### Alternative Symbols Available

| Symbol | Unicode | Hex | Description | Potential Use |
|--------|---------|-----|-------------|---------------|
| ☄ | U+2604 | \u2604 | Comet | Special events |
| ★ | U+2605 | \u2605 | Black Star | Night clear |
| ☆ | U+2606 | \u2606 | White Star | Night partly cloudy |
| ☇ | U+2607 | \u2607 | Lightning | Storm alternative |
| ☈ | U+2608 | \u2608 | Thunderstorm | Storm alternative |
| ☉ | U+2609 | \u2609 | Sun | Clear alternative |
| ⛇ | U+26C7 | \u26C7 | Black Snowman | Heavy snow |

---

## Current Symbol Mapping

### Based on Open-Meteo Weather Codes

```cpp
Code 0-1:  Clear/Mostly Clear    → ☼  (U+263C) Sun with Rays
Code 2:    Partly Cloudy         → ⛅ (U+26C5) Sun Behind Cloud
Code 3:    Overcast              → ☁  (U+2601) Cloud
Code 45-48: Fog                  → ☁  (U+2601) Cloud
Code 51-57: Drizzle              → ☂  (U+2602) Umbrella
Code 61-67: Rain                 → ⛆  (U+26C6) Rain
Code 80-82: Showers              → ⛆  (U+26C6) Rain
Code 71-77: Snow                 → ☃  (U+2603) Snowman
Code 85-86: Snow Showers         → ☃  (U+2603) Snowman
Code 95-99: Thunderstorm         → ⛈  (U+26C8) Thunder Cloud
```

---

## How to Change Symbols

### In Your Code

Edit the `getWeatherSymbol()` function (around line 159):

```cpp
String getWeatherSymbol(int code) {
  if (code == 0) return "\u263C";  // ← Change this Unicode
  if (code == 1) return "\u263C";
  if (code == 2) return "\u26C5";
  // etc...
}
```

### Example Modifications

**Want a different clear sky symbol?**
```cpp
// Original:
if (code == 0) return "\u263C";  // ☼ Sun with Rays

// Option 1 - Black Sun:
if (code == 0) return "\u2600";  // ☀ Black Sun

// Option 2 - Simple Sun:
if (code == 0) return "\u2609";  // ☉ Sun
```

**Want different rain symbols?**
```cpp
// Original:
if (code >= 61 && code <= 67) return "\u26C6";  // ⛆ Rain

// Alternative - Use umbrella:
if (code >= 61 && code <= 67) return "\u2602";  // ☂ Umbrella

// Note: Your drizzle already uses umbrella
```

**Want to show different symbols for light vs heavy rain?**
```cpp
// Light rain (61-63):
if (code >= 61 && code <= 63) return "\u2602";  // ☂ Umbrella

// Heavy rain (64-67):
if (code >= 64 && code <= 67) return "\u26C6";  // ⛆ Rain

// Showers (80-82):
if (code >= 80 && code <= 82) return "\u26C6";  // ⛆ Rain
```

---

## Extended Weather Symbols Available

Your fonts include many more symbols in the U+2600-U+26FF range:

### Celestial (U+2600-U+2618)
- ☀ ☁ ☂ ☃ ☄ ★ ☆ ☇ ☈ ☉ ☊ ☋ ☌ ☍ ☎ ☏
- ☐ ☑ ☒ ☓ ☔ ☕ ☖ ☗ ☘ ☙ ☚ ☛ ☜ ☝ ☞ ☟

### Additional Symbols (U+2619-U+263F)
- ☠ ☡ ☢ ☣ ☤ ☥ ☦ ☧ ☨ ☩ ☪ ☫ ☬ ☭ ☮ ☯
- ☰ ☱ ☲ ☳ ☴ ☵ ☶ ☷ ☸ ☹ ☺ ☻ ☼ ☽ ☾ ☿

### Extended Miscellaneous (U+26A0-U+26CF)
- ⚠ ⚡ ⚢ ⚣ ⚤ ⚥ ⚦ ⚧ ⚨ ⚩ ⚪ ⚫ ⚬ ⚭ ⚮ ⚯
- ⚰ ⚱ ⚲ ⚳ ⚴ ⚵ ⚶ ⚷ ⚸ ⚹ ⚺ ⚻ ⚼ ⚽ ⚾ ⚿
- ⛀ ⛁ ⛂ ⛃ ⛄ ⛅ ⛆ ⛇ ⛈ ⛉ ⛊ ⛋ ⛌ ⛍ ⛎ ⛏

---

## Useful Non-Weather Symbols

These are also in your fonts and could be useful:

| Symbol | Unicode | Use Case |
|--------|---------|----------|
| ⚠ | U+26A0 | Weather warnings |
| ⚡ | U+26A1 | Lightning (alternative to ⛈) |
| ☠ | U+2620 | Hazardous conditions |
| ☢ | U+2622 | Air quality alerts |
| ⚪ | U+26AA | Indicators/bullets |
| ⚫ | U+26AB | Indicators/bullets |
| ☺ | U+263A | Pleasant weather mood |
| ☹ | U+2639 | Unpleasant weather mood |
| ☮ | U+262E | Calm conditions |

---

## Testing Symbols

### Quick Test Code

Add this to your `setup()` function to test symbol display:

```cpp
void testSymbols() {
  Serial.println("Testing weather symbols:");
  Serial.println("Clear: " + getWeatherSymbol(0));
  Serial.println("Partly Cloudy: " + getWeatherSymbol(2));
  Serial.println("Overcast: " + getWeatherSymbol(3));
  Serial.println("Fog: " + getWeatherSymbol(45));
  Serial.println("Drizzle: " + getWeatherSymbol(51));
  Serial.println("Rain: " + getWeatherSymbol(61));
  Serial.println("Snow: " + getWeatherSymbol(71));
  Serial.println("Thunderstorm: " + getWeatherSymbol(95));
}
```

---

## Symbol Display Tips

### Size Considerations

**48pt Font:**
- Best for single large icons
- Main current weather display
- Featured elements

**36pt Font:**
- Ideal for cards and small spaces
- Hourly forecast (6 cards)
- Daily forecast (7 cards)

### Color Combinations

Current color scheme:
```cpp
#define COLOR_SUN        0xFFA500  // Orange
#define COLOR_CLOUD      0xC0C0C0  // Light Gray
#define COLOR_RAIN       0x5599FF  // Blue
#define COLOR_SNOW       0xFFFFFF  // White
#define COLOR_THUNDER    0xFFDD00  // Yellow
```

Recommended alternatives:
```cpp
// Warmer sun
#define COLOR_SUN        0xFFCC00  // Golden Yellow

// Darker clouds
#define COLOR_CLOUD      0x808080  // Medium Gray

// Deeper blue rain
#define COLOR_RAIN       0x3366CC  // Deep Blue

// Icy snow
#define COLOR_SNOW       0xE0F0FF  // Light Blue-White

// Red lightning
#define COLOR_THUNDER    0xFF0000  // Red
```

---

## Unicode Syntax

### In Your Code
Use escape sequences:
```cpp
String symbol = "\u263C";  // ☼
```

### Testing in Browser/Editor
Copy/paste actual symbols:
```cpp
String symbol = "☼";  // Direct Unicode character
```

Both methods work, but `\uXXXX` is more portable.

---

## Advanced: Day vs Night Symbols

Want different symbols for day and night? Add this function:

```cpp
bool isNightTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return false;
  
  int hour = timeinfo.tm_hour;
  int sunriseHour = weather.sunriseTime.substring(0, 2).toInt();
  int sunsetHour = weather.sunsetTime.substring(0, 2).toInt();
  
  return (hour < sunriseHour || hour > sunsetHour);
}

String getWeatherSymbol(int code) {
  bool isNight = isNightTime();
  
  // Clear sky
  if (code == 0 || code == 1) {
    return isNight ? "\u2605" : "\u263C";  // ★ at night, ☼ during day
  }
  
  // Partly cloudy
  if (code == 2) {
    return isNight ? "\u2606" : "\u26C5";  // ☆ at night, ⛅ during day
  }
  
  // Rest of symbols unchanged...
}
```

---

## Symbol Fallbacks

If a symbol doesn't display:

1. **Check font file inclusion**
   ```cpp
   LV_FONT_DECLARE(lv_font_weather_symbols_36);
   LV_FONT_DECLARE(lv_font_weather_symbols_48);
   ```

2. **Verify Unicode is in range**
   - Your fonts cover U+2600-U+26FF
   - Outside this range = no symbol

3. **Test with simple text first**
   ```cpp
   lv_label_set_text(label, "X");  // Test with ASCII
   lv_label_set_text(label, "\u263C");  // Then try symbol
   ```

---

## Creating Custom Weather Profiles

### Beach Day Profile
```cpp
String getWeatherSymbolBeach(int code) {
  if (code == 0) return "\u263C";  // ☼ Perfect beach day
  if (code == 2) return "\u2601";  // ☁ Cloudy, maybe not ideal
  if (code >= 61) return "\u2602"; // ☂ Not a beach day
  return getWeatherSymbol(code);   // Default
}
```

### Skiing Profile
```cpp
String getWeatherSymbolSki(int code) {
  if (code >= 71 && code <= 77) return "\u2603"; // ☃ Perfect ski conditions!
  if (code == 0) return "\u263C";  // ☼ Good visibility
  if (code == 3) return "\u2601";  // ☁ Flat light
  return getWeatherSymbol(code);
}
```

---

## Summary

Your display now has access to **over 200 symbols** from the Unicode Miscellaneous Symbols block. The key weather symbols you're currently using are:

- ☼ (Sun) → Clear
- ⛅ (Sun/Cloud) → Partly Cloudy
- ☁ (Cloud) → Overcast/Fog
- ☂ (Umbrella) → Drizzle
- ⛆ (Rain) → Rain
- ☃ (Snowman) → Snow
- ⛈ (Thunder) → Storms

All symbols are color-coded and automatically scaled to fit your beautiful weather display!
