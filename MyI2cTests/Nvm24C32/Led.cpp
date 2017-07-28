// Led.cpp

#include "Arduino.h"

int ledState = LOW;

void blinkLed(int ledPin) {
  if (ledState == LOW) {
    ledState = HIGH;
  }
  else {
    ledState = LOW;
  }

  digitalWrite(ledPin, ledState);
}

