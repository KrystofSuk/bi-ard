#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>

#define DOTL 200             
#define DASHL 800            
#define NO_LIGHTL 200        
#define DOWN SWITCH_3       
#define BACK SWITCH_3       
#define SELECT SWITCH_2     
#define NEXT SWITCH_2        
#define UP SWITCH_1          
#define SET SWITCH_1            
#define TEST SWITCH_4       

enum mode {
  MENU_A,
  MENU_B,
  RGB_R,
  RGB_G,
  RGB_B,
  MORSE_EDIT,
  MORSE_RUN
};

enum flash_mode{
    NO_LIGHT,
    DOT,
    DASH
};

mode current_mode = MENU_A;
flash_mode current_flash_mode = NO_LIGHT;

int r = 255;
int g = 255;
int b = 255;
int tmp = 0;
int last = 0;

int slider = 0;
int up_pressed = 0;
int down_pressed = 0;
int select_pressed = 0;
int set_pressed = 0;
int next_pressed = 0;
int test_pressed = 0;
int back_pressed = 0;

char colorPrintout[4];

unsigned long time_now = 0;

void setTextProperties(int _r, int _g, int _b, int _size){
  EsploraTFT.stroke(_b,_g,_r);
  EsploraTFT.setTextSize(_size);
}

#ifndef MainMenu

void displayMenu(void){
    EsploraTFT.background(0,0,0);
    setTextProperties(255,255,255,1);
    EsploraTFT.text("  1. Set RGB",30,50);
    EsploraTFT.text("  2. Blink Morse",30,60);
    displayMenuSelector();
}

void displayMenuSelector(void){
    setTextProperties(0,0,0,1);
    EsploraTFT.fill(0,0,0);
    if(current_mode == MENU_A){
        EsploraTFT.rect(30,60,10,10);
        setTextProperties(255,255,255,1);
        EsploraTFT.text(">",30,50);
    }
    if(current_mode == MENU_B){
        EsploraTFT.rect(30,50,10,10);
        setTextProperties(255,255,255,1);
        EsploraTFT.text(">",30,60);
    }
}

#endif 

#ifndef RGBMenu

void displayRGBMenu(void){
    EsploraTFT.background(0,0,0);

    setTextProperties(255,0,0,1);    
    EsploraTFT.text("  R",30,50);  
    setTextProperties(0,255,0,1);
    EsploraTFT.text("  G",30,60);
    setTextProperties(0,0,255,1);
    EsploraTFT.text("  B",30,70);
    
    setTextProperties(255,255,255,1);     
    EsploraTFT.text("   :",30,50);
    EsploraTFT.text("   :",30,60);
    EsploraTFT.text("   :",30,70); 

    displayRGBColor(RGB_R,1);
    displayRGBColor(RGB_G,0);
    displayRGBColor(RGB_B,0);
}

void displayRGBClear(mode m, int temp){      
    setTextProperties(0,0,0,1);
    EsploraTFT.fill(0,0,0);
    if(m == RGB_R){
        EsploraTFT.rect(70,50,40,10);
        setTextProperties(255,255,255,1);   
        String(r).toCharArray(colorPrintout, 4);
        if(temp)
            String(tmp).toCharArray(colorPrintout, 4);
        EsploraTFT.text(colorPrintout,80,50);
    }else if(m == RGB_G){
        EsploraTFT.rect(70,60,40,10);
        setTextProperties(255,255,255,1); 
        String(g).toCharArray(colorPrintout, 4);
        if(temp)
            String(tmp).toCharArray(colorPrintout, 4);
        EsploraTFT.text(colorPrintout,80,60);
    }else if(m == RGB_B){
        EsploraTFT.rect(70,70,40,10);
        setTextProperties(255,255,255,1); 
        String(b).toCharArray(colorPrintout, 4);
        if(temp)
            String(tmp).toCharArray(colorPrintout, 4);
        EsploraTFT.text(colorPrintout,80,70);        
    }
}

void displayRGBColor(mode m, int temp){    
    setTextProperties(0,0,0,1);
    EsploraTFT.fill(0,0,0);
    if(m == RGB_R){
        EsploraTFT.rect(80,50,20,10);
        setTextProperties(255,255,255,1);   
        String(r).toCharArray(colorPrintout, 4);
        if(temp)
            String(tmp).toCharArray(colorPrintout, 4);
        EsploraTFT.text(colorPrintout,80,50);
    }else if(m == RGB_G){
        EsploraTFT.rect(80,60,20,10);
        setTextProperties(255,255,255,1); 
        String(g).toCharArray(colorPrintout, 4);
        if(temp)
            String(tmp).toCharArray(colorPrintout, 4);
        EsploraTFT.text(colorPrintout,80,60);
    }else if(m == RGB_B){
        EsploraTFT.rect(80,70,20,10);
        setTextProperties(255,255,255,1); 
        String(b).toCharArray(colorPrintout, 4);
        if(temp)
            String(tmp).toCharArray(colorPrintout, 4);
        EsploraTFT.text(colorPrintout,80,70);        
    }
}

void displayRGBActive(mode m){  
    displayRGBColor(current_mode,1);
    setTextProperties(255,255,255,1);     
    if(m == RGB_R){  
        EsploraTFT.text("<    >",70,50);
    }else if(m == RGB_G){
        EsploraTFT.text("<    >",70,60);
    }else if(m == RGB_B){
        EsploraTFT.text("<    >",70,70);    
    } 
}

#endif 

void processInputs(void){
    if(current_mode == MENU_A || current_mode == MENU_B){

        if(Esplora.readButton(UP) == LOW){
            up_pressed = 1;
        }else{
            if(up_pressed){
                up_pressed = 0;
                if(current_mode == MENU_A)
                    current_mode = MENU_B;
                else if(current_mode == MENU_B)
                    current_mode = MENU_A;
                displayMenuSelector();
        
            }
         }

        if(Esplora.readButton(DOWN) == LOW){
            down_pressed = 1;
        }else{
            if(down_pressed){
                down_pressed = 0;
                if(current_mode == MENU_A)
                    current_mode = MENU_B;
                else if(current_mode == MENU_B)
                    current_mode = MENU_A;
                displayMenuSelector();
            }
        }        

        if(Esplora.readButton(SELECT) == LOW){
            select_pressed = 1;
        }else{
            if(select_pressed){
                select_pressed = 0;
                if(current_mode == MENU_A){
                    current_mode = RGB_R;
                    displayRGBMenu();
                    displayRGBActive(current_mode);
                }
                else if(current_mode == MENU_B){
                    current_mode = MORSE_EDIT;
                }
            }
        }

    }else if(current_mode == RGB_R || current_mode == RGB_G || current_mode == RGB_B){

        if(Esplora.readButton(NEXT) == LOW){
            next_pressed = 1;
        }else{
            if(next_pressed){
                next_pressed = 0;
                displayRGBClear(current_mode,0);
                switch(current_mode){
                    case RGB_R:
                        current_mode = RGB_G;
                        break;
                    case RGB_G:
                        current_mode = RGB_B;
                        break;
                    case RGB_B:
                        current_mode = RGB_R;
                        break;
                }
                displayRGBActive(current_mode);
            }
        }

        if(Esplora.readButton(TEST) == LOW){
            if(!test_pressed){
                test_pressed = 1;
                flash(current_mode, DOT);
            }
        }else{
            if(test_pressed){
                test_pressed = 0;
            }
        }

        if(Esplora.readButton(SET) == LOW){
            set_pressed = 1;
        }else{
            if(set_pressed){
                set_pressed = 0;
                switch(current_mode){
                    case RGB_R:
                        r = tmp;
                        break;
                    case RGB_G:
                        g = tmp;
                        break;
                    case RGB_B:
                        b = tmp;
                        break;
                }
            }
        }
        
        if(Esplora.readButton(BACK) == LOW){
            back_pressed = 1;
        }else{
            if(back_pressed){
                back_pressed = 0;                
                current_mode = MENU_A;
                displayMenu();
            }
        }

        tmp = Esplora.readSlider();
        tmp = (int)((1.0-((float)tmp / 1023.0)) * 255.0);
        if(tmp != last){
            last = tmp;            
            displayRGBActive(current_mode);
        }
    }
}

void flash(mode m, flash_mode f){    
    time_now = millis();
    current_flash_mode = f;
    if(m == RGB_B){
        Esplora.writeRGB(r, g, tmp);
    }else if(m == RGB_G){
        Esplora.writeRGB(r, tmp, b);
    }else if(m == RGB_R){
        Esplora.writeRGB(tmp, g, b);
    }
}

void setup(){
    EsploraTFT.begin();
    EsploraTFT.background(0,0,0);
    displayMenu();
}

void loop(){
    processInputs();  

    
    if(current_flash_mode == NO_LIGHT){
        Esplora.writeRGB(0, 0, 0);
    }else if(current_flash_mode == DOT){
        if(millis() > time_now + DOTL){
            current_flash_mode = NO_LIGHT;
        }
    }else if(current_flash_mode == DASH){
        if(millis() > time_now + DASHL)
            current_flash_mode = NO_LIGHT;        
    }
}