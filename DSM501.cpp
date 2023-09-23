#include "DSM501.h"

DSM501::DSM501(int pin, userfunc interruptDispatch)
{
    _pin = pin;
    _interruptDispatch = interruptDispatch;
    _bootMillis = millis();
    _lastReadPmMillis = _bootMillis;
    pinMode(_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_pin), _interruptDispatch, CHANGE);
}

bool DSM501::isReady()
{
    unsigned long currentMillis = millis();
    return currentMillis - _bootMillis > DSM501_WARMUP_TIME;
}

int DSM501::getReadyCountdown()
{
    unsigned long currentMillis = millis();
    return (int)((DSM501_WARMUP_TIME - currentMillis + _bootMillis) / 1000);
}

void DSM501::handleInterrupt()
{
    byte state = digitalRead(_pin);

    // On rising edge: report pulse length.
    if (_lastState == LOW && state == HIGH)
    {
        unsigned long lowPulseMicros = micros() - _lastMicros;
        _lowPulseTotalMicros += lowPulseMicros;
        _lastState = HIGH;
    }
    // On falling edge: record current time.
    if (_lastState == HIGH && state == LOW)
    {
        _lastMicros = micros();
        _lastState = LOW;
    }
}

float DSM501::readPM()
{
    if (!isReady())
    {
        return 0.0;
    }
    unsigned long currentMillis = millis();
    unsigned long interval = currentMillis - _lastReadPmMillis;
    float ratio = _lowPulseTotalMicros / (interval * 10.0);
    float concentration = ratio * ratio * ratio * 1.1 - ratio * ratio * 3.8 + ratio * 520 + 0.62;
    if (interval < 3600000UL)
    {
        concentration *= (interval / 3600000.0);
    }

    _lowPulseTotalMicros = 0;
    _lastReadPmMillis = currentMillis;

    return concentration;
}
