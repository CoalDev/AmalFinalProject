//===============================================
//                   Example3_6
//===============================================
#include "TFT9341.h"
#include "Wire.h"
#define Tout 2 
byte hour=12,minute=59 ,second=57;
void setup() {
  unsigned int  i;
     lcd.begin();
     Serial.begin(9600);
     Wire.begin();
     setupTime();
     setupTout();
     pinMode(Tout,INPUT_PULLUP);
     lcd.clrscr();
  }//End of setup function

void loop() {
  lcd.clrscr(WHITE);
  lcd.setColor(BLUE);
  lcd.drawCircle(160,120, 90,BLUE);
  lcd.drawCircle(160,120, 89,BLUE);
 
  do{
    while(digitalRead(Tout)==0);
    readTime();
    while(digitalRead(Tout)==1);
    printTimeTft();
   }while(!Serial.available());

    getTimeValues();
 
}
  
 
//===============================================
//         End Of File " Example3_6"        
//===============================================

