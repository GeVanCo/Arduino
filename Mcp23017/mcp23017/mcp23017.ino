#include <Wire.h>

#define PCA9548ADDR 0x70

#define I2C_CHANNEL_SDSC0 0x01
#define I2C_CHANNEL_SDSC1 0x02
#define I2C_CHANNEL_SDSC2 0x04
#define I2C_CHANNEL_SDSC3 0x08
#define I2C_CHANNEL_SDSC4 0x10
#define I2C_CHANNEL_SDSC5 0x20
#define I2C_CHANNEL_SDSC6 0x40
#define I2C_CHANNEL_SDSC7 0x80

int ledState = 0;
int result = 0;


void setup() {
  // put your setup code here, to run once:
  delay(2000);
  
  Wire.begin();

  //select SDA3 / SCL3
  selectI2cChannels(I2C_CHANNEL_SDSC3);
  Wire.beginTransmission(0x20);
  //port A as output
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x20);
  //port B as output
  Wire.write(0x01);
  Wire.write(0x00);
  Wire.endTransmission();
//  Wire.beginTransmission(0x20);
//  //Write '1' to GPB0-GPB3
//  Wire.write(0x13);
//  Wire.write(0x0f);
//  Wire.endTransmission();

  //select SDA0 / SCL0
  selectI2cChannels(I2C_CHANNEL_SDSC0);
  Wire.beginTransmission(0x20);
  //port A as output
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x20);
  //port B, GPB0 as output / GPB1-GPB7 as input
  Wire.write(0x01);
  Wire.write(0xFE);
  Wire.endTransmission();
  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.write(0x01);
  Wire.endTransmission();
}

void loop() {
  // put your main code here, to run repeatedly:
  selectI2cChannels(I2C_CHANNEL_SDSC3);
  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(100);
  
  selectI2cChannels(I2C_CHANNEL_SDSC0);
  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.endTransmission();
  Wire.requestFrom(0x20, 1);
  result = Wire.read();
  if (result & 0x02) {
    Wire.beginTransmission(0x20);
    Wire.write(0x13);
    Wire.write(0x00);
    Wire.endTransmission();
  } else {
    Wire.beginTransmission(0x20);
    Wire.write(0x13);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(100);
    Wire.beginTransmission(0x20);
    Wire.write(0x13);
    Wire.write(0x00);
    Wire.endTransmission();
  }
  
//  Wire.beginTransmission(0x20);
//  Wire.write(0x13);
//  if (ledState == 0x01) {
//    ledState = 0x00;
//  } else {
//    ledState = 0x01;
//  }
//  Wire.write(ledState);
//  Wire.endTransmission();
//  
  selectI2cChannels(I2C_CHANNEL_SDSC3);
  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(100);
  
//  selectI2cChannels(I2C_CHANNEL_SDSC0);
//  Wire.beginTransmission(0x20);
//  Wire.write(0x13);
//  if (ledState == 0x01) {
//    ledState = 0x00;
//  } else {
//    ledState = 0x01;
//  }
//  Wire.write(ledState);
//  Wire.endTransmission();
  
  selectI2cChannels(I2C_CHANNEL_SDSC3);
  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.write(0x02);
  Wire.endTransmission();
  delay(100);
  
//  selectI2cChannels(I2C_CHANNEL_SDSC0);
//  Wire.beginTransmission(0x20);
//  Wire.write(0x13);
//  if (ledState == 0x01) {
//    ledState = 0x00;
//  } else {
//    ledState = 0x01;
//  }
//  Wire.write(ledState);
//  Wire.endTransmission();
//  
  selectI2cChannels(I2C_CHANNEL_SDSC3);
  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.write(0x04);
  Wire.endTransmission();
  delay(100);
  
//  selectI2cChannels(I2C_CHANNEL_SDSC0);
//  Wire.beginTransmission(0x20);
//  Wire.write(0x13);
//  if (ledState == 0x01) {
//    ledState = 0x00;
//  } else {
//    ledState = 0x01;
//  }
//  Wire.write(ledState);
//  Wire.endTransmission();
//
  selectI2cChannels(I2C_CHANNEL_SDSC3);
  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.write(0x08);
  Wire.endTransmission();
  delay(100);

//  selectI2cChannels(I2C_CHANNEL_SDSC0);
//  Wire.beginTransmission(0x20);
//  Wire.write(0x13);
//  if (ledState == 0x01) {
//    ledState = 0x00;
//  } else {
//    ledState = 0x01;
//  }
//  Wire.write(ledState);
//  Wire.endTransmission();
}

void selectI2cChannels(int channels) 
{
  Wire.beginTransmission(PCA9548ADDR);
  Wire.write(channels);
  Wire.endTransmission();  
}
