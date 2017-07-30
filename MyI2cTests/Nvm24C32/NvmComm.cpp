// NvmComm.cpp

#include <Wire.h>
#include "Arduino.h"
#include "PrintSerial.h"

void nvmWrite(int device, int addressHigh, int addressLow, int rndNumber) {
  uint8_t arr[2];
  arr[0] = addressHigh;
  arr[1] = addressLow;

  Wire.beginTransmission(device);
  // Passing an array with the page address where to write to...
  Wire.write(arr, 2);
  Wire.write(rndNumber);
  Wire.endTransmission();
  delay(5);
}

// Reading one byte from the EEPROM
// Returns the value read from the EEPROM
int nvmRead(int device, int addressHigh, int addressLow) {
  // This is an I2c read action with repeated start (last parm is false iso true)
  // and it also passes the internal register address of the 24C32 NVM device
  Wire.requestFrom(device, 1, (addressHigh << 8) | addressLow, 2, false);
  
  return Wire.read();
}

// Reading bulk data from the EEPROM
// Returns the number of bytes read from the EEPROM.  Should be the same as nrOfBytes.
// The bulk data is returned in the result parameter.
int nvmRead(int device, int addressHigh, int addressLow, int nrOfBytes, int *result) {
  int bytesRead = Wire.requestFrom(device, nrOfBytes, (addressHigh << 8) | addressLow, 2, false);

  for (int i = 0; i < bytesRead; i++) {
    *result++ = Wire.read();
  }

  return bytesRead;
}

int nvmReadCurrentAddress(int device) {
  Wire.requestFrom(device, 1);

  return Wire.read();
}

