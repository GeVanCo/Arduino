// PrintSerial.cpp

#include "Arduino.h"

char tbs[6];

void printWriteMessage(int rndNumber, int location) {
  Serial.print(" Written value       : ");
  sprintf(tbs, "0x%02X", rndNumber);
  Serial.print(tbs);
  Serial.print(" @ location ");
  sprintf(tbs, "0x%02X", location);
  Serial.print(tbs);
  Serial.println(" of the eeprom 24C32...");
}

void printReadMessage(int retVal, int location) {
  Serial.print(" Read value from ");
  sprintf(tbs, "0x%02X", location);
  Serial.print(tbs);
  Serial.print(": ");
  sprintf(tbs, "0x%02X", retVal);
  Serial.println(tbs);
}

