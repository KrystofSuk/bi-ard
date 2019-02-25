
#include <SD.h>
#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>
#include <EEPROM.h>

#define DOTL 200             
#define DASHL 800            
#define NO_LIGHTL 200        
#define NO_LIGHTL_LETTER 400 
#define NO_LIGHTL_WORD 1000        
#define DOWN SWITCH_1     
#define LEFT SWITCH_2        
#define UP SWITCH_3             
#define RIGHT SWITCH_4       

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

int current_char = 0;
int last_current_char = 0;
int word_list [27] = {3,13,4,18, 26 ,20,12,24,9,8, 26 ,18,15,14,17,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int word_list2 [27] = {3,13,4,18, 26 ,20,12,24,9,8, 26 ,18,15,14,17,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int current_index = 0; 
char letters_list[27] = "DNES UMYJI SPORA            ";
char letters_list2[27] = "DNES UMYJI SPORA            ";
char letter[2] = " \0";

int SW1 = 0;
int SW2 = 0;
int SW3 = 0;
int SW4 = 0;

int slider = 0;
int up_pressed = 0;
int down_pressed = 0;
int left_pressed = 0;
int right_pressed = 0;

char colorPrintout[4];
unsigned long time_now = 0;

int run_index = 0;
int letter_index = 0;

char* letters[]={
".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
"...." , "..", ".---", "-.-", ".-..", "--", "-.",
"---", ".--.", "--.-", ".-.", "...", "-", "..-",
"...-", ".--", "-..-", "-.--", "--..", "_"
};

#ifndef Helpers

void setTextProperties(int _r, int _g, int _b, int _size){
  EsploraTFT.stroke(_b,_g,_r);
  EsploraTFT.setTextSize(_size);
}

void writeData(int val, int index){
    EEPROM.write(index, highByte(val));
    EEPROM.write(index+1, lowByte(val));
}

int readData(int index){
    return word(EEPROM.read(index), EEPROM.read(index+1));
}

void processInputs(void){

    if(Esplora.readButton(UP) == LOW){
        up_pressed = 1;
        SW3 = 1;
    }else{
        if(up_pressed){
            up_pressed = 0;        
        }
    }

    if(Esplora.readButton(DOWN) == LOW){
        down_pressed = 1;
        SW1 = 1;
    }else{
        if(down_pressed){
            down_pressed = 0;
        }
    }   
    if(Esplora.readButton(LEFT) == LOW){
        left_pressed = 1;
        SW2 = 1;
    }else{
        if(left_pressed){
            left_pressed = 0;
        }
    }
    if(Esplora.readButton(RIGHT) == LOW){
        right_pressed = 1;
        SW4 = 1;
    }else{
        if(right_pressed){
            right_pressed = 0;
        }
    }

}

#endif

#ifndef MainMenu

void displayMenu(void){
    EsploraTFT.background(0,0,0);
    setTextProperties(255,255,255,1);
    EsploraTFT.text("  1. Set RGB",30,50);
    EsploraTFT.text("  2. Blink Morse",30,60);
    EsploraTFT.text("  Made by Suk",30,10);
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

#ifndef MorseMenu

char getChar(int index){
    if(index == 26){
        index = 95;
    }else{
        index = 65 + index;
    }
    return (char)index;
}

char getLetter(int index){
    if(index == 26){
        index = 32;
    }else{
        index = 65 + index;
    }
    return (char)index;
}

void displayMorseMenu(void){
    EsploraTFT.background(0,0,0);
    current_index = 16;
    for(int i = 0; i < 27; i++){
        word_list[i] = word_list2[i];
        letters_list[i] = ' ';
        letters_list[i] = letters_list2[i];
    }
        
    current_char = Esplora.readSlider();
    current_char = (int)((1.0-((float)current_char / 1023.0)) * 26); 
    displayMorseLetter();    
    displayMorseText();
}

void displayMorseLetter(void){    
    setTextProperties(0,0,0,1);
    EsploraTFT.fill(0,0,0);
    EsploraTFT.rect(80,30,10,15);
    setTextProperties(255,0,0,2);
    letter[0] = getChar(current_char);
    EsploraTFT.text(letter,80,30);
}

void displayMorseText(void){    
    setTextProperties(0,0,0,1);
    EsploraTFT.fill(0,0,0);
    EsploraTFT.rect(0+current_index*6,60,6,10);
    setTextProperties(255,255,255,1);
    Serial.println(letters_list);
    EsploraTFT.text(letters_list,0,60);
}

#endif

void startMorse(){
    run_index = 0;
    letter_index = 0;
    current_mode = MORSE_RUN;
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
    }else{
        Esplora.writeRGB(r, g, b);
    }
}

int getLetterLenght(int index){
    if(index == 4 || index == 19 || index == 26)
        return 1;
    else if(index == 0 || index == 8 || index == 12 || index == 13)
        return 2;
    else if(index == 3 || index == 6 || index == 10|| index == 14|| index == 17|| index == 18|| index == 20|| index == 22)
        return 3;
    else
        return 4;
}

int dif = 0;
int add = 0;

void processMorse(){  

    if(current_mode == MORSE_RUN){
        
        if(current_flash_mode == NO_LIGHT){
            Esplora.writeRGB(0, 0, 0);
            if(millis() > time_now + dif + add + NO_LIGHTL){
                if(letters[word_list[run_index]][letter_index] == '.'){
                    add = 0;
                    Serial.println(".");
                    flash(MORSE_RUN, DOT);
                }
                if(letters[word_list[run_index]][letter_index] == '-'){
                    add = 0;
                    Serial.println("-");                    
                    flash(MORSE_RUN, DASH);
                }
                if(letters[word_list[run_index]][letter_index] == '_'){
                    Serial.println("________");                    
                    add = NO_LIGHTL_WORD;
                }
                letter_index++;
                if(getLetterLenght(word_list[run_index]) <= letter_index){
                    if(add == 0)
                        add = NO_LIGHTL_LETTER;
                    run_index++;
                    if(run_index == 27 || word_list[run_index] == -1)
                        current_mode = MORSE_EDIT;
                    else
                        letter_index = 0;
                }
            }
        }else if(current_flash_mode == DOT){
            if(millis() > time_now + DOTL){
                current_flash_mode = NO_LIGHT;
                dif = DOTL;
            }
        }else if(current_flash_mode == DASH){
            if(millis() > time_now + DASHL){
                current_flash_mode = NO_LIGHT;  
                dif = DASHL;      
            }
        }

    }else{

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

}

void setup(){
    Serial.begin(9600);

    r = readData(0);
    g = readData(2);
    b = readData(4);
    if(r == -1){
        r = 0;
    }
    if(g == -1){
        g = 0;
    }
    if(b == -1){
        b = 0;
    }

    EsploraTFT.begin();
    EsploraTFT.background(0,0,0);
    displayMenu();
}

void loop(){
    processInputs();  
    processMorse();

    switch(current_mode){
        case MENU_A:
            if(!up_pressed && SW3){
                SW3 = 0;
                current_mode = MENU_B;
                displayMenuSelector();
            }
            if(!down_pressed && SW1){
                SW1 = 0;
                current_mode = MENU_B;
                displayMenuSelector();
            }
            if(!left_pressed && SW2){ 
                SW2 = 0;  
                current_mode = RGB_R;
                displayRGBMenu();
                displayRGBActive(current_mode);
            }
            break;
        case MENU_B:
            if(!up_pressed && SW3){
                SW3 = 0;
                current_mode = MENU_A;
                displayMenuSelector();
            }
            if(!down_pressed && SW1){
                SW1 = 0;
                current_mode = MENU_A;
                displayMenuSelector();
            }
            if(!left_pressed && SW2){ 
                SW2 = 0;   
                current_mode = MORSE_EDIT;
                displayMorseMenu();
            }
            break;
        case RGB_R:
            tmp = Esplora.readSlider();
            tmp = (int)((1.0-((float)tmp / 1023.0)) * 255.0);
            if(tmp != last){
                last = tmp;            
                displayRGBActive(current_mode);
            }
            if(!left_pressed && SW2){ 
                SW2 = 0;   
                displayRGBClear(current_mode,0);
                current_mode = RGB_G;
                displayRGBActive(current_mode);
            }
            if(!right_pressed && SW4){ 
                SW4 = 0;   
                flash(current_mode, DOT);
            }            
            if(!down_pressed && SW1){
                SW1 = 0;
                r = tmp;
                writeData(r, 0);
            }
            if(!up_pressed && SW3){
                SW3 = 0;
                current_mode = MENU_A;
                displayMenu();
            }
            break;
        case RGB_G:  
            tmp = Esplora.readSlider();
            tmp = (int)((1.0-((float)tmp / 1023.0)) * 255.0);
            if(tmp != last){
                last = tmp;            
                displayRGBActive(current_mode);
            }
            if(!left_pressed && SW2){ 
                SW2 = 0;   
                displayRGBClear(current_mode,0);
                current_mode = RGB_B;
                displayRGBActive(current_mode);
            }
            if(!right_pressed && SW4){ 
                SW4 = 0;   
                flash(current_mode, DOT);
            }        
            if(!down_pressed && SW1){
                SW1 = 0;
                g = tmp;
                writeData(g, 2);
            }
            if(!up_pressed && SW3){
                SW3 = 0;
                current_mode = MENU_A;
                displayMenu();
            }
            break;
        case RGB_B:    
            tmp = Esplora.readSlider();
            tmp = (int)((1.0-((float)tmp / 1023.0)) * 255.0);
            if(tmp != last){
                last = tmp;            
                displayRGBActive(current_mode);
            }
            if(!left_pressed && SW2){ 
                SW2 = 0;   
                displayRGBClear(current_mode,0);
                current_mode = RGB_R;
                displayRGBActive(current_mode);
            }
            if(!right_pressed && SW4){ 
                SW4 = 0;   
                flash(current_mode, DOT);
            }        
            if(!down_pressed && SW1){
                SW1 = 0;
                b = tmp;
                writeData(b, 4);
            }
            if(!up_pressed && SW3){
                SW3 = 0;
                current_mode = MENU_A;
                displayMenu();
            }
            break;
        case MORSE_EDIT:
            current_char = Esplora.readSlider();
            current_char = (int)((1.0-((float)current_char / 1023.0)) * 26);

            if(current_char != last_current_char){
                last_current_char = current_char;            
                displayMorseLetter();
            }
            
            if(!left_pressed && SW2){ 
                SW2 = 0;   
                if(current_index > 0)                    
                    current_index--;  
                word_list[current_index] = -1;
                letters_list[current_index] = ' ';
                displayMorseText();
            }
            if(!right_pressed && SW4){ 
                SW4 = 0;   
                startMorse();
                current_mode == MORSE_RUN;
            }            
            if(!down_pressed && SW1){
                SW1 = 0;
                letters_list[current_index] = getLetter(current_char);
                word_list[current_index] = current_char;
                if(current_index < 27)                    
                    current_index++;
                displayMorseText();
            }
            if(!up_pressed && SW3){
                SW3 = 0;
                current_mode = MENU_B;
                displayMenu(); 
            }
            break;
        case MORSE_RUN:
            break;
    }

}