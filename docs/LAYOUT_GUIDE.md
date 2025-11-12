# Weather Display Layout Guide

## Visual Layout Overview

```
┌─────────────────────────────── 800px ────────────────────────────────┐
│                                                                       │
│  Sebastopol, CA                                         71°F         │ 10px
│  Tue, Nov 11, 1:54:08 PM                         Humidity: 55%       │ 50px
│  Sunrise: 6:49AM    Sunset: 5:01PM               Pressure 762 mmHg   │ 75px
│                                                   Feels like: 70°F    │
│                                                                       │
│                                                                       │
│                              ☁                                        │ 60px
│                          (64pt icon)                                  │
│                                                                       │
│                           Overcast                                    │ 135px
│                         (20pt text)                                   │
│                                                                       │
│  Next 12 Hours                                                        │ 130px
│  ┌────────────────────────────────────────────────────────────────┐ │
│  │  ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐        │ │ 155px
│  │  │1:00PM│ │3:00PM│ │5:00PM│ │7:00PM│ │9:00PM│ │11:0PM│        │ │
│  │  │ 73°F │ │ 65°F │ │ 54°F │ │ 53°F │ │ 52°F │ │ 53°F │        │ │
│  │  │  ☁   │ │  ☁   │ │  ☁   │ │  ☁   │ │  ☁   │ │  ☁   │        │ │
│  │  │Wind:5│ │Wind:8│ │Wind:13│ │Wind:6│ │Wind:5│ │Wind:5│        │ │
│  │  │Prc:0│ │Prc:0│ │Prc:0│ │Prc:0│ │Prc:0│ │Prc:0│        │ │
│  │  └──────┘ └──────┘ └──────┘ └──────┘ └──────┘ └──────┘        │ │
│  └────────────────────────────────────────────────────────────────┘ │
│                                                                       │ 260px
│  7-Day Forecast                                                       │ 270px
│  ┌────────────────────────────────────────────────────────────────┐ │
│  │ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐                     │ │ 295px
│  │ │Today│Wed │Thu │Fri │Sat │Sun │Mon │                     │ │
│  │ │     │     │     │     │     │     │     │                     │ │
│  │ │ ☁   │ ⛆   │ ⛆   │ ☁   │ ☁   │ ⛆   │ ⛆   │                     │ │
│  │ │     │     │     │     │     │     │     │                     │ │
│  │ │H73° │H60° │H59° │H62° │H66° │H55° │H58° │                     │ │
│  │ │L50°F│L53°F│L49°F│L47°F│L46°F│L48°F│L46°F│                     │ │
│  │ │Prc: │Prc: │Prc: │Prc: │Prc: │Prc: │Prc: │                     │ │
│  │ │0.00"│0.07"│0.61"│0.98"│0.00"│0.21"│0.85"│                     │ │
│  │ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘                     │ │
│  └────────────────────────────────────────────────────────────────┘ │
│                                                                       │ 420px
│  Last 24Hr: Rain: 0.00", H: 73°, L: 50°.  YTD: Max High: 95°,...    │ 430px
│                                                                       │
└───────────────────────────────────────────────────────────────────────┘
                                480px
```

---

## Element Specifications

### Header Section (Top)
| Element | Position | Font | Size | Color |
|---------|----------|------|------|-------|
| Location | (10, 10) | Montserrat 32 | - | White (#FFFFFF) |
| Date/Time | (10, 50) | Montserrat 16 | - | Gray (#C0C0C0) |
| Sunrise | (10, 75) | Montserrat 14 | - | Gray (#C0C0C0) |
| Sunset | (180, 75) | Montserrat 14 | - | Gray (#C0C0C0) |

### Current Temperature (Top-Right)
| Element | Position | Font | Size | Color |
|---------|----------|------|------|-------|
| Temperature | Align Right (-10, 10) | Montserrat 48 | - | Orange (#FFA500) |
| Humidity | Align Right (-10, 70) | Montserrat 12 | - | Gray (#C0C0C0) |
| Pressure | Align Right (-10, 87) | Montserrat 12 | - | Gray (#C0C0C0) |
| Feels Like | Align Right (-10, 104) | Montserrat 12 | - | Gray (#C0C0C0) |

### Main Weather Display (Center) **← NEW!**
| Element | Position | Font | Size | Color |
|---------|----------|------|------|-------|
| **Weather Symbol** | **Align Center (0, 60)** | **Weather 64pt** | **~84px height** | **Dynamic** |
| **Condition Text** | **Align Center (0, 135)** | **Montserrat 20** | **~24px height** | **White (#FFFFFF)** |

### Hourly Forecast Section
| Element | Position | Font | Size | Color |
|---------|----------|------|------|-------|
| Title | (20, 130) | Montserrat 16 | - | White (#FFFFFF) |
| Container | (20, 155) | - | 760x105 | Dark Blue (#1A3A52) |
| Cards (6x) | Flex Row | - | 120x100 each | Light Blue (#3A5A72) |

#### Hourly Card Contents
| Element | Alignment | Font | Color |
|---------|-----------|------|-------|
| Time | Top Mid | Montserrat 12 | White (#FFFFFF) |
| Temperature | Top Mid (+18) | Montserrat 20 | Orange (#FFA500) |
| Symbol | Top Mid (+43) | **Weather 36pt** | **Dynamic** |
| Details | Bottom Mid | Montserrat 10 | Dim Gray (#808080) |

### Daily Forecast Section
| Element | Position | Font | Size | Color |
|---------|----------|------|------|-------|
| Title | (20, 270) | Montserrat 16 | - | White (#FFFFFF) |
| Container | (20, 295) | - | 760x125 | Dark Blue (#1A3A52) |
| Cards (7x) | Flex Row | - | 105x120 each | Light Blue (#3A5A72) |

#### Daily Card Contents
| Element | Alignment | Font | Color |
|---------|-----------|------|-------|
| Day/Date | Top Mid | Montserrat 10 | Gray (#C0C0C0) |
| Symbol | Top Mid (+30) | **Weather 36pt** | **Dynamic** |
| Temperatures | Top Mid (+70) | Montserrat 12 | Gray (#C0C0C0) |
| Precipitation | Bottom Mid | Montserrat 10 | Dim Gray (#808080) |

### Statistics Bar (Bottom)
| Element | Position | Font | Size | Color |
|---------|----------|------|------|-------|
| Stats Text | (20, 430) | Montserrat 12 | - | Gray (#C0C0C0) |

---

## Font Size Visual Reference

```
Weather Symbols:
64pt: ☼  ← Main current weather (VERY LARGE)
36pt: ☼  ← Hourly and daily cards (MEDIUM)

Text Fonts:
48pt: 71°F                    ← Temperature
32pt: Sebastopol, CA          ← Location
20pt: Overcast                ← Weather condition text
16pt: Next 12 Hours           ← Section headers
14pt: Sunrise: 6:49AM         ← Time labels
12pt: Humidity: 55%           ← Data labels
10pt: Wind: 5 mph             ← Detail text
```

---

## Spacing and Alignment

### Main Weather Display Spacing
```
                Y = 60
                  ↓
              ┌───────┐
              │       │
              │   ☁   │ ← 64pt symbol (height ~84px)
              │       │
              └───────┘
                  ↓
                Y = 135 (60 + 75)
                
              Overcast  ← 20pt text (height ~24px)
```

**Formula:** Text Y position = Symbol Y position + 75

### Card Spacing
```
Hourly Cards:
┌─3px─┬──120px──┬─3px─┬──120px──┬─3px─┬──120px──┬─3px─┐
│ gap │  Card1  │ gap │  Card2  │ gap │  Card3  │ ... │
└─────┴─────────┴─────┴─────────┴─────┴─────────┴─────┘

Daily Cards:
┌─3px─┬──105px──┬─3px─┬──105px──┬─3px─┬──105px──┬─3px─┐
│ gap │  Card1  │ gap │  Card2  │ gap │  Card3  │ ... │
└─────┴─────────┴─────┴─────────┴─────┴─────────┴─────┘
```

---

## Color Scheme

### Background Colors
```cpp
#define COLOR_BG_DARK    0x1A3A52  // Main background
#define COLOR_BG_LIGHT   0x3A5A72  // Cards
```

### Text Colors
```cpp
#define COLOR_TEXT_PRIMARY   0xFFFFFF  // White - Main text
#define COLOR_TEXT_SECONDARY 0xC0C0C0  // Light Gray - Labels
#define COLOR_TEXT_DIM       0x808080  // Gray - Details
#define COLOR_ACCENT         0xFFA500  // Orange - Temperature
```

### Dynamic Symbol Colors
```cpp
#define COLOR_SUN        0xFFA500  // Orange - Clear/Sunny
#define COLOR_CLOUD      0xC0C0C0  // Light Gray - Cloudy
#define COLOR_RAIN       0x5599FF  // Blue - Rain
#define COLOR_SNOW       0xFFFFFF  // White - Snow
#define COLOR_THUNDER    0xFFDD00  // Yellow - Storms
```

---

## Symbol Size Comparison in Display

### Current Weather (Center)
```
Before (48pt):        After (64pt):
     ☼                    ☼
   Medium              VERY LARGE
  ~63px tall           ~84px tall
```

### Hourly Cards (36pt) - Unchanged
```
    ☁
  Medium
 ~47px tall
```

### Daily Cards (36pt) - Unchanged
```
    ☁
  Medium
 ~47px tall
```

---

## Responsive Layout Notes

### Main Display Centering
All center-aligned elements use:
```cpp
lv_obj_align(element, LV_ALIGN_TOP_MID, X_offset, Y_offset);
```
Where:
- `LV_ALIGN_TOP_MID` = horizontally centered
- `X_offset = 0` = perfect center
- `Y_offset` = vertical position from top

### Flex Layout for Cards
Both hourly and daily containers use:
```cpp
lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(container, 
    LV_FLEX_ALIGN_SPACE_EVENLY,  // Horizontal: equal spacing
    LV_FLEX_ALIGN_START,          // Vertical: align to top
    LV_FLEX_ALIGN_START);         // Content: start
```

---

## Customization Examples

### Move Main Symbol Higher
```cpp
// Current:
lv_obj_align(currentCondLabel, LV_ALIGN_TOP_MID, 0, 60);

// Move up 10 pixels:
lv_obj_align(currentCondLabel, LV_ALIGN_TOP_MID, 0, 50);

// Also adjust text:
lv_obj_align(currentCondTextLabel, LV_ALIGN_TOP_MID, 0, 125);
```

### Make Text Label Larger
```cpp
// Current (20pt):
lv_obj_set_style_text_font(currentCondTextLabel, &lv_font_montserrat_20, 0);

// Change to 24pt:
lv_obj_set_style_text_font(currentCondTextLabel, &lv_font_montserrat_24, 0);

// Adjust position if needed:
lv_obj_align(currentCondTextLabel, LV_ALIGN_TOP_MID, 0, 140);
```

### Hide Text Label (Symbol Only)
```cpp
// In updateWeatherDisplay(), comment out:
// lv_label_set_text(currentCondTextLabel, getWeatherCondition(weather.currentCode).c_str());

// Or set visibility:
lv_obj_add_flag(currentCondTextLabel, LV_OBJ_FLAG_HIDDEN);
```

---

## Screen Real Estate Usage

```
Total Height: 480px

Section Breakdown:
- Header (0-105px): 105px (22%)
- Main Weather (60-160px): 100px (21%)
- Hourly (130-260px): 130px (27%)
- Daily (270-420px): 150px (31%)
- Stats (430-450px): 20px (4%)
- Padding/Gaps: ~75px (16%)
```

The 64pt symbol with text label uses approximately 100px of vertical space, making it the most prominent element on the display while still leaving room for all forecast data.


