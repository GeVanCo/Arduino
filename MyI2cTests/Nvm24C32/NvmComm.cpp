// NvmComm.cpp

#include <Wire.h>
#include "Arduino.h"

void callNvmWrite(int device, int addressHigh, int addressLow, int rndNumber) {
  uint8_t arr[2];
  arr[0] = addressHigh;
  arr[1] = addressLow;
  Wire.beginTransmission(device);
//  Wire.write(addressHigh);
//  Wire.write(addressLow);

  // Passing an array with the page address where to write to...
  Wire.write(arr, 2);
  Wire.write(rndNumber);
  Wire.endTransmission();
}

int callNvmRead(int device, int addressHigh, int addressLow, int nrOfBytes) {
//  Wire.beginTransmission(device);
//  Wire.write(addressHigh);
//  Wire.write(addressLow);
//  Wire.endTransmission();

  // This is an I2c read action with repeated start (last parm is false iso true)
  // and it also passes the internal register address of the 24C32 NVM device
  Wire.requestFrom(device, nrOfBytes, (addressHigh << 8) | addressLow, 2, false);
  
  return Wire.read();
}

