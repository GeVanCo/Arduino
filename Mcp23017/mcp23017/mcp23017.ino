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

byte result = 0;
byte state = 0;
byte oldbutton = 0;

class Flasher {
private:
  long OnTime;
  long OffTime;
  int ledState;
  unsigned long previousMillis;
  int channel;
  int ledPin;

public:
  static int mask;

  
  Flasher(int ledPin, long on, long off, int channel) {
    this->ledPin = ledPin;
    this->OnTime = on;
    this->OffTime = off;
    this->ledState = LOW;
    this->previousMillis = 0;
    this->channel = channel;
  }

  void Update() {
    unsigned long currentMillis = millis();

    selectI2cChannels(channel);
    Wire.beginTransmission(0x20);
    
    if ((ledState == HIGH) && (currentMillis - previousMillis) >= OnTime) {
      previousMillis = currentMillis;
      ledState = LOW;
      mask = mask & ~ledPin;
      Wire.write(0x13);
      Wire.write(mask);
    } else if ((ledState == LOW) && (currentMillis - previousMillis) >= OffTime) {
      previousMillis = currentMillis;
      ledState = HIGH;
      mask = mask | ledPin;
      Wire.write(0x13);
      Wire.write(mask);
    }

    Wire.endTransmission();
  }

private:
  void selectI2cChannels(int channels) 
  {
    Wire.beginTransmission(PCA9548ADDR);
    Wire.write(channels);
    Wire.endTransmission();  
  }  
};

Flasher led1(0x01, 1000, 4000, I2C_CHANNEL_SDSC3);
Flasher led2(0x02, 350, 350, I2C_CHANNEL_SDSC3);
Flasher led3(0x04, 100, 1000, I2C_CHANNEL_SDSC3);
Flasher led4(0x08, 30, 300, I2C_CHANNEL_SDSC3);

int Flasher::mask = 0x00;

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
}

void loop() {
  // put your main code here, to run repeatedly:
  led1.Update();
  led2.Update();
  led3.Update();
  led4.Update();

  selectI2cChannels(I2C_CHANNEL_SDSC0);
  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.endTransmission();
  Wire.requestFrom(0x20, 1);
  result = (Wire.read() & 0x02);
  if (result && !oldbutton) {
    if (state == 0) {
      state = 1;
    } else {
      state = 0;
    }
    oldbutton = 1;
    Wire.beginTransmission(0x20);
    Wire.write(0x13);
    Wire.write(state);
    Wire.endTransmission();
  } else if (!result && oldbutton) {
    oldbutton = 0;
  }
}

void selectI2cChannels(int channels) 
{
  Wire.beginTransmission(PCA9548ADDR);
  Wire.write(channels);
  Wire.endTransmission();  
}
