
#include <SD.h>
#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>
#include <EEPROM.h>


#ifndef Helpers

int SW1 = 0;
int SW2 = 0;
int SW3 = 0;

int r=0,g=0,b=0;

enum states {
  A, B, C, N
};
unsigned long time_now = 0;

enum states STATE, NEXT_STATE;

void processInputs(void){
    if(Esplora.readButton(SWITCH_2) == LOW){
        if(!SW2){
            SW2 = 1;
        }
    }else{
        SW2 = 0;
    }
    if(Esplora.readButton(SWITCH_1) == LOW){
        if(!SW1){
            SW1 = 1;
        }
    }else{
        SW1=0;
        
    }
}

#endif

void setup(){
    STATE = N;
    Serial.begin(9600);

    EsploraTFT.begin();
    EsploraTFT.background(0,0,0);
    Serial.println("Start");
}

void loop(){
    processInputs();  

    switch(STATE){
        case N:
            if(SW1){
                STATE = A;
            }
            if(SW2){
                STATE = B;
            }
            break;
        case A:
            if(SW2){
                STATE = C;
            }
            if(!SW1){
                STATE = N;
                time_now = millis();
                Serial.println("Akce A");
                Esplora.writeRGB(255, 0, 0);
            }
            break;
        case B:
            if(SW1){
                STATE = C;
            }
            if(!SW2){
                STATE = N;
                time_now = millis();
                Serial.println("Akce B");
                Esplora.writeRGB(0, 255, 0);
            }
            break;
        case C:
            if(!SW1 && !SW2){
                STATE = N;
                time_now = millis();
                Serial.println("Akce C");
                Esplora.writeRGB(0, 0, 255);
            }
            break;
    }

    if(millis() > time_now + 100)
        Esplora.writeRGB(0, 0, 0);
}