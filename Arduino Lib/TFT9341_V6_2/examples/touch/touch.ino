//===============================================
#include "spiUnit.h"
#include "TFT9341.h"
#include "touch.h"
#include <SPI.h>

byte screen=1,ch=1;
word x,y;

void screen1(){
  ch=0;
  lcd.clrscr (BLACK);
  lcd.fillRoundRect (20,20,70,40,5,RED);
  lcd.setColor(WHITE,RED);
  lcd.setFont (2);
  lcd.gotoxy (25,30);
  lcd.print ("PRESS");
}


void screen2(){
  ch=0;
  lcd.clrscr (BLUE);
  lcd.fillRoundRect (20,20,70,40,5,YELLOW);
  lcd.setColor(BLACK,YELLOW);
  lcd.setFont (2);
  lcd.gotoxy (25,30);
  lcd.print ("NEXT");
  
}

void screen3(){
  ch=0;
  lcd.clrscr (RED);
  lcd.fillRoundRect (20,20,70,40,5,CYAN);
  lcd.setColor(BLACK,CYAN);
  lcd.setFont (2);
  lcd.gotoxy (25,30);
  lcd.print ("OK");
  
}
 
void setup() {	
  lcd.begin();
  lcdtouch.begin();
  lcdtouch.InitTypeTouch(2);//0,1,2
  lcd.setRotation(1);//0,1,2,3

  
}

void loop() {
  if (ch)
    switch (screen){
      case 1: screen1(); break;
      case 2: screen2(); break;
      case 3: screen3(); break;
    }

  if (digitalRead(2)==0){
    lcdtouch.readxy();
    x = lcdtouch.readx();
    y = lcdtouch.ready();

    switch (screen){
    case 1:
      if ((x>20) && (x<90) && (y>20) && (y<60)){
        screen=2;
        ch=1;
      }
      break;
    case 2:
      if ((x>20) && (x<90) && (y>20) && (y<60)){
        screen=3;
        ch=1;
      }
      break;
      
    case 3:
      if ((x>20) && (x<90) && (y>20) && (y<60)){
        screen=1;
        ch=1;
      }
      break;
    
    }//switch  
  }//if irq
}//loop
//===============================================
