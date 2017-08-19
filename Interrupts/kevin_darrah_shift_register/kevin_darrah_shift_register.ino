#include <PrintEx.h>

#include <SPI.h>

byte Input, Output, Check=1;
int j;

PrintEx serial = Serial;

void setup(){
    pinMode(13, OUTPUT);//clock
    pinMode(11, OUTPUT);//data
    pinMode(4, OUTPUT);//latch
    pinMode(2, INPUT);//Input from buttons

    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.begin();

    // In his project, 2 shift registers are used: one for output, one for input.
    // First, 0xFF is sent to shift register 1
    // Then 0x00 is sent to shift register 1, while 0xFF is shifted through to shift register 2
    SPI.transfer(255);
    SPI.transfer(0);

    // Latch into shift register 74HCT595
    digitalWrite(4, HIGH);
    digitalWrite(4, LOW);
    
    Serial.begin(115200);
    attachInterrupt(0, pin_read, RISING);
    serial.printf("input = %d, output = %d, check = %d\n", Input, Output, Check); 

}//setup

void loop(){

}//loop


void pin_read(){

    for(j=0; j<50; j++)
    delayMicroseconds(1000);

    Check=1;
    for(j=0; j<8; j++){
        SPI.transfer(Check);
        SPI.transfer(Output);
        digitalWrite(4, HIGH);
        digitalWrite(4, LOW);
        delayMicroseconds(500);
        if(digitalRead(2)==HIGH){
            if(bitRead(Output, j)==1)
            bitWrite(Output, j, 0);
            else
            bitWrite(Output, j, 1);
        }//dig check

        Check = Check<<1;
    }//j
    
    SPI.transfer(255);
    SPI.transfer(Output);
    digitalWrite(4, HIGH);
    digitalWrite(4, LOW);

    while(digitalRead(2)==HIGH){}

}//pin_rea
