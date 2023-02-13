
#include "led_controller.h"
#include <Adafruit_NeoPixel.h>

#define PIN_NEOPIXELS 15
#define PIXELCOUNT 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELCOUNT, PIN_NEOPIXELS, NEO_GRB + NEO_KHZ800);

void LEDController::setLEDColor(uint8_t r, uint8_t g, uint8_t b)
{
    strip.clear();

    for (int i = 0; i < PIXELCOUNT; i++)
    {
        strip.setPixelColor(i, strip.Color(r, g, b));
        strip.show();
    }
}

void LEDController::setBrightness(uint8_t level)
{
    strip.setBrightness(level);
    strip.show();
}

// Fill the dots one after the other with a color
void LEDController::colorWipe(uint16_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void LEDController::initLEDController()
{
    pinMode(PIN_NEOPIXELS, OUTPUT);

    strip.begin();
    strip.setBrightness(255);
    strip.show(); // Initialize
    delay(100);

    LEDController led;

    led.colorWipe(strip.Color(255, 0, 0), 50);
    led.colorWipe(strip.Color(0, 255, 0), 50);
    led.colorWipe(strip.Color(0, 0, 255), 50);
    led.colorWipe(strip.Color(255, 0, 0), 50);
    led.colorWipe(strip.Color(0, 255, 0), 50);
    led.colorWipe(strip.Color(0, 0, 255), 50);

    led.setLEDColor(0, 0, 255);
}
