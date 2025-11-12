#!/bin/bash

# Check if correct number of arguments provided
if [ "$#" -ne 2 ]; then
    echo "Error: Missing required arguments"
    echo ""
    echo "Usage: $0 <font_size> <font_name>"
    echo ""
    echo "Arguments:"
    echo "  font_size   - Size of the font (e.g., 64, 48, 32)"
    echo "  font_name   - Name of the font file (e.g., NotoSansSymbols2-Regular.ttf)"
    echo ""
    echo "Example:"
    echo "  $0 64 NotoSansSymbols2-Regular.ttf"
    exit 1
fi

FONT_SIZE=$1
FONT_NAME=$2
OUTPUT="lv_font_weather_symbols_${FONT_SIZE}.c"
LVGL_INCLUDE="/Users/clarkh/Documents/Arduino/libraries/lvgl/src/lvgl.h"

# Check if font file exists
if [ ! -f "$FONT_NAME" ]; then
    echo "Error: Font file '$FONT_NAME' not found!"
    exit 1
fi

echo "Generating font with size $FONT_SIZE from $FONT_NAME..."

lv_font_conv \
  --font "$FONT_NAME" \
  --size "$FONT_SIZE" --bpp 4 --no-compress \
  --range 0x20-0x7E,0x2600-0x26FF --format lvgl \
  --lv-include "$LVGL_INCLUDE" \
  -o "$OUTPUT"

if [ $? -eq 0 ]; then
    echo "Success! Font generated: $OUTPUT"
else
    echo "Error: Font generation failed"
    exit 1
fi
