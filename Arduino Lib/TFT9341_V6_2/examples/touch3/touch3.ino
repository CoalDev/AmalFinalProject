//===============================================
#include "spiUnit.h"
#include "TFT9341.h"
#include "touch.h"
#include <SPI.h>


#define IRQ  2  //IRQ Touch

//* CS - to digital pin 8  (SS pin)
//* SDI - to digital pin 11 (MOSI pin)
//* CLK - to digital pin 13 (SCK pin)

byte screen = 1;
word x, y;



void setup() {
  lcd.begin();
  lcdtouch.begin();
  lcdtouch.InitTypeTouch(2);//0,1,2
  lcd.setRotation(0);//0,1,2,3
  screenMain();


}

void loop() {

  if (digitalRead(IRQ) == 0)
  {
    delay(20);
    lcdtouch.readxy();
    x = lcdtouch.readx();
    y = lcdtouch.ready();

    lcd.setColor(BLACK, CYAN);
    lcd.gotoxy (25, 200);
    lcd.print ("x=");
    lcd.print (x);
    lcd.print ("  ");
    lcd.gotoxy (125, 200);
    lcd.print ("y=");
    lcd.print (y);
    lcd.print ("  ");


    if ((x > 20) && (x < 110) && (y > 20) && (y < 70))
    {
      screen1();
      screenMain();
    }


    if ((x > 20) && (x < 110) && (y > 100) && (y < 150))
    {
      screen2();
      screenMain();
    }


  }
}



void screenMain() {

  lcd.clrscr (BLACK);
  lcd.fillRoundRect (20, 20, 90, 50, 5, RED);
  lcd.fillRoundRect (20, 100, 90, 50, 5, BLUE);
  lcd.setColor(WHITE, RED);
  lcd.setFont (2);
  lcd.gotoxy (25, 30);
  lcd.print ("EXAM1");
  lcd.setColor(WHITE, BLUE);
  lcd.gotoxy (25, 110);
  lcd.print ("EXAM2");

}


void screen1() {

  lcd.clrscr (BLACK);
  //lcd.fillRoundRect (20, 20, 70, 40, 5, CYAN);
  lcd.setColor(BLACK, CYAN);
  lcd.setFont (3);
  lcd.gotoxy (25, 30);
  lcd.println ("OK1");
  lcd.setFont (1);
  lcd.print ("Press Keyboard to return");
  while (digitalRead(IRQ));

}


void screen2() {

  lcd.clrscr (RED);
  //lcd.fillRoundRect (20, 20, 70, 40, 5, CYAN);
  lcd.setColor(BLACK, WHITE);
  lcd.setFont (4);
  lcd.gotoxy (25, 30);
  lcd.println ("OK2");
  lcd.setFont (1);
  lcd.print ("Press Keyboard to return");
  while (digitalRead(IRQ));

}

