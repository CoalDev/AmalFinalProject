//===============================================
//                   Example3_10
//===============================================
#include "SPI.h"
#include "TFT9341.h"
#include "touch.h"
unsigned int x,y;
byte counter,temp=100;

void setup() {  
  lcd.begin();
  lcdtouch.begin();
   lcdtouch.InitTypeTouch(2);//0,1,2
  lcd.setRotation(0);//0,1,2,3
  
  lcd.clrscr();
  lcd.fillRoundRect (50,160,80, 40, 10 ,BLUE);
  lcd.fillRoundRect (200,160,80, 40, 10 ,BLUE);
  lcd.setColor (WHITE);
  lcd.setFont(2);
  lcd.gotoxy(65,172);
  lcd.print("DOWN");
  lcd.gotoxy(230,172);
  lcd.print("UP");
  lcd.setColor (BLUE);
  lcd.setFont(3);
  lcd.gotoxy(103,42);
  lcd.print("LED BLUE"); 
  lcd.gotoxy(160,82);
  lcd.print(counter); 
  }//End of setup function
  
void loop() {
  while(digitalRead(2)==1);
  lcdtouch.readxy();
  x = lcdtouch.readx();
  y = lcdtouch.ready();
  if(x > 50 && x<50+80 && y> 160 && y< 160+40)
       if(counter==0)
          counter=0;
          else
       counter--; 
  if(x > 200 && x<200+80 && y> 160 && y< 160+40)
   if(counter==255)
       counter=255;
     else
       counter++; 
   if(temp != counter){
     lcd.gotoxy(155,80);
     lcd.print("         ");
     lcd.gotoxy(155,80);
     lcd.print(counter); 
      analogWrite(5,counter);
      temp=counter; 
    }  
}//End of loop function
//===============================================
//         End Of File " Example3_10"        
//===============================================


