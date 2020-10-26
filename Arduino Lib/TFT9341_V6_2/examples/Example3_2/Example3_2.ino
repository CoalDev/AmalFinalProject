//===============================================
//                   Example3_2
//===============================================
#include "TFT9341.h"
void setup() {	
   lcd.begin();
  }
void loop() {
lcd.clrscr();
lcd.drawPixel(20,30,BLACK);
delay(500);
lcd.drawHLine(0,120, 319 );
delay(500);
 lcd.drawVLine(160,0, 239,RED ); 
 delay(500);
 lcd.drawLine(0,0,319, 239,BLUE ); 
 delay(500);
 lcd.drawLine(0,239,319,0,GREEN ); 
 delay(500);
}

//===============================================
//         End Of File "Example3_2"        
//===============================================


