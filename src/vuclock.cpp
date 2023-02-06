#include <Arduino.h>
#include <iostream>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "vuclock.h"

// For UTC -5.00 : -5 * 60 * 60 : -18000
long utcOffsetInSeconds = 0;
int twentyFourHourOffset = 12;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, utcOffsetInSeconds);

void VUClock::StartClock()
{
    timeClient.begin();
}

VUClock::_timeComponents VUClock::GetTimeComponents()
{
    long hours = timeClient.getHours() >= 13 ? timeClient.getHours() - twentyFourHourOffset : timeClient.getHours();
    long mins = timeClient.getMinutes();
    _timeComponents tc = {hours, mins};
    Serial.println("TC TIME: " + String(hours) + ":" + String(mins));

    return tc;
}

String VUClock::GetTime()
{
    long hours = timeClient.getHours();
    long mins = timeClient.getMinutes();

    String hoursString = hours < 10 ? "0" + String(hours) : String(hours);
    String minutesString = mins < 10 ? "0" + String(mins) : String(mins);
    String formattedTime = hoursString + ":" + minutesString;
    return formattedTime;
}

void VUClock::UpdateTime()
{
    int twentyFourHourOffset = 12;
    String meridiem = "AM";

    timeClient.update();

    if (timeClient.getHours() <= 12)
    {
        twentyFourHourOffset = 0;
    }
    else
    {
        meridiem = "PM";
    }

    Serial.print(timeClient.getHours() - twentyFourHourOffset);
    Serial.print(":");
    Serial.print(timeClient.getMinutes());
    Serial.print(" " + meridiem);
    Serial.println("");
}

void VUClock::SetUtcOffSet(int tzOffsetInHours)
{
    utcOffsetInSeconds = tzOffsetInHours * 60 * 60;
    timeClient.setTimeOffset(utcOffsetInSeconds);
    Serial.println("Offset in Seconds: " + String(utcOffsetInSeconds));
}
