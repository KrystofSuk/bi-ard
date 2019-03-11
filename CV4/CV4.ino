#include <SD.h>
#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>
#include <Arduino.h>
#include <math.h>
enum MODE {
  HOME_SEL_TEMP, HOME_SEL_MIC, HOME_SEL_ACCE, TEMP, MIC, ACCE
};
MODE curr = HOME_SEL_TEMP;
MODE NEXT_STATE = HOME_SEL_TEMP;

void setup(){
    EsploraTFT.begin();
    EsploraTFT.background(0,0,0);
    display_menu();
  Serial.begin(9600);        // initialize serial communications with your computer
}

void setTextProperties(int _r, int _g, int _b, int _size){
  EsploraTFT.stroke(_b,_g,_r);
  EsploraTFT.setTextSize(_size);
}

void display_menu(){
    
    EsploraTFT.background(0,0,0);
    setTextProperties(255,255,255,1);
    EsploraTFT.text("  1. Temperature",30,50);
    EsploraTFT.text("  2. Microphone",30,60);
    EsploraTFT.text("  3. Akcelerometer",30,70);
    change_position();
}

void change_position(){
    setTextProperties(0,0,0,1);
    EsploraTFT.fill(0,0,0);
    if(NEXT_STATE == HOME_SEL_TEMP){
        EsploraTFT.rect(30,60,10,20);
        setTextProperties(255,255,255,1);
        EsploraTFT.text(">",30,50);
    }
    if(NEXT_STATE == HOME_SEL_MIC){
        EsploraTFT.rect(30,50,10,10);
        EsploraTFT.rect(30,70,10,10);
        setTextProperties(255,255,255,1);
        EsploraTFT.text(">",30,60);
    }
    if(NEXT_STATE == HOME_SEL_ACCE){
        EsploraTFT.rect(30,50,10,20);
        setTextProperties(255,255,255,1);
        EsploraTFT.text(">",30,70);
    }
}


int SW1 = 0;
int SW2 = 0;
int SW3 = 0;
int SW4 = 0;

int up_pressed = 0;
int down_pressed = 0;
int left_pressed = 0;
int right_pressed = 0;
char temperature[4];

unsigned long time_now = 0;
int xMic = 0;

int buttonEvent(int c){
    if(c == 1){
        if(SW1 && !down_pressed){
            SW1 = 0;
            return 1;
        }
    }
    if(c == 2){
        if(SW2 && !left_pressed){
            SW2 = 0;
            return 1;
        }
    }
    if(c == 3){
        if(SW3 && !up_pressed){
            SW3 = 0;
            return 1;
        }
    }
    if(c == 4){
        if(SW4 && !right_pressed){
            SW4 = 0;
            return 1;
        }
    }
    return 0;
}
void process_inputs(){
    
    if(Esplora.readButton(SWITCH_3) == LOW){
        up_pressed = 1;
        SW3 = 1;
    }else{
        if(up_pressed){
            up_pressed = 0;        
        }
    }

    if(Esplora.readButton(SWITCH_1) == LOW){
        down_pressed = 1;
        SW1 = 1;
    }else{
        if(down_pressed){
            down_pressed = 0;
        }
    }   
    if(Esplora.readButton(SWITCH_2) == LOW){
        left_pressed = 1;
        SW2 = 1;
    }else{
        if(left_pressed){
            left_pressed = 0;
        }
    }
    if(Esplora.readButton(SWITCH_4) == LOW){
        right_pressed = 1;
        SW4 = 1;
    }else{
        if(right_pressed){
            right_pressed = 0;
        }
    }
}

void draw_temp(){

    if(millis() > time_now + 2000){
        String(Esplora.readTemperature(DEGREES_C)).toCharArray(temperature, 4);
        EsploraTFT.text(temperature,40,60);
        time_now = millis();
    }
}

int yLast = 0;
void draw_mic(){    
    
    if(millis() > time_now + 50){
        int value = Esplora.readMicrophone();
        value = ((double)value/1023.0)*256;
        xMic++;
        if(xMic > 160){
            xMic = 0;
        }
        EsploraTFT.stroke(0,0,0);
        EsploraTFT.fill(0,0,0);
        EsploraTFT.rect(xMic,0,1,160);
        EsploraTFT.stroke(250,180,10);
        EsploraTFT.line(xMic-1, EsploraTFT.height() - yLast - 5, xMic,  EsploraTFT.height() - value - 5);
        yLast = value;
        time_now = millis();
    }
}

int x = 80;
int y = 60;
void draw_acce(){
    if(millis() > time_now + 40){
        EsploraTFT.stroke(0,0,0);        
        EsploraTFT.fill(0,0,0);
        EsploraTFT.rect(x-3, y-3, 6, 6);
        double xAxis = Esplora.readAccelerometer(X_AXIS);    // read the X axis
        double yAxis = Esplora.readAccelerometer(Y_AXIS);    // read the Y axis
    
        xAxis = ((double)xAxis/1023.0)*500;
        x = -xAxis + 80;
        yAxis = ((double)yAxis/1023.0)*500;
        y = yAxis + 54;
        if(x > 160-4)
            x = 160-4;
        if(x < 0+4)
            x = 0+4;
        if(y > 128-4)
            y = 128-4;
        if(y < 0+4)
            y = 0+4;

        EsploraTFT.stroke(250,180,10);        
        EsploraTFT.fill(250,180,10);
        EsploraTFT.circle(x, y, 2);
        
        time_now = millis();
    }
}

void loop()
{
    process_inputs();
   switch (curr)
   {
      case HOME_SEL_TEMP:                       // MENU - TEMPERATURE
          if(buttonEvent(1))                 // je stisknuto tlačítko 'pohyb dolu'?
          {                  
            NEXT_STATE = HOME_SEL_MIC;
            change_position();
          }
          else if(buttonEvent(4))                
          {                  
            NEXT_STATE = HOME_SEL_ACCE;
            change_position();
          }
          else if (buttonEvent(2))          // je stisknuto tlačítko 'vybrat senzor'?
          {
            NEXT_STATE = TEMP;
            EsploraTFT.background(0,0,0);
            setTextProperties(255,255,255,1);
            EsploraTFT.text("  Temperature:",30,50);
            String(Esplora.readTemperature(DEGREES_C)).toCharArray(temperature, 4);
            EsploraTFT.text(temperature,40,60);
            time_now = millis();
          }
          break;

      case HOME_SEL_MIC:                        // MENU - MICROPHONE
          if (buttonEvent(1))                // je stisknuto tlačítko 'pohyb dolu'?
          {
            NEXT_STATE = HOME_SEL_ACCE;
            change_position();
          }
          else if (buttonEvent(4))             // je stisknuto tlačítko 'pohyb nahoru'?
          {
            NEXT_STATE = HOME_SEL_TEMP;
            change_position();
          }
          else if (buttonEvent(2))          // je stisknuto tlačítko 'vybrat senzor'?
          {
            NEXT_STATE = MIC;
            xMic = 0;
            EsploraTFT.background(0,0,0);
          }
          break;

      case HOME_SEL_ACCE:                       // MENU - ACCELEROMETER
          if (buttonEvent(4))                  // je stisknuto tlačítko 'pohyb nahoru'?
          {
            NEXT_STATE = HOME_SEL_MIC;
            change_position();
          }
          else if(buttonEvent(1))                 // je stisknuto tlačítko 'pohyb dolu'?
          {                  
            NEXT_STATE = HOME_SEL_TEMP;
            change_position();
          }
          else if (buttonEvent(2))          // je stisknuto tlačítko 'vybrat senzor'?
          {
            EsploraTFT.background(0,0,0);
            NEXT_STATE = ACCE;
          }
          break;

      case TEMP:                                // DRAW TEMPERATURE
          if(buttonEvent(3))                 // je stisknuto tlačítko 'zpět do menu'?
          {                    
            NEXT_STATE = HOME_SEL_TEMP;            
            display_menu();
          }
          else
          {
            draw_temp();   
          }
          break;

      case MIC:                                 // DRAW MICROPHONE
          if(buttonEvent(3))                 // je stisknuto tlačítko 'zpět do menu'?
          {                     
            NEXT_STATE = HOME_SEL_MIC;
            display_menu();
          }
          else
          {
            draw_mic();                         // funkce draw_mic vykreslí křivku podle momentálního zvuku z mikrofonu
          }
          break;

      case ACCE:                                //  DRAW ACCELEROMETER
          if(buttonEvent(3))                 // je stisknuto tlačítko 'zpět do menu'?
          {                    
            NEXT_STATE = HOME_SEL_ACCE;
            display_menu();
          }
          else
          {
            draw_acce();                 
          }
          break;
   }
   curr = NEXT_STATE;
}