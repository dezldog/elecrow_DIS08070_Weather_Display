/*
 * ESP32-S3 Weather Display with Symbol Fonts
 * For ELECROW 7" ESP32 Display (DIS08070H) - 800x480 RGB
 * 
 * REQUIRED FILES (must be in same folder):
 * - lv_font_weather_symbols_36.c
 * - lv_font_weather_symbols_48.c
 * - lv_font_weather_symbols_64.c
 * - lv_font_weather_symbols_92.c
 * - lv_font_weather_symbols_120.c
 */

#include <lvgl.h>
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "time.h"

// External font declarations
LV_FONT_DECLARE(lv_font_weather_symbols_36);
LV_FONT_DECLARE(lv_font_weather_symbols_48);
LV_FONT_DECLARE(lv_font_weather_symbols_64);
LV_FONT_DECLARE(lv_font_weather_symbols_92);
LV_FONT_DECLARE(lv_font_weather_symbols_120);

// ===== CONFIGURATION =====
const char* ssid = "#########";
const char* password = "########";

const long gmtOffset_sec = -8 * 3600;
const int daylightOffset_sec = 3600;

const float latitude = 38.3986;
const float longitude = -122.8790;
const char* locationName = "Sebastopol, CA";

const char* ntpServer = "pool.ntp.org";
const unsigned long weatherUpdateInterval = 5 * 60 * 1000;

// ===== COLORS =====
#define COLOR_BG_DARK    0x1A3A52
#define COLOR_BG_LIGHT   0x3A5A72
#define COLOR_ACCENT     0xFFA500
#define COLOR_TEXT_PRIMARY   0xFFFFFF
#define COLOR_TEXT_SECONDARY 0xC0C0C0
#define COLOR_TEXT_DIM       0x808080

// Additional colors for weather symbols
#define COLOR_SUN        0xFFA500  // Orange for sun
#define COLOR_CLOUD      0xC0C0C0  // Gray for clouds
#define COLOR_RAIN       0x5599FF  // Blue for rain
#define COLOR_SNOW       0xFFFFFF  // White for snow
#define COLOR_THUNDER    0xFFDD00  // Yellow for thunder

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define TFT_BL 2

// LovyanGFX
class LGFX : public lgfx::LGFX_Device {
public:
  lgfx::Bus_RGB _bus_instance;
  lgfx::Panel_RGB _panel_instance;

  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.panel = &_panel_instance;
      cfg.pin_d0 = GPIO_NUM_15; cfg.pin_d1 = GPIO_NUM_7;
      cfg.pin_d2 = GPIO_NUM_6; cfg.pin_d3 = GPIO_NUM_5;
      cfg.pin_d4 = GPIO_NUM_4; cfg.pin_d5 = GPIO_NUM_9;
      cfg.pin_d6 = GPIO_NUM_46; cfg.pin_d7 = GPIO_NUM_3;
      cfg.pin_d8 = GPIO_NUM_8; cfg.pin_d9 = GPIO_NUM_16;
      cfg.pin_d10 = GPIO_NUM_1; cfg.pin_d11 = GPIO_NUM_14;
      cfg.pin_d12 = GPIO_NUM_21; cfg.pin_d13 = 47;
      cfg.pin_d14 = 48; cfg.pin_d15 = GPIO_NUM_45;
      cfg.pin_henable = GPIO_NUM_41;
      cfg.pin_vsync = GPIO_NUM_40;
      cfg.pin_hsync = GPIO_NUM_39;
      cfg.pin_pclk = GPIO_NUM_0;
      cfg.freq_write = 15000000;
      cfg.hsync_polarity = 0; cfg.hsync_front_porch = 40;
      cfg.hsync_pulse_width = 48; cfg.hsync_back_porch = 40;
      cfg.vsync_polarity = 0; cfg.vsync_front_porch = 1;
      cfg.vsync_pulse_width = 31; cfg.vsync_back_porch = 13;
      cfg.pclk_active_neg = 1; cfg.de_idle_high = 0;
      cfg.pclk_idle_high = 0;
      _bus_instance.config(cfg);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.memory_width = 800; cfg.memory_height = 480;
      cfg.panel_width = 800; cfg.panel_height = 480;
      cfg.offset_x = 0; cfg.offset_y = 0;
      _panel_instance.config(cfg);
    }
    _panel_instance.setBus(&_bus_instance);
    setPanel(&_panel_instance);
  }
};

LGFX lcd;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf[SCREEN_WIDTH * SCREEN_HEIGHT / 15];
static lv_disp_drv_t disp_drv;

// UI
lv_obj_t *locationLabel, *dateTimeLabel, *sunriseLabel, *sunsetLabel;
lv_obj_t *currentCondLabel, *currentCondTextLabel, *currentTempLabel;
lv_obj_t *humidityLabel, *pressureLabel, *feelsLikeLabel;
lv_obj_t *hourlyContainer, *dailyContainer, *statsLabel;

// Data structure for UI
struct {
  float currentTemp, feelsLike, humidity, pressure;
  int currentCode;  // Store weather code instead of string
  String sunriseTime, sunsetTime;
  float last24hRain, last24hHigh, last24hLow;
  float ytdHigh, ytdLow, ytdRain;
  int ytdRainyDays;
  struct { String time; float temp; int code; float windSpeed, precip; } hourly[6];
  struct { String dayName, date; float high, low; int code; float precip; } daily[7];
} weather;

unsigned long lastWeatherUpdate = 0, lastTimeUpdate = 0;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  lcd.pushImageDMA(area->x1, area->y1, w, h, (lgfx::rgb565_t*)&color_p->full);
  lv_disp_flush_ready(disp);
}

// Get weather condition text (kept for fallback/debugging)
String getWeatherCondition(int code) {
  if (code == 0) return "Clear";
  if (code == 1) return "Mostly Clear";
  if (code == 2) return "Partly Cloudy";
  if (code == 3) return "Overcast";
  if (code <= 48) return "Fog";
  if (code <= 57) return "Drizzle";
  if (code <= 67) return "Rain";
  if (code <= 77) return "Snow";
  if (code <= 86) return "Showers";
  if (code <= 99) return "Thunderstorm";
  return "Unknown";
}

// Get weather symbol from code
String getWeatherSymbol(int code) {
  // Clear Sky (0)
  if (code == 0) return "\u263C";        // ☼ White Sun with Rays
  
  // Mainly Clear (1)
  if (code == 1) return "\u263C";        // ☼ White Sun with Rays
  
  // Partly Cloudy (2)
  if (code == 2) return "\u26C5";        // ⛅ Sun Behind Cloud
  
  // Overcast (3)
  if (code == 3) return "\u2601";        // ☁ Cloud
  
  // Fog (45-48)
  if (code >= 45 && code <= 48) {
    return "\u2601";                     // ☁ Cloud (fog uses cloud)
  }
  
  // Drizzle (51-55, 56-57)
  if ((code >= 51 && code <= 55) || (code >= 56 && code <= 57)) {
    return "\u2602";                     // ☂ Umbrella
  }
  
  // Rain (61-65, 66-67, 80-82)
  if ((code >= 61 && code <= 65) || (code >= 66 && code <= 67) || 
      (code >= 80 && code <= 82)) {
    return "\u26C6";                     // ⛆ Rain
  }
  
  // Snow (71-75, 77, 85-86)
  if ((code >= 71 && code <= 75) || code == 77 || 
      (code >= 85 && code <= 86)) {
    return "\u2603";                     // ☃ Snowman
  }
  
  // Thunderstorm (95, 96, 99)
  if (code == 95 || code == 96 || code == 99) {
    return "\u26C8";                     // ⛈ Thunder Cloud and Rain
  }
  
  // Default/Unknown
  return "?";
}

// Get symbol color based on weather code
uint32_t getWeatherSymbolColor(int code) {
  if (code == 0 || code == 1) return COLOR_SUN;           // Clear/Mostly Clear
  if (code == 2) return COLOR_SUN;                         // Partly Cloudy
  if (code == 3 || (code >= 45 && code <= 48)) return COLOR_CLOUD;  // Overcast/Fog
  if ((code >= 51 && code <= 67) || (code >= 80 && code <= 82)) return COLOR_RAIN;  // Drizzle/Rain
  if ((code >= 71 && code <= 77) || (code >= 85 && code <= 86)) return COLOR_SNOW;  // Snow
  if (code >= 95) return COLOR_THUNDER;                   // Thunderstorm
  return COLOR_TEXT_PRIMARY;                               // Default
}

String getTimezone() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "";
  return (timeinfo.tm_isdst > 0) ? "PDT" : "PST";
}

void createUI() {
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(COLOR_BG_DARK), LV_PART_MAIN);
  
  locationLabel = lv_label_create(lv_scr_act());
  lv_label_set_text(locationLabel, locationName);
  lv_obj_set_style_text_font(locationLabel, &lv_font_montserrat_32, 0);
  lv_obj_set_style_text_color(locationLabel, lv_color_hex(COLOR_TEXT_PRIMARY), 0);
  lv_obj_set_pos(locationLabel, 10, 10);
  
  dateTimeLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(dateTimeLabel, &lv_font_montserrat_16, 0);
  lv_obj_set_style_text_color(dateTimeLabel, lv_color_hex(COLOR_TEXT_SECONDARY), 0);
  lv_obj_set_pos(dateTimeLabel, 10, 50);
  
  sunriseLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(sunriseLabel, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(sunriseLabel, lv_color_hex(COLOR_TEXT_SECONDARY), 0);
  lv_obj_set_pos(sunriseLabel, 10, 75);
  
  sunsetLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(sunsetLabel, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(sunsetLabel, lv_color_hex(COLOR_TEXT_SECONDARY), 0);
  lv_obj_set_pos(sunsetLabel, 140, 75);
  
  // Current condition uses 120pt weather symbol (Y=5)
  currentCondLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(currentCondLabel, &lv_font_weather_symbols_120, 0);
  lv_obj_set_style_text_color(currentCondLabel, lv_color_hex(COLOR_ACCENT), 0);
  lv_obj_align(currentCondLabel, LV_ALIGN_TOP_MID, 0, 5);
  
  // Text label - STAYS AT Y=112 - DO NOT MOVE!
  currentCondTextLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(currentCondTextLabel, &lv_font_montserrat_20, 0);
  lv_obj_set_style_text_color(currentCondTextLabel, lv_color_hex(COLOR_TEXT_PRIMARY), 0);
  lv_obj_align(currentCondTextLabel, LV_ALIGN_TOP_MID, 0, 112);
  
  currentTempLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(currentTempLabel, &lv_font_montserrat_48, 0);
  lv_obj_set_style_text_color(currentTempLabel, lv_color_hex(COLOR_ACCENT), 0);
  lv_obj_align(currentTempLabel, LV_ALIGN_TOP_RIGHT, -10, 10);
  
  humidityLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(humidityLabel, &lv_font_montserrat_12, 0);
  lv_obj_set_style_text_color(humidityLabel, lv_color_hex(COLOR_TEXT_SECONDARY), 0);
  lv_obj_align(humidityLabel, LV_ALIGN_TOP_RIGHT, -10, 70);
  
  pressureLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(pressureLabel, &lv_font_montserrat_12, 0);
  lv_obj_set_style_text_color(pressureLabel, lv_color_hex(COLOR_TEXT_SECONDARY), 0);
  lv_obj_align(pressureLabel, LV_ALIGN_TOP_RIGHT, -10, 87);
  
  feelsLikeLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(feelsLikeLabel, &lv_font_montserrat_12, 0);
  lv_obj_set_style_text_color(feelsLikeLabel, lv_color_hex(COLOR_TEXT_SECONDARY), 0);
  lv_obj_align(feelsLikeLabel, LV_ALIGN_TOP_RIGHT, -10, 104);
  
  lv_obj_t *hourlyTitle = lv_label_create(lv_scr_act());
  lv_label_set_text(hourlyTitle, "Next 6 Hours");
  lv_obj_set_style_text_font(hourlyTitle, &lv_font_montserrat_16, 0);
  lv_obj_set_style_text_color(hourlyTitle, lv_color_hex(COLOR_TEXT_PRIMARY), 0);
  lv_obj_set_pos(hourlyTitle, 20, 130);
  
  hourlyContainer = lv_obj_create(lv_scr_act());
  lv_obj_set_size(hourlyContainer, 760, 105);
  lv_obj_set_pos(hourlyContainer, 20, 155);
  lv_obj_set_style_bg_color(hourlyContainer, lv_color_hex(COLOR_BG_DARK), 0);
  lv_obj_set_style_border_width(hourlyContainer, 0, 0);
  lv_obj_set_style_pad_all(hourlyContainer, 3, 0);
  lv_obj_set_style_pad_row(hourlyContainer, 0, 0);
  lv_obj_set_style_pad_column(hourlyContainer, 0, 0);
  lv_obj_clear_flag(hourlyContainer, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scrollbar_mode(hourlyContainer, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_flex_flow(hourlyContainer, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(hourlyContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  
  // Hourly cards with 36pt weather symbol font
  for (int i = 0; i < 6; i++) {
    lv_obj_t *card = lv_obj_create(hourlyContainer);
    lv_obj_set_size(card, 120, 100);
    lv_obj_set_style_bg_color(card, lv_color_hex(COLOR_BG_LIGHT), 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_set_style_border_side(card, LV_BORDER_SIDE_NONE, 0);
    lv_obj_set_style_radius(card, 8, 0);
    lv_obj_set_style_pad_top(card, 5, 0);
    lv_obj_set_style_pad_bottom(card, 0, 0);
    lv_obj_set_style_pad_left(card, 5, 0);
    lv_obj_set_style_pad_right(card, 5, 0);
    lv_obj_set_style_shadow_width(card, 0, 0);
    lv_obj_set_style_shadow_opa(card, LV_OPA_TRANSP, 0);
    lv_obj_set_style_outline_width(card, 0, 0);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(card, LV_SCROLLBAR_MODE_OFF);
    
    lv_obj_t *timeL = lv_label_create(card);
    lv_obj_set_style_text_font(timeL, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(timeL, lv_color_hex(COLOR_TEXT_PRIMARY), 0);
    lv_obj_align(timeL, LV_ALIGN_TOP_MID, 0, 0);
    
    lv_obj_t *tempL = lv_label_create(card);
    lv_obj_set_style_text_font(tempL, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(tempL, lv_color_hex(COLOR_ACCENT), 0);
    lv_obj_align(tempL, LV_ALIGN_TOP_MID, 0, 18);
    
    // Weather symbol instead of text
    lv_obj_t *condL = lv_label_create(card);
    lv_obj_set_style_text_font(condL, &lv_font_weather_symbols_36, 0);
    lv_obj_set_style_text_color(condL, lv_color_hex(COLOR_TEXT_SECONDARY), 0);
    lv_obj_align(condL, LV_ALIGN_TOP_MID, 0, 30);
    
    lv_obj_t *detailsL = lv_label_create(card);
    lv_obj_set_style_text_font(detailsL, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(detailsL, lv_color_hex(COLOR_TEXT_DIM), 0);
    lv_obj_align(detailsL, LV_ALIGN_BOTTOM_MID, 0, 0);
  }
  
  lv_obj_t *dailyTitle = lv_label_create(lv_scr_act());
  lv_label_set_text(dailyTitle, "7-Day Forecast");
  lv_obj_set_style_text_font(dailyTitle, &lv_font_montserrat_16, 0);
  lv_obj_set_style_text_color(dailyTitle, lv_color_hex(COLOR_TEXT_PRIMARY), 0);
  lv_obj_set_pos(dailyTitle, 20, 270);
  
  dailyContainer = lv_obj_create(lv_scr_act());
  lv_obj_set_size(dailyContainer, 760, 125);
  lv_obj_set_pos(dailyContainer, 20, 295);
  lv_obj_set_style_bg_color(dailyContainer, lv_color_hex(COLOR_BG_DARK), 0);
  lv_obj_set_style_border_width(dailyContainer, 0, 0);
  lv_obj_set_style_pad_all(dailyContainer, 3, 0);
  lv_obj_set_style_pad_row(dailyContainer, 0, 0);
  lv_obj_set_style_pad_column(dailyContainer, 0, 0);
  lv_obj_clear_flag(dailyContainer, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scrollbar_mode(dailyContainer, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_flex_flow(dailyContainer, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(dailyContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  
  // Daily cards with 36pt weather symbol font
  for (int i = 0; i < 7; i++) {
    lv_obj_t *card = lv_obj_create(dailyContainer);
    lv_obj_set_size(card, 105, 120);
    lv_obj_set_style_bg_color(card, lv_color_hex(COLOR_BG_LIGHT), 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_set_style_border_side(card, LV_BORDER_SIDE_NONE, 0);
    lv_obj_set_style_radius(card, 8, 0);
    lv_obj_set_style_pad_top(card, 5, 0);
    lv_obj_set_style_pad_bottom(card, 0, 0);
    lv_obj_set_style_pad_left(card, 5, 0);
    lv_obj_set_style_pad_right(card, 5, 0);
    lv_obj_set_style_shadow_width(card, 0, 0);
    lv_obj_set_style_shadow_opa(card, LV_OPA_TRANSP, 0);
    lv_obj_set_style_outline_width(card, 0, 0);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(card, LV_SCROLLBAR_MODE_OFF);
    
    lv_obj_t *dayL = lv_label_create(card);
    lv_obj_set_style_text_font(dayL, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(dayL, lv_color_hex(COLOR_TEXT_SECONDARY), 0);
    lv_obj_align(dayL, LV_ALIGN_TOP_MID, 0, 0);
    
    // Weather symbol instead of text
    lv_obj_t *condL = lv_label_create(card);
    lv_obj_set_style_text_font(condL, &lv_font_weather_symbols_36, 0);
    lv_obj_set_style_text_color(condL, lv_color_hex(COLOR_TEXT_PRIMARY), 0);
    lv_obj_align(condL, LV_ALIGN_TOP_MID, 0, 25);
    
    lv_obj_t *tempL = lv_label_create(card);
    lv_obj_set_style_text_font(tempL, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(tempL, lv_color_hex(COLOR_TEXT_SECONDARY), 0);
    lv_obj_align(tempL, LV_ALIGN_TOP_MID, 0, 80);
    
    lv_obj_t *precipL = lv_label_create(card);
    lv_obj_set_style_text_font(precipL, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(precipL, lv_color_hex(COLOR_TEXT_DIM), 0);
    lv_obj_align(precipL, LV_ALIGN_BOTTOM_MID, 0, 0);
  }
  
  statsLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(statsLabel, &lv_font_montserrat_12, 0);
  lv_obj_set_style_text_color(statsLabel, lv_color_hex(COLOR_TEXT_SECONDARY), 0);
  lv_obj_set_pos(statsLabel, 20, 430);
}

void updateTimeDisplay() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;
  char buffer[100];
  strftime(buffer, sizeof(buffer), "%a, %b %d, %l:%M:%S %p", &timeinfo);
  String timeStr = String(buffer);
  timeStr.trim();
  lv_label_set_text(dateTimeLabel, timeStr.c_str());
}

void updateWeather() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  Serial.println("Fetching weather...");
  
  // Get current date for YTD calculation
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  char startDate[20], endDate[20];
  sprintf(startDate, "%04d-01-01", timeinfo.tm_year + 1900);
  sprintf(endDate, "%04d-%02d-%02d", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
  
  // Fetch current and forecast data
  HTTPClient http;
  String url = "https://api.open-meteo.com/v1/forecast?latitude=" + String(latitude, 4) +
               "&longitude=" + String(longitude, 4) +
               "&current=temperature_2m,relative_humidity_2m,apparent_temperature,pressure_msl,weather_code" +
               "&hourly=temperature_2m,weather_code,wind_speed_10m,precipitation" +
               "&daily=weather_code,temperature_2m_max,temperature_2m_min,sunrise,sunset,precipitation_sum" +
               "&temperature_unit=fahrenheit&wind_speed_unit=mph&precipitation_unit=inch&timezone=auto&forecast_days=8";
  
  http.begin(url);
  if (http.GET() != HTTP_CODE_OK) {
    http.end();
    return;
  }
  
  DynamicJsonDocument doc(16384);
  deserializeJson(doc, http.getString());
  http.end();
  
  weather.currentTemp = doc["current"]["temperature_2m"];
  weather.feelsLike = doc["current"]["apparent_temperature"];
  weather.humidity = doc["current"]["relative_humidity_2m"];
  weather.pressure = doc["current"]["pressure_msl"];
  weather.currentCode = doc["current"]["weather_code"];  // Store code instead of string
  
  String sunrise = doc["daily"]["sunrise"][0].as<String>();
  String sunset = doc["daily"]["sunset"][0].as<String>();
  weather.sunriseTime = sunrise.substring(11, 16);
  weather.sunsetTime = sunset.substring(11, 16);
  
  int currentHour = timeinfo.tm_hour;
  
  // Store weather codes for hourly forecast
  for (int i = 0; i < 6; i++) {
    int offset = i * 2;
    weather.hourly[i].temp = doc["hourly"]["temperature_2m"][currentHour + offset];
    weather.hourly[i].code = doc["hourly"]["weather_code"][currentHour + offset];  // Store code
    weather.hourly[i].windSpeed = doc["hourly"]["wind_speed_10m"][currentHour + offset];
    weather.hourly[i].precip = doc["hourly"]["precipitation"][currentHour + offset];
    
    int displayHour = (currentHour + offset) % 24;
    if (displayHour == 0) weather.hourly[i].time = "12:00 AM";
    else if (displayHour < 12) weather.hourly[i].time = String(displayHour) + ":00 AM";
    else if (displayHour == 12) weather.hourly[i].time = "12:00 PM";
    else weather.hourly[i].time = String(displayHour - 12) + ":00 PM";
  }
  
  const char* dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  const char* monthNames[] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  
  // Store weather codes for daily forecast
  for (int i = 0; i < 7; i++) {
    weather.daily[i].high = doc["daily"]["temperature_2m_max"][i];
    weather.daily[i].low = doc["daily"]["temperature_2m_min"][i];
    weather.daily[i].code = doc["daily"]["weather_code"][i];  // Store code
    weather.daily[i].precip = doc["daily"]["precipitation_sum"][i];
    
    time_t now = time(nullptr) + (i * 86400);
    struct tm* dayTime = localtime(&now);
    weather.daily[i].dayName = String(dayNames[dayTime->tm_wday]);
    weather.daily[i].date = String(monthNames[dayTime->tm_mon + 1]) + " " + String(dayTime->tm_mday);
  }
  
  // Calculate last 24 hours stats
  weather.last24hRain = 0;
  weather.last24hHigh = -999;
  weather.last24hLow = 999;
  for (int i = 0; i < 24; i++) {
    float temp = doc["hourly"]["temperature_2m"][i];
    if (temp > weather.last24hHigh) weather.last24hHigh = temp;
    if (temp < weather.last24hLow) weather.last24hLow = temp;
    weather.last24hRain += doc["hourly"]["precipitation"][i].as<float>();
  }
  
  // Fetch YTD historical data
  String ytdUrl = "https://archive-api.open-meteo.com/v1/archive?latitude=" + String(latitude, 4) +
                  "&longitude=" + String(longitude, 4) +
                  "&start_date=" + String(startDate) +
                  "&end_date=" + String(endDate) +
                  "&daily=temperature_2m_max,temperature_2m_min,precipitation_sum" +
                  "&temperature_unit=fahrenheit&precipitation_unit=inch&timezone=auto";
  
  http.begin(ytdUrl);
  if (http.GET() == HTTP_CODE_OK) {
    DynamicJsonDocument ytdDoc(32768);
    deserializeJson(ytdDoc, http.getString());
    
    weather.ytdHigh = -999;
    weather.ytdLow = 999;
    weather.ytdRain = 0;
    weather.ytdRainyDays = 0;
    
    JsonArray maxTemps = ytdDoc["daily"]["temperature_2m_max"];
    JsonArray minTemps = ytdDoc["daily"]["temperature_2m_min"];
    JsonArray precips = ytdDoc["daily"]["precipitation_sum"];
    
    for (int i = 0; i < maxTemps.size(); i++) {
      float tmax = maxTemps[i];
      float tmin = minTemps[i];
      float precip = precips[i];
      
      if (!isnan(tmax) && tmax > weather.ytdHigh) weather.ytdHigh = tmax;
      if (!isnan(tmin) && tmin < weather.ytdLow) weather.ytdLow = tmin;
      if (!isnan(precip)) {
        weather.ytdRain += precip;
        if (precip > 0.01) weather.ytdRainyDays++;
      }
    }
  } else {
    // Fallback values if YTD fetch fails
    weather.ytdHigh = -999;
    weather.ytdLow = 999;
    weather.ytdRain = 0;
    weather.ytdRainyDays = 0;
  }
  http.end();
  
  lastWeatherUpdate = millis();
  updateWeatherDisplay();
  Serial.println("Weather updated");
}

void updateWeatherDisplay() {
  lv_label_set_text(currentTempLabel, (String((int)weather.currentTemp) + "°F").c_str());
  
  // Display 64pt weather symbol
  lv_label_set_text(currentCondLabel, getWeatherSymbol(weather.currentCode).c_str());
  lv_obj_set_style_text_color(currentCondLabel, lv_color_hex(getWeatherSymbolColor(weather.currentCode)), 0);
  
  // Display weather condition text below the symbol
  lv_label_set_text(currentCondTextLabel, getWeatherCondition(weather.currentCode).c_str());
  
  lv_label_set_text(humidityLabel, ("Humidity: " + String((int)weather.humidity) + "%").c_str());
  lv_label_set_text(pressureLabel, ("Pressure " + String((int)(weather.pressure * 0.75)) + " mmHg").c_str());
  lv_label_set_text(feelsLikeLabel, ("Feels like: " + String((int)weather.feelsLike) + "°F").c_str());
  
  int sunriseHour = weather.sunriseTime.substring(0, 2).toInt();
  int sunriseMin = weather.sunriseTime.substring(3, 5).toInt();
  String sunriseStr = "Sunrise: " + String(sunriseHour > 12 ? sunriseHour - 12 : sunriseHour) + ":" +
                     (sunriseMin < 10 ? "0" : "") + String(sunriseMin) + (sunriseHour >= 12 ? "PM" : "AM");
  lv_label_set_text(sunriseLabel, sunriseStr.c_str());
  
  int sunsetHour = weather.sunsetTime.substring(0, 2).toInt();
  int sunsetMin = weather.sunsetTime.substring(3, 5).toInt();
  String sunsetStr = "Sunset: " + String(sunsetHour > 12 ? sunsetHour - 12 : sunsetHour) + ":" +
                    (sunsetMin < 10 ? "0" : "") + String(sunsetMin) + (sunsetHour >= 12 ? "PM" : "AM");
  lv_label_set_text(sunsetLabel, sunsetStr.c_str());
  
  // Display weather symbols in hourly cards
  for (int i = 0; i < 6; i++) {
    lv_obj_t *card = lv_obj_get_child(hourlyContainer, i);
    lv_label_set_text(lv_obj_get_child(card, 0), weather.hourly[i].time.c_str());
    lv_label_set_text(lv_obj_get_child(card, 1), (String((int)weather.hourly[i].temp) + "°F").c_str());
    
    // Set weather symbol and color
    lv_obj_t *condLabel = lv_obj_get_child(card, 2);
    lv_label_set_text(condLabel, getWeatherSymbol(weather.hourly[i].code).c_str());
    lv_obj_set_style_text_color(condLabel, lv_color_hex(getWeatherSymbolColor(weather.hourly[i].code)), 0);
    
    String details = "Wind: " + String((int)weather.hourly[i].windSpeed) + " mph\nPrecip: " + String(weather.hourly[i].precip, 2) + " in";
    lv_label_set_text(lv_obj_get_child(card, 3), details.c_str());
  }
  
  // Display weather symbols in daily cards
  for (int i = 0; i < 7; i++) {
    lv_obj_t *card = lv_obj_get_child(dailyContainer, i);
    String dayStr = (i == 0 ? "Today" : weather.daily[i].dayName) + ", " + weather.daily[i].date;
    lv_label_set_text(lv_obj_get_child(card, 0), dayStr.c_str());
    
    // Set weather symbol and color
    lv_obj_t *condLabel = lv_obj_get_child(card, 1);
    lv_label_set_text(condLabel, getWeatherSymbol(weather.daily[i].code).c_str());
    lv_obj_set_style_text_color(condLabel, lv_color_hex(getWeatherSymbolColor(weather.daily[i].code)), 0);
    
    String temps = "H " + String((int)weather.daily[i].high) + "° - L " + String((int)weather.daily[i].low) + "°F";
    lv_label_set_text(lv_obj_get_child(card, 2), temps.c_str());
    lv_label_set_text(lv_obj_get_child(card, 3), ("Precip: " + String(weather.daily[i].precip, 2) + " in").c_str());
  }
  
  String stats = "Last 24Hr: Rain: " + String(weather.last24hRain, 2) + "\", H: " + String((int)weather.last24hHigh) +
                 "°, L: " + String((int)weather.last24hLow) + "°.    YTD: Max High: " + String((int)weather.ytdHigh) +
                 "°, Min Low " + String((int)weather.ytdLow) + "°, Rain: " + String(weather.ytdRain, 2) +
                 "\", Rain Days: " + String(weather.ytdRainyDays) + "  PST/PDT? " + getTimezone() +"!" ;
  lv_label_set_text(statsLabel, stats.c_str());
}

void setup() {
  Serial.begin(115200);
  delay(500);
  
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  
  lcd.begin();
  lcd.fillScreen(TFT_BLACK);
  
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, SCREEN_WIDTH * SCREEN_HEIGHT / 15);
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
  
  pinMode(TFT_BL, OUTPUT);
  ledcSetup(1, 300, 8);
  ledcAttachPin(TFT_BL, 1);
  ledcWrite(1, 166);
  
  createUI();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && millis() < 15000) delay(500);
  
  if (WiFi.status() == WL_CONNECTED) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(2000);
    updateWeather();
  }
  
  Serial.println("Ready - Weather Symbols Loaded!");
}

void loop() {
  lv_timer_handler();
  delay(5);
  
  if (millis() - lastTimeUpdate >= 1000) {
    updateTimeDisplay();
    lastTimeUpdate = millis();
  }
  
  if (millis() - lastWeatherUpdate >= weatherUpdateInterval) {
    if (WiFi.status() == WL_CONNECTED) updateWeather();
  }
}
