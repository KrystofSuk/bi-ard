#include <Esplora.h>

#define DOT 200             // doba bliknutí tečky
#define DASH 800            // doba bliknutí čárky
#define NO_LIGHT 200        // doba vypnutí LED mezi jednotlivými bliknutími
#define BRIGHT_GREEN 50     // intenzita jasu zelené LED

int stateBLINK = 0;
int stateCOLOR = 0;
unsigned long time_now = 0;
int current = 0;
int dur = 0;
int colorMode = 0;

int r = 0;
int g = 255;
int b = 0;


void flash(){
    Esplora.writeRGB(r, g, b);
}

int GetDuration(int index){
    if(index == 0 || index == 2 || index == 4 || index == 6 || index == 7)
        return DOT;
    if(index == 1 || index == 3 || index == 5)
        return DASH;
}

void SetColors(int _r, int _g, int _b){
    r = _r;
    g = _g;
    b = _b;
}

void ChangeColorMode(){
    colorMode++;
    if(colorMode > 6){
        colorMode = 0;
    }
    switch (colorMode)
    {
        case 0:
            SetColors(0, 255, 0);
            break;
        case 1:
            SetColors(0, 0, 255);
            break;
        case 2:
            SetColors(255, 0, 0);
            break;
        case 3:
            SetColors(255, 255, 0);
            break;
        case 4:
            SetColors(0, 255, 255);
            break;
        case 5:
            SetColors(255, 0, 255);
            break;
        case 6:
            SetColors(255, 255, 255);
            break;
    
        default:
            break;
    }
}

void ReadInput(){
    int buttonSTART = Esplora.readButton(SWITCH_UP);
    if(buttonSTART == LOW)
    {
        if(stateBLINK == 0){
            stateBLINK = 1;
            time_now = millis();
            dur = GetDuration(current);
        }
    }
    else {
        stateBLINK = 0;
        Esplora.writeRGB(0, 0, 0);
        current = 0;
        dur = 0;
    }
    
    int buttonCOLOR = Esplora.readButton(SWITCH_LEFT);
    if(buttonCOLOR == LOW)
    {
        if(stateCOLOR == 0){
            ChangeColorMode();
            stateCOLOR = 1;
        }   
    }
    else {
        stateCOLOR = 0;
    }
}

void BlinkProcess(){

}

void setup(){
}

void loop(){
    ReadInput();

    if(stateBLINK == 1){
        if(millis() < time_now + dur){
            flash();
        }else if(millis() < time_now + dur + NO_LIGHT){
            Esplora.writeRGB(0, 0, 0);
        }else{
            current++;
            if(current > 7){
                current = 0;
            }
            dur = GetDuration(current);
            time_now = millis();
        }
    }
}