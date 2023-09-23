#include "SDM501.h"

SDM501::SDM501(int pin, userfunc interruptDispatch) {
  _pin = pin;
  _interruptDispatch = interruptDispatch;
  _bootMillis = millis();
  _lastReadPmMillis = _bootMillis;
  _lastState = LOW;
  _lastMicros = micros();
  _lowPulseTotalMicros = 0;
  pinMode(_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_pin), _interruptDispatch, CHANGE);
}

bool SDM501::isReady() {
  unsigned long currentMillis = millis();
  return currentMillis - _bootMillis > SDM501_WARMUP_TIME;
}

int SDM501::getReadyCountdown() {
  unsigned long currentMillis = millis();
  unsigned long elapsedMillis = currentMillis - _bootMillis;
  return (int)((SDM501_WARMUP_TIME - elapsedMillis) / 1000);
}

void SDM501::handleInterrupt() {
  byte state = digitalRead(_pin);

  if (_lastState != state) {
    if (state == HIGH) { // Rising edge
      _lowPulseMicros = micros() - _lastMicros;
    } else { // Falling edge
      _lastMicros = micros();
      _lowPulseTotalMicros += _lowPulseMicros;
    }
    _lastState = state;
  }
}

float SDM501::readPM() {
  if (!isReady()) {
    return 0.0;
  }

  unsigned long currentMillis = millis();
  unsigned long elapsedMillis = currentMillis - _lastReadPmMillis;
  _lastReadPmMillis = currentMillis;

  float ratio = _lowPulseTotalMicros / (elapsedMillis * 10.0);
  float concentration = ratio * ratio * ratio * 1.1 - ratio * ratio * 3.8 + ratio * 520 + 0.62;
  if (elapsedMillis < 3600000UL) {
    concentration *= (elapsedMillis / 3600000.0);
  }

  _lowPulseTotalMicros = 0;
  return concentration;
}
