#ifndef DSM501_H
#define DSM501_H

#include <Arduino.h>

typedef void (*userfunc)(void);

class DSM501
{
public:
    DSM501(int pin, userfunc interruptDispatch);
    bool isReady();
    int getReadyCountdown();
    void handleInterrupt();
    float readPM();

private:
    int _pin;
    userfunc _interruptDispatch;
    unsigned long _bootMillis;
    unsigned long _lastReadPmMillis;
    byte _lastState = LOW;
    unsigned long _lastMicros = 0;
    unsigned long _lowPulseTotalMicros = 0;
    const unsigned long DSM501_WARMUP_TIME = 30000UL; // Adjust warm-up time as needed
};

#endif // DSM501_H
