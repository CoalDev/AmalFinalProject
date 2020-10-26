    //===============================================
    //                  "getTimeValues "
    //===============================================
     byte readByte() {
     byte num = 0,val;
     do{
       val = Serial.read();
       if (val >= '0' && val <= '9')
           num = num * 10 + (val - '0');
      }while (val != 0xd);
     return num;
    }//End of readByte function 
    //===============================================
    void getTimeValues(){
      lcd.clrscr(BLUE);
      lcd.setColor (WHITE); 
     
      lcd.setFont(2);
     do{
      Serial.read();
    } while(Serial.available());   
    lcd.println("Please enter the current ");
    lcd.print("hour:  ");
    while(!Serial.available());
    hour = readByte();
    lcd.println(hour);
    
    lcd.print("minute: "); 
    while(!Serial.available());
    minute = readByte();
    lcd.println( minute);
    
    lcd.print("second:  ");
    while(!Serial.available());
    
    second = readByte();
    lcd.println(second);
    setupTime(); 
    }//End of getTimeValues function 
    //===============================================
     //   End of file  " getTimeValues "
    //===============================================

