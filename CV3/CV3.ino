#include <SD.h>
#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>
#include <Arduino.h>
#include <math.h>
enum MODE { A, B };
MODE curr = A;
int x = 0;
int y = 0;
int r = 0;
int g = 0;
int b = 0;

double angle = 0;

float j = 0;
int ton = 0;
void setup(){
    Serial.begin(9600);

    EsploraTFT.begin();
    EsploraTFT.background(0,0,0);
    Serial.println("Start");
}

int cnt = 0;
int right = 0;
void loop()
{
    if(cnt != 5){
  int num = (random(60000) % 4);             // je třeba, aby barvy blikaly náhodně

  switch(num)
  {
    case 0:
        Esplora.writeRGB(255,0,0);
      push_button(0);       // pokud se rozsvítí červená LED, musíte zmáčknout SWITCH 1
      break;

    case 1:
        Esplora.writeRGB(0,255,0);
      push_button(1);     // pokud se rozsvítí zelená LED, musíte zmáčknout SWITCH 2
      break;

     case 2:
        Esplora.writeRGB(0,0,255);
      push_button(2);       // pokud se rozsvítí modrá LED, musíte zmáčknout SWITCH 3
      break;

     case 3:
        Esplora.writeRGB(255,255,0);
      push_button(3);     // pokud se rozsvítí žlutá LED, musíte zmáčknout SWITCH 4
  }
        cnt++;
    if( cnt == 5 )
    {

  EsploraTFT.stroke(255,255,255);
  EsploraTFT.setTextSize(2);
  EsploraTFT.text("Right : ",0,0);
  EsploraTFT.text(right,0,20);
        if( right > 2 )
        {
  EsploraTFT.text("Winner !",00,40);
        }
        else
        {
  EsploraTFT.text("Loser !",00,40);
        }
    }
  }
}

void push_button( int inp)
{
    for(int i=0; i < 8000; i++)                    // velikost i určuje dobu rozsvícení LED
    {
        if(inp == 0){
      if( Esplora.readButton(SWITCH_1) == LOW)
      {
            right++;
            i = 8000;
      }
      }
      if(inp == 1){
      if( Esplora.readButton(SWITCH_2) == LOW)
      {
            right++;
            i = 8000;
      }
      }
      if(inp == 2){
      if( Esplora.readButton(SWITCH_3) == LOW)
      {
            right++;
            i = 8000;
      }
      }if(inp == 3){
      if( Esplora.readButton(SWITCH_4) == LOW)
      {
            right++;
            i = 8000;
      }
      }
    }
}