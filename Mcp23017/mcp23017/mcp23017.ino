#include <Wire.h>
#include <PrintEx.h>

#define PCA9548ADDR 0x70
#define MCP23017ADDR 0x20

#define I2C_CHANNEL_SDSC0 0x01
#define I2C_CHANNEL_SDSC1 0x02
#define I2C_CHANNEL_SDSC2 0x04
#define I2C_CHANNEL_SDSC3 0x08
#define I2C_CHANNEL_SDSC4 0x10
#define I2C_CHANNEL_SDSC5 0x20
#define I2C_CHANNEL_SDSC6 0x40
#define I2C_CHANNEL_SDSC7 0x80

#define DCF_MINUTE_TRANSIENT 1500
#define DCF_SHORT_PULSE_MIN       70
#define DCF_SHORT_PULSE_MAX       110
#define DCF_LONG_PULSE_MIN        185
#define DCF_LONG_PULSE_MAX        210


//#define DEBUG
#ifdef DEBUG
#define DBG(...)    serial.print(__VA_ARGS__)
#define DBGLN(...)  serial.println(__VA_ARGS__)
#define DBGF(...)   serial.printf(__VA_ARGS__)
#else
#define DBG(...)
#define DBGLN(...)
#define DBGF(...)
#endif

enum dcfStates {
    INITIAL_STATE,
    FIND_HIGH_STATE,
    FIND_LOW_STATE,
    FIND_MINUTE_STATE
};

byte result = 0;
PrintEx serial = Serial;
byte minShortPulse = 255;
byte maxShortPulse;
byte minLongPulse = 255;
byte maxLongPulse;

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
        Wire.beginTransmission(MCP23017ADDR);

        if ((ledState == HIGH) && (currentMillis - previousMillis) >= OnTime) {
            previousMillis = currentMillis;
            ledState = LOW;
            mask &= ~ledPin;
            Wire.write(0x13);
            Wire.write(mask);
        } else if ((ledState == LOW) && (currentMillis - previousMillis) >= OffTime) {
            previousMillis = currentMillis;
            ledState = HIGH;
            mask |= ledPin;
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
    Serial.begin(115200);
    Wire.begin();

    //select SDA3 / SCL3: GPA = output / GPB = output
    prepareMcp23017AtBus(I2C_CHANNEL_SDSC3, 0x00);
    //select SDA3 / SCL3: GPA = output / GPB7/GPB0: output / GPB6-GPB1: input
    prepareMcp23017AtBus(I2C_CHANNEL_SDSC0, 0x7E);
}

void loop() {
    // put your main code here, to run repeatedly:
    led1.Update();
    led2.Update();
    led3.Update();
    led4.Update();

    checkButtonPress(readIoExpander(I2C_CHANNEL_SDSC0));
    checkDcfState(readIoExpander(I2C_CHANNEL_SDSC0));
}

void selectI2cChannels(int channels) {
    Wire.beginTransmission(PCA9548ADDR);
    Wire.write(channels);
    Wire.endTransmission();
}

void checkButtonPress(int result) { // bit GPB1 of MCP23017
    static boolean buttonState = false;
    static boolean state = false;
    if ((result & 0x02) && !buttonState) {
        DBGF("result = 0x%02x\n", result);
        if (state == false) {
            state = true;
        } else {
            state = false;
        }
        buttonState = true;
        Wire.beginTransmission(0x20);
        Wire.write(0x13);
        // Start initially with LED's on.  If initially LED's should be off, switch ternary operators.
        Wire.write(state ? (result | 0x01) : (result & 0xFE));
        Wire.endTransmission();
    } else if (!(result & 0x02) && buttonState) {
        DBGF("result = 0x%02x\n", result);
        buttonState = false;
    }
}

void checkDcfState(int result) { // bit GPB6 of MCP23017
    static unsigned long referenceTime = 0;
    static boolean pulseHigh = false;
    static byte counter = 0;
    unsigned long currentMillis;

    Wire.beginTransmission(0x20);
    Wire.write(0x13);
    Wire.write((result & 0x40) ? (result | 0x80) : (result & 0x7F));
    Wire.endTransmission();

    if (!minuteTransientDetected(result)) {
        return;
    }

    currentMillis = millis();
    
    if ((result & 0x40) && !pulseHigh) {
        pulseHigh = true;
        referenceTime = currentMillis;
    } else if (!(result & 0x40) && pulseHigh) {
        unsigned long timeElapsed = currentMillis - referenceTime;
        pulseHigh = false;

        if ((timeElapsed >= DCF_SHORT_PULSE_MIN) && (timeElapsed <= DCF_SHORT_PULSE_MAX)) {
            if (timeElapsed < minShortPulse) { 
                minShortPulse = (byte)timeElapsed;
            }
            if (timeElapsed > maxShortPulse) { 
                maxShortPulse = (byte)timeElapsed;
            }
            if (counter++ == 58) {
                SayLn("0");
                DBGF("minShortPulse = %d, maxShortPulse = %d, minLongPulse = %d, maxLongPulse = %d\n", minShortPulse, maxShortPulse, minLongPulse, maxLongPulse);
                counter = 0;
            } else {
                Say("0-");
            }
        } else if ((timeElapsed >= DCF_LONG_PULSE_MIN) && (timeElapsed <= DCF_LONG_PULSE_MAX)) {
            if (timeElapsed < minLongPulse) {
                minLongPulse = (byte)timeElapsed;
            }
            if (timeElapsed > maxLongPulse) { 
                maxLongPulse = (byte)timeElapsed;
            }
            if (counter++ == 58) {
                SayLn("1");
                DBGF("minShortPulse = %d, maxShortPulse = %d, minLongPulse = %d, maxLongPulse = %d\n", minShortPulse, maxShortPulse, minLongPulse, maxLongPulse);
                counter = 0;
            } else {
                Say("1-");
            }
        }
    }
}

boolean minuteTransientDetected(int result) {
    static boolean minuteDetected = false;
    static enum dcfStates dcfState = INITIAL_STATE;
    static unsigned long referenceTime = 0;
    static boolean printed = false;

    // Check for minute syncronisation, where there's no pulse for 2 seconds.
    //
    //    58         59         00         01
    //   +--+       ....       +--+       +--+
    //   |  |       .  .       |  |       |  |
    // --+  +------------------+  +-------+  +--.........

    // If minute interval not yet detected (minuteIntervalPassed = false):
    // 1) Check for high level
    // 2) Check for transient to low: take reference time
    // 3) Check for transient to high: take current time, make subtraction with reference and check if > 1000ms
    // 4) If so, we have the first second of a new minute
    // 5) If not, go back to 2) and repeat process

    if (!minuteDetected) {
        switch (dcfState) {
        case INITIAL_STATE:
            DBGLN("In INITIAL_STATE");
            SayLn("Waiting for minute synchronisation...");
            dcfState = ((result & 0x40) ? FIND_LOW_STATE : FIND_HIGH_STATE);
            break;

        case FIND_HIGH_STATE:
            if (!(result & 0x40)) {
                if (!printed) {
                    printed = true;
                    DBGLN("In FIND_HIGH_STATE");
                }
            } else {
                DBGLN("Move to FIND_LOW_STATE");
                dcfState = FIND_LOW_STATE;
                printed = false;
            }
            break;

        case FIND_LOW_STATE:
            if (result & 0x40) {
                if (!printed) {
                    printed = true;
                    DBGLN("In FIND_LOW_STATE");
                }
            } else {
                referenceTime = millis();
                DBGF("Move to FIND_MINUTE_STATE, reference time = %ul\n", referenceTime);
                dcfState = FIND_MINUTE_STATE;
                printed = false;
            }
            break;

        case FIND_MINUTE_STATE:
            if (!(result & 0x40)) {
                if (!printed) {
                    printed = true;
                    DBGLN("In FIND_MINUTE_STATE");
                }
            } else {
                unsigned long timeElapsed = millis() - referenceTime;
                if ((timeElapsed) < DCF_MINUTE_TRANSIENT) {
                    DBGF("Minute not found yet, delta time = %d, move back to FIND_HIGH_STATE...\n", timeElapsed);
                    dcfState = FIND_HIGH_STATE;
                    printed = false;
                } else {
                    SayLn("Minute found, continue...");
                    SayLn("0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5");
                    SayLn("0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9");
                    minuteDetected = true;
                }
            }
            break;

        default:
            break;
        }
    }

    return minuteDetected;
}


void prepareMcp23017AtBus(byte channel, byte mask) {
    selectI2cChannels(channel);
    // Port GPA
    Wire.beginTransmission(MCP23017ADDR);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
    // Port GPB
    Wire.beginTransmission(MCP23017ADDR);
    Wire.write(0x01);
    Wire.write(mask);
    Wire.endTransmission();
}


int readIoExpander(int channel) {
    selectI2cChannels(channel);
    Wire.beginTransmission(0x20);
    Wire.write(0x13);
    Wire.endTransmission();
    Wire.requestFrom(0x20, 1);
    return Wire.read();
}

void Say(char x[]) {
    serial.print(x);
}

void SayLn(char x[]) {
    serial.println(x);
}

void Say(int x) {
    serial.println(x);
}

