# LEVITA GUIDE

# This project includes:

Spotify display

GIF playback

Weather mode

Clock mode

RGB animations

Rotary encoders

Touchscreen

Audio output

Hologram visuals

# REQUIRED SOFTWARE

Install Arduino IDE

Official: https://www.arduino.cc/en/

# INSTALL ESP32 BOARD PACKAGE
Open:
Arduino IDE → File → Preferences

Add this URL :
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Then :
Tools → Board Manager

Install:
ESP32 by Espressif

# REQUIRED LIBRARIES:

Install these using:
Arduino IDE → Library Manager

Required Libraries :

TFT_eSPI

LVGL

FastLED

ArduinoJson

WiFi

HTTPClient

AnimatedGIF

XPT2046_Touchscreen

ESP32-audioI2S

NTPClient

Adafruit GFX Library

# TFT_eSPI CONFIGURATION :
Open : 
Documents/Arduino/libraries/TFT_eSPI/User_Setup.h

Replace contents with:

#define ILI9488_DRIVER

#define TFT_WIDTH  320

#define TFT_HEIGHT 480


#define TFT_MISO 19

#define TFT_MOSI 23

#define TFT_SCLK 18

#define TFT_CS    5

#define TFT_DC    2

#define TFT_RST   4


#define SPI_FREQUENCY  40000000

#define SPI_READ_FREQUENCY  20000000
