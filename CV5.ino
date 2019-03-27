#include <SD.h>
#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>
#include <Arduino.h>
#include <math.h>

void setup(){
    EsploraTFT.begin();
    EsploraTFT.background(0,0,0);
    Serial.begin(9600);
}

char temperature[4];

void loop()
{
    int incomingByte = Serial.read(); // r -114 g - 103 b - 98
    if(incomingByte == 114)        
        Esplora.writeRGB(255,0,0);
    if(incomingByte == 103)        
        Esplora.writeRGB(0,255,0);
    if(incomingByte == 98)        
        Esplora.writeRGB(0,0,255);
    Serial.println(Esplora.readTemperature(DEGREES_C));
    delay(10);
}