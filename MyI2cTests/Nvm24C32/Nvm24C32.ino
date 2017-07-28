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
	Serial.begin(9600);
	Serial.println(" Hello there, goodday! ");
	Serial.println(" Starting I2c communication ");
	randomSeed(millis());
}

void loop() {
	delay(1000);

  blinkLed(ledPin);

  randomNumber = random(0, 256);
  randomFeature = random(0, 6);

  callNvmWrite(EEPROM, 0x00, locationArray[randomFeature][1], randomNumber);
  printWriteMessage(randomNumber, locationArray[randomFeature][1]);

  epromValue = callNvmRead(EEPROM, 0x00, locationArray[randomFeature][1], 1);
  printReadMessage(epromValue, locationArray[randomFeature][1]);
}

