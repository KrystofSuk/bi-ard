#include <SD.h>
#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>
#include <Arduino.h>

int x = 0;
int y = 0;
void setup(){
    Serial.begin(9600);

    EsploraTFT.begin();
    EsploraTFT.background(0,0,0);
    Serial.println("Start");
}

void input(){        
    x = Esplora.readJoystickX();
    if(abs(x) < 3)
        x = 0;
    y = Esplora.readJoystickY();
    y-=16;
    Serial.println(x);
    Serial.println(y);
    if(abs(y) < 3)
        y = 0;
    else
        y+=16;
}

void loop(){
    input();
    int tx = map(abs(x),0 ,512, 0, 255);
    int ty = map(abs(y), 0, 512, 0, 255);
    Esplora.writeRGB(tx, ty, 0);
    Serial.println(tx);
    Serial.println(ty);
    Serial.println("---");
}