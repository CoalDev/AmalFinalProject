//==================================================
//           "printTimeTft"
//==================================================
   
   void printTimeTft(){
     lcd.gotoxy(88,109);
     lcd.setFont(3);
     lcd.print(hour/10,DEC);
     lcd.print(hour%10,DEC);
     lcd.print(":");
     lcd.print(minute/10,DEC);
     lcd.print(minute%10,DEC);
     lcd.print(":");
     lcd.print(second/10,DEC); 
     lcd.print(second%10,DEC);
     
}//End of printTimeTft function
//==================================================
//        End of file "printTimeTft"
//==================================================     
