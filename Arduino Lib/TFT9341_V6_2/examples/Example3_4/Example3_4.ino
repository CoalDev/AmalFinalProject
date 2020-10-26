//===============================================
//                   Example3_4
//===============================================
#include "TFT9341.h"
void setup() {
  unsigned int  i;
   lcd.begin();
   lcd.clrscr(BLUE);
   lcd.setFont(4);
  }

void loop() {
   byte i;
   for(i=0;i<4;i++){
     lcd.clrscr(BLUE);
     lcd.setRotation(i);
     lcd.gotoxy(10,10);
     lcd.print("TFT9341");
     delay(1000);
   }
  }
//===============================================
//         End Of File " Example3_4"        
//===============================================

