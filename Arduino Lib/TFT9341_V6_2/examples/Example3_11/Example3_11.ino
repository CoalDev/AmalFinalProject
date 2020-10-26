//===============================================
//                   Example3_11
//===============================================
#include <SPI.h>
#include "TFT9341.h"
#include "touch.h"
#define speakerPin 6
 unsigned int x,y;
unsigned int toneArr[8]={523, 587, 657, 698, 784, 880, 987};
unsigned int color[8]={0xFFE0,0x001F, 0xF800, 0x07E0, 0xF81F, 0xFE00, 0x07FF  };                                        
void setup(){  
     lcd.begin();
     lcdtouch.begin();
      lcdtouch.InitTypeTouch(2);//0,1,2
  lcd.setRotation(0);//0,1,2,3
     lcd.clrscr(WHITE);
     printKey();
    }//End of setup function



void loop() {
     byte sel;
     while(digitalRead(2));
     lcdtouch.readxy();
     x=lcdtouch.readx();
     y=lcdtouch.ready();
     sel=testTouch();
    if(sel!=0xff){
       printKeyTon(sel);
     }     
    noTone(speakerPin);
}//End of loop function
//===============================================
//         End Of File " Example3_11"        
//===============================================

