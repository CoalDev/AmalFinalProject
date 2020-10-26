//===============================================
//                   Example3_3
//===============================================
#include "TFT9341.h"
void setup() {
unsigned int  i;
 lcd.begin();
 lcd.clrscr(PURPLE);
 lcd.drawTriangle(100,80, 160, 80,120,10,GREEN); 
 lcd.drawRect(200,90, 81, 50,BLUE);
 lcd.drawCircle(80,135, 50,YELLOW);
 delay(1000);
 lcd.fillTriangle(101,79, 159, 79,120,11,YELLOW); 
 lcd.fillRect(201,91, 80, 48,MAGENTA);
 lcd.fillCircle(80,135, 49,RED);
lcd.drawRoundRect (10,190,300,40,10);
 for(i=0;i<278;i++)
   lcd.fillRoundRect(11,191,20+i,38,10,WHITE);
}
void loop() {
}
//===============================================
//         End Of File " Example3_3"
//===============================================


