#pragma once
#ifndef VUCLOCK_H
#define VUCLOCK_H

class VUClock
{
private:
    struct _timeComponents {
        long hour;
        long mins;
    };

public:
    static void StartClock();
    static void UpdateTime();
    static String GetTime();
    static _timeComponents TimeComponents;
    static _timeComponents GetTimeComponents();
    static void SetUtcOffSet(int tzOffsetInHours);
};

#endif