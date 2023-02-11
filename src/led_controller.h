#pragma once
#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>
#include <iostream>

class LEDController
{

private:
    void colorWipe(uint16_t c, uint8_t wait);

public:
    static void initLEDController();
    static void setLEDColor(uint8_t r, uint8_t g, uint8_t b);
    static void setBrightness(uint8_t level);
};

#endif