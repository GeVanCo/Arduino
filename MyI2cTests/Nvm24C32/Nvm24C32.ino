#include <Wire.h>
#include <stdio.h>

#include "NvmComm.h"
#include "PrintSerial.h"
#include "Led.h"

#define EEPROM 0x50
#define SOUND 0x00
#define PICTURE 0x01
#define TINT 0x02
#define HUE 0x03
#define CONTRAST 0x04
#define BRIGHTNESS 0x05

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

void setup() {
	pinMode(ledPin, OUTPUT);
	Wire.begin();
	Serial.begin(115200);
	Serial.println(" Hello there, goodday! ");
	Serial.println(" Starting I2c communication ");
	randomSeed(millis());
}

void loop() {
	delay(1000);

  blinkLed(ledPin);

  randomNumber = random(0, 256);
  randomFeature = random(0, 6);

//  nvmWrite(EEPROM, 0x00, locationArray[randomFeature][1], randomNumber);
//  printWriteMessage(randomNumber, locationArray[randomFeature][1]);

  nvmWrite(EEPROM, 0x00, 0, 5);
  printWriteMessage(5, 0);
  nvmWrite(EEPROM, 0x00, 1, 4);
  printWriteMessage(4, 1);
  nvmWrite(EEPROM, 0x00, 2, 3);
  printWriteMessage(3, 2);
  nvmWrite(EEPROM, 0x00, 3, 2);
  printWriteMessage(2, 3);
  nvmWrite(EEPROM, 0x00, 4, 1);
  printWriteMessage(1, 4);
  nvmWrite(EEPROM, 0x00, 5, randomNumber);
  printWriteMessage(randomNumber, 5);

//  epromValue = nvmRead(EEPROM, 0x00, locationArray[randomFeature][1], 1);
//  printReadMessage(epromValue, locationArray[randomFeature][1]);
  epromValue = nvmRead(EEPROM, 0, 0, 1);
  printReadMessage(epromValue, 0);
  epromValue = nvmRead(EEPROM, 0, 1, 1);
  printReadMessage(epromValue, 1);
  epromValue = nvmRead(EEPROM, 0, 2, 1);
  printReadMessage(epromValue, 2);
  epromValue = nvmRead(EEPROM, 0, 3, 1);
  printReadMessage(epromValue, 3);
  epromValue = nvmRead(EEPROM, 0, 4, 1);
  printReadMessage(epromValue, 4);
//  epromValue = nvmRead(EEPROM, 0, 5, 1);
//  printReadMessage(epromValue, locationArray[5][1]);

//  delay(1000);

  // This command will read the next location following the last location read or written.
  // In this example, the last location read is location 0x0004 (see above, last nvmRead()
  // command, so the "current address" becomes location 0x0005.
  // Hence, nvmReadCurrentAddress() reads content of location 0x0005, NOT 0x0004, which is
  // a bit misleading, if you ask me...
  // The content of location 0x0005 has beenwritten with the value 0x55 (see command on
  // line 60 above), so we should read out 0x55 with the below command.  Or whatever is
  // written into location 0x0005 before...
  epromValue = nvmReadCurrentAddress(EEPROM);
  printReadMessage(epromValue);
}

