//===============================================
#include "TFT9341.h"
 
void setup() {	
  lcd.begin();
  lcd.setBackground(BLACK);
  lcd.clrscr();
  lcd.setColor (RED);
  lcd.gotoxy (10,10);
  lcd.setFont(3);
  lcd.println ("Hello World");  
  lcd.setColor (GREEN);
  lcd.setFont(2);
  lcd.println (73, DEC);
  lcd.setColor (BLUE);  
  lcd.println (73, HEX);
  lcd.setColor (YELLOW);  
  lcd.println (73, BIN);
  lcd.setColor (CYAN);  
  lcd.println (73, OCT);
  
}

void loop() {
}
//===============================================
