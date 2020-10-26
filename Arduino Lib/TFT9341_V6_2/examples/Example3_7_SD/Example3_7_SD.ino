//===============================================
//                   Example3_7
//===============================================
#include <SdFat.h>
#include <SPI.h>

#include "TFT9341.h"
#include "bitmap.h"

#define SD_CS 4

void setup() {
  SdFat sd;
  
  Serial.begin(9600);
  lcd.begin();
  lcd.clrscr();
  lcd.print("Initializing SD card...");
    if (!sd.begin(SD_CS, SPI_QUARTER_SPEED)) 
         lcd.println("failed!");
   else
         lcd.println("OK!");
     delay(1000);
     lcd.clrscr();
    
     bmp::draw("admin.bmp", 0, 0);

}//End of setup function 


void loop() {
    }//End of loop function 

//===============================================
//         End Of File "Example3_7"        
//===============================================

