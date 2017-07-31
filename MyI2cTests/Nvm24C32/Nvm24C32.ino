/*
 * Copyright 2017 Geert Vancompernolle
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHE
 * R LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

/* 
 *  This project shows how to write to and read from the EEPROM device 24C32.
 *  This is an I2c device which can hold a total of 32 Kbit of information (4 kB)
 *  divided over different pages.
 *  
 *  The below code shows basic access to the device:
 *  - writing to a random place
 *  - reading from a random place
 *  
 *  More complex situations are possible:
 *  - write/read pages
 *  - write/read blocks
 *  
 *  See the datasheet of the 24C32 for more information about the latter situations
 */
#include <Wire.h>
#include <stdio.h>

#include "NvmComm.h"
#include "PrintSerial.h"
#include "Led.h"

#define NVM24C32 0x50
#define PCA9548ADDR 0x70

#define SOUND 0x00
#define PICTURE 0x01
#define TINT 0x02
#define HUE 0x03
#define CONTRAST 0x04
#define BRIGHTNESS 0x05

#define I2C_CHANNEL_SD0_SC0 0x01
#define I2C_CHANNEL_SD1_SC1 0x02
#define I2C_CHANNEL_SD2_SC2 0x04
#define I2C_CHANNEL_SD3_SC3 0x08
#define I2C_CHANNEL_SD4_SC4 0x10
#define I2C_CHANNEL_SD5_SC5 0x20
#define I2C_CHANNEL_SD6_SC6 0x40
#define I2C_CHANNEL_SD7_SC7 0x80

byte locationArray[6][2] {
  {SOUND, 0x00},
  {PICTURE, 0x01},
  {TINT, 0x02},
  {HUE, 0x03},
  {CONTRAST, 0x04},
  {BRIGHTNESS, 0x05}
};

int ledPin = 12;
byte epromValue;
long randomNumber;
long randomFeature;
int resultArray[64];

void setup() {
  delay(2000);
	pinMode(ledPin, OUTPUT);
	Wire.begin();
	Serial.begin(115200);
	Serial.println(" Hello there, goodday! ");
	Serial.println(" Starting I2c communication using PCA9548 I2c switch");
 
  selectI2cChannels(I2C_CHANNEL_SD7_SC7);

	randomSeed(millis());
}

void loop() {
	delay(1000);
  
  blinkLed(ledPin);

  randomNumber = random(0, 256);
  randomFeature = random(0, 6);

  // Here, only the first 6 locations are used for testing: 0x0000 to 0x0005
  // In every location a random number between 0 and 255 is written.
  randomNumber = random(0, 256);
  nvmWrite(NVM24C32, 0x00, 0x00, randomNumber);
  printWriteMessage(0, randomNumber);
  
  randomNumber = random(0, 256);
  nvmWrite(NVM24C32, 0x00, 0x01, randomNumber);
  printWriteMessage(1, randomNumber);
  
  randomNumber = random(0, 256);
  nvmWrite(NVM24C32, 0x00, 0x02, randomNumber);
  printWriteMessage(2, randomNumber);
  
  randomNumber = random(0, 256);
  nvmWrite(NVM24C32, 0x00, 0x03, randomNumber);
  printWriteMessage(3, randomNumber);
  
  randomNumber = random(0, 256);
  nvmWrite(NVM24C32, 0x00, 0x04, randomNumber);
  printWriteMessage(4, randomNumber);
  
  randomNumber = random(0, 256);
  nvmWrite(NVM24C32, 0x00, 0x05, randomNumber);
  printWriteMessage(5, randomNumber);

  // Here, the first 4 locations are read.  This means, after the last read the
  // "current address" (see further) is set to the next location, being 0x0004
  epromValue = nvmRead(NVM24C32, 0x00, 0x00);
  printReadMessage(0, epromValue);
  
  epromValue = nvmRead(NVM24C32, 0x00, 0x01);
  printReadMessage(1, epromValue);
  
  epromValue = nvmRead(NVM24C32, 0x00, 0x02);
  printReadMessage(2, epromValue);
  
  epromValue = nvmRead(NVM24C32, 0x00, 0x03);
  printReadMessage(3, epromValue);

  // This command will read the next location following the last location read or written.
  // If the last location read is x then the "current address" becomes location x + 1.
  // E.g. when the last location read or written is 0x0004, then nvmReadCurrentAddress() 
  // reads content of location 0x0005, NOT 0x0004, which is a bit misleading, if you ask me...
  epromValue = nvmReadCurrentAddress(NVM24C32);
  printReadMessage(epromValue);

  printLn();

  // Here, a sequence of 5 bytes will be read
  epromValue = nvmRead(NVM24C32, 0x00, 0x00, 6, resultArray);
  printMessage(" Read eprom value: ", epromValue);
  
  for (int i = 0; i < 6; i++) {
    printReadMessage(i, resultArray[i]);
  }

  printLn();
  Serial.println("=============================");
  printLn();
}

void selectI2cChannels(int channels) 
{
  Wire.beginTransmission(PCA9548ADDR);
  Wire.write(channels);
  Wire.endTransmission();  
}

