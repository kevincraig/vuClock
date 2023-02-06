#include <Arduino.h>
#include <iostream>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "wifi.h"
#include "wifi_config.h"
#include "api_server.h"
#include "vuclock.h"
#include "LittleFS.h"
#include <Adafruit_NeoPixel.h>
#include "main.h"
const char *ssid = CLKSSID;
const char *password = CLKPSK;

ApiServer api;

#define PIN_METER_HOUR 5
#define PIN_METER_MINS 4
#define PIN_LED_SERVER 2
#define PIN_NEOPIXELS 15
#define PIXEL_COUNT

#define MINIMUM_PWM_VALUE_HOUR 18
#define MAXIMUM_PWM_VALUE_HOUR 240
#define MINIMUM_PWM_VALUE_MIN 1
#define MAXIMUM_PWM_VALUE_MIN 221
#define MINIMUM_PWM_VALUE_SEC 5
#define MAXIMUM_PWM_VALUE_SEC 235

int period = 60000;
unsigned long timeNow = 0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN_NEOPIXELS, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void runClock()
{
  timeNow = millis();
  VUClock::UpdateTime();
  long hour = VUClock::GetTimeComponents().hour;
  long mins = VUClock::GetTimeComponents().mins;
  analogWrite(PIN_METER_HOUR, map(hour, 1, 12, MINIMUM_PWM_VALUE_HOUR, MAXIMUM_PWM_VALUE_HOUR));
  analogWrite(PIN_METER_MINS, map(mins, 0, 60, MINIMUM_PWM_VALUE_MIN, MAXIMUM_PWM_VALUE_MIN));
  // delay(60000);
}

void Main::setLedColor(uint8_t r, uint8_t g, uint8_t b)
{
  strip.Color(r, g, b);
}

void initializeClock()
{
  colorWipe(strip.Color(255, 0, 0), 50);
  colorWipe(strip.Color(0, 255, 0), 50);
  colorWipe(strip.Color(0, 0, 255), 50);
  Main::setLedColor(255, 0, 0);
}

void setup()
{

  Serial.begin(115200);
  MDNS.begin("esp8266");

  if (!LittleFS.begin())
  {
    Serial.println("An error has occurred while mounting LittleFS");
    return;
  }

  File file = LittleFS.open("/index.html", "r");
  if (!file)
  {
    Serial.println("Failed to open file");
    return;
  }

  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(PIN_NEOPIXELS, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }

  Serial.println("Connected to: " + String(ssid));
  Serial.println("WiFi Connected");
  Serial.println("IP Address (DHCP): " + WiFi.localIP().toString());
  VUClock::SetUtcOffSet(-5);
  ApiServer::StartServer();
  MDNS.addService("http", "tcp", 80);
  digitalWrite(PIN_LED_SERVER, HIGH);

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
  initializeClock();

  timeNow = millis();
  runClock();
}

void loop()
{

  MDNS.update();

  // update clock ever 60 sec
  if (millis() > timeNow + period)
  {
    runClock();
  }
}
