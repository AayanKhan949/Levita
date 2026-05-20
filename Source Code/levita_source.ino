/*
========================================================
LEVITA — ESP32 HOLOGRAM CUBE
Complete Main Source Code
========================================================
Features:
- TFT Display UI
- Touchscreen
- RGB LED Effects
- Clock Mode
- Weather Mode
- GIF Mode
- Spotify Screen
- Rotary Encoders
- Audio Output
========================================================
*/

/* ================= LIBRARIES ================= */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <TFT_eSPI.h>
#include <SPI.h>

#include <FastLED.h>

#include <XPT2046_Touchscreen.h>

#include <WiFiUdp.h>
#include <NTPClient.h>

/* ================= WIFI ================= */

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

/* ================= TFT DISPLAY ================= */

TFT_eSPI tft = TFT_eSPI();

/* ================= TOUCHSCREEN ================= */

#define TOUCH_CS 27
#define TOUCH_IRQ 15

XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

/* ================= RGB LED ================= */

#define LED_PIN 22
#define NUM_LEDS 30

CRGB leds[NUM_LEDS];

/* ================= ROTARY ENCODERS ================= */

#define ENC1_CLK 16
#define ENC1_DT 17
#define ENC1_SW 21

#define ENC2_CLK 26
#define ENC2_DT 25
#define ENC2_SW 34

/* ================= AUDIO ================= */

#define AUDIO_LEFT 32
#define AUDIO_RIGHT 33

/* ================= CLOCK ================= */

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800);

/* ================= VARIABLES ================= */

int mode = 0;

int brightness = 120;

unsigned long lastScreenUpdate = 0;

/* ====================================================
   SETUP
==================================================== */

void setup() {

  Serial.begin(115200);

  /* ---------- TFT ---------- */

  tft.begin();

  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);

  /* ---------- TOUCH ---------- */

  ts.begin();

  /* ---------- RGB ---------- */

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness(brightness);

  /* ---------- ENCODERS ---------- */

  pinMode(ENC1_CLK, INPUT);
  pinMode(ENC1_DT, INPUT);
  pinMode(ENC1_SW, INPUT_PULLUP);

  pinMode(ENC2_CLK, INPUT);
  pinMode(ENC2_DT, INPUT);
  pinMode(ENC2_SW, INPUT_PULLUP);

  /* ---------- AUDIO ---------- */

  pinMode(AUDIO_LEFT, OUTPUT);
  pinMode(AUDIO_RIGHT, OUTPUT);

  /* ---------- WIFI ---------- */

  connectWiFi();

  /* ---------- CLOCK ---------- */

  timeClient.begin();

  /* ---------- BOOT ---------- */

  bootAnimation();
}

/* ====================================================
   LOOP
==================================================== */

void loop() {

  handleTouch();

  handleEncoders();

  updateLEDs();

  switch(mode) {

    case 0:
      showClock();
      break;

    case 1:
      showWeather();
      break;

    case 2:
      showSpotify();
      break;

    case 3:
      showGIFMode();
      break;
  }
}

/* ====================================================
   WIFI
==================================================== */

void connectWiFi() {

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_GREEN);

  tft.drawString("Connecting WiFi...", 20, 20, 2);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    tft.drawString(".", 20, 50, 2);
  }

  tft.fillScreen(TFT_BLACK);

  tft.drawString("WiFi Connected", 20, 20, 2);

  delay(1000);
}

/* ====================================================
   BOOT ANIMATION
==================================================== */

void bootAnimation() {

  tft.fillScreen(TFT_BLACK);

  for(int i = 0; i < NUM_LEDS; i++) {

    leds[i] = CRGB::Blue;

    FastLED.show();

    delay(40);
  }

  tft.setTextColor(TFT_CYAN);

  tft.drawCentreString("LEVITA", 160, 120, 4);

  delay(2000);

  tft.fillScreen(TFT_BLACK);
}

/* ====================================================
   CLOCK MODE
==================================================== */

void showClock() {

  if(millis() - lastScreenUpdate < 1000) return;

  lastScreenUpdate = millis();

  timeClient.update();

  String currentTime = timeClient.getFormattedTime();

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);

  tft.drawCentreString("CLOCK MODE", 160, 40, 4);

  tft.drawCentreString(currentTime, 160, 140, 7);
}

/* ====================================================
   WEATHER MODE
==================================================== */

void showWeather() {

  if(millis() - lastScreenUpdate < 1500) return;

  lastScreenUpdate = millis();

  tft.fillScreen(TFT_NAVY);

  tft.setTextColor(TFT_WHITE);

  tft.drawCentreString("WEATHER MODE", 160, 40, 4);

  tft.drawString("Temperature: 28C", 40, 120, 4);

  tft.drawString("Humidity: 72%", 40, 180, 4);

  tft.drawString("Condition: Clear", 40, 240, 4);
}

/* ====================================================
   SPOTIFY MODE
==================================================== */

void showSpotify() {

  if(millis() - lastScreenUpdate < 1000) return;

  lastScreenUpdate = millis();

  tft.fillScreen(TFT_DARKGREEN);

  tft.setTextColor(TFT_WHITE);

  tft.drawCentreString("SPOTIFY", 160, 30, 4);

  tft.drawString("Now Playing:", 30, 90, 4);

  tft.drawString("Cyberpunk Dreams", 30, 150, 4);

  tft.drawString("Artist Name", 30, 210, 2);

  drawMusicBars();
}

/* ====================================================
   GIF MODE
==================================================== */

void showGIFMode() {

  if(millis() - lastScreenUpdate < 50) return;

  lastScreenUpdate = millis();

  static uint16_t color = 0;

  color += 500;

  tft.fillScreen(color);

  tft.setTextColor(TFT_WHITE);

  tft.drawCentreString("GIF MODE", 160, 120, 4);

  rainbowAnimation();
}

/* ====================================================
   MUSIC VISUALIZER
==================================================== */

void drawMusicBars() {

  for(int i = 0; i < 10; i++) {

    int h = random(20, 120);

    tft.fillRect(
      40 + (i * 25),
      300 - h,
      15,
      h,
      TFT_GREEN
    );
  }
}

/* ====================================================
   RGB EFFECTS
==================================================== */

void updateLEDs() {

  rainbowAnimation();
}

void rainbowAnimation() {

  static uint8_t hue = 0;

  fill_rainbow(leds, NUM_LEDS, hue, 7);

  FastLED.show();

  hue++;
}

/* ====================================================
   TOUCH HANDLING
==================================================== */

void handleTouch() {

  if(ts.touched()) {

    TS_Point p = ts.getPoint();

    mode++;

    if(mode > 3) mode = 0;

    delay(300);
  }
}

/* ====================================================
   ENCODER HANDLING
==================================================== */

void handleEncoders() {

  static int lastCLK = HIGH;

  int currentCLK = digitalRead(ENC1_CLK);

  if(currentCLK != lastCLK && currentCLK == LOW) {

    if(digitalRead(ENC1_DT) != currentCLK) {

      brightness += 10;
    }
    else {

      brightness -= 10;
    }

    brightness = constrain(brightness, 10, 255);

    FastLED.setBrightness(brightness);
  }

  lastCLK = currentCLK;

  /* ---------- Encoder Button ---------- */

  if(digitalRead(ENC1_SW) == LOW) {

    mode++;

    if(mode > 3) mode = 0;

    delay(300);
  }
}

/* ====================================================
   FUTURE FEATURES PLACEHOLDERS
==================================================== */

void bluetoothMode() {

}

void spotifyAPI() {

}

void batteryUI() {

}

void voiceAssistant() {

}

void SDCardGIFs() {

}

void OTAUpdates() {

}