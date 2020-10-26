//===============================================
//                   Example3_1
//===============================================

#include "TFT9341.h"
void setup() {	
   lcd.begin();
   lcd.clrscr(RED);
   lcd.clrscr(WHITE);
   lcd.setBackground(BLUE);
   lcd.clrscr();
  
   lcd.gotoxy (5,5);
   lcd.setColor (MAGENTA);
   lcd.setFont(1);
   lcd.println ("The number 255 at the base:"); 
   lcd.setColor (RED);
   lcd.println (255, DEC);
   lcd.setColor (BLACK,WHITE); 
   lcd.setFont(5); 
   lcd.println (255, HEX);
   lcd.setBackground(BLUE);
   lcd.setColor (YELLOW); 
   lcd.setFont(6); 
   lcd.println (255, BIN);
   lcd.setColor (CYAN); 
   lcd.setFont(14); 
   lcd.println (255, OCT);
   
}

void loop() {
}
//===============================================
//         End Of File "Example3_1"        
//===============================================


