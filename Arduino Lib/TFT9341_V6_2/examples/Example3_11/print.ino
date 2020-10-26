//===============================================
//                   print
//===============================================
void printKey(){
byte i;
lcd.setColor(BLUE);
lcd.setFont(2);

for(i=0;i<5;i++){
    lcd.gotoxy(30 + i*40,40);
    lcd.write('C'+i);
    lcd.print(5);
    }

for(i=0;i<2;i++){
    lcd.gotoxy(230 + i*40,40);
    lcd.write('A'+i);
    lcd.print(5);
    }

 for(i=0; i<7 ; i++)
    lcd.drawRect(20+40*i,60, 40, 170,BLACK);
 }//End of printkey function
  
void printKeyTon(byte val){
    lcd.fillRect(21+40*val,61,39,169,color[val]);
    tone(speakerPin,toneArr[val]);
    delay(350);
    lcd.fillRect(21+40*val,61,39,169,WHITE);
}//End of printKeyTon function
//===============================================
//         End Of File "print"        
//===============================================  

