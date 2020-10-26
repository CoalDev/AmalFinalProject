//===============================================
//                   Example3_5
//===============================================
#include "TFT9341.h"
#include "Wire.h"
#define Tout 2 
byte hour=12,minute=59 ,second=57;
void setup() {
  unsigned int  i;
     lcd.begin();
     Wire.begin();
     setupTime();
     setupTout();
     pinMode(Tout,INPUT);
     lcd.clrscr();
     lcd.setFont(4);
     lcd.gotoxy(40,70);
     lcd.println("RTC DS1307");
     lcd.drawRect(35,65, 250, 100,BLUE);
     lcd.drawRect(34,64, 251, 101,BLUE);
     }

void loop() {
    readTime();
    while(digitalRead(Tout)==0);
     lcd.gotoxy(38,110);
     lcd.setFont(5);
     lcd.print(hour/10,DEC);
     lcd.print(hour%10,DEC);
     lcd.print(":");
     lcd.print(minute/10,DEC);
     lcd.print(minute%10,DEC);
     lcd.print(":");
     lcd.print(second/10,DEC); 
     lcd.print(second%10,DEC);
    while(digitalRead(Tout)==1);
  
}
  
 
//===============================================
//         End Of File " Example3_5"        
//===============================================

