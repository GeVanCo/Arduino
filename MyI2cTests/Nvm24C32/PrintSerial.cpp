// PrintSerial.cpp

#include "Arduino.h"

char printBuffer[6];

void printWriteMessage(int rndNumber, int location) {
  Serial.print(" Written value       : ");
  sprintf(printBuffer, "0x%02X", rndNumber);
  Serial.print(printBuffer);
  Serial.print(" @ location ");
  sprintf(printBuffer, "0x%02X", location);
  Serial.print(printBuffer);
  Serial.println(" of the eeprom 24C32...");
}

void printReadMessage(int value, int location) {
  Serial.print(" Read value from ");
  sprintf(printBuffer, "0x%02X", location);
  Serial.print(printBuffer);
  Serial.print(": ");
  sprintf(printBuffer, "0x%02X", value);
  Serial.println(printBuffer);
}

void printReadMessage(int value) {
  Serial.print(" Read value: ");
  sprintf(printBuffer, "0x%02X", value);
  Serial.println(printBuffer);
}

