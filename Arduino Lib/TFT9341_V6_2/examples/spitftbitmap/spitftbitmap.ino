#include <SdFat.h>
#include <SPI.h>

#include "TFT9341.h"
#include "bitmap.h"

#define SD_CS 4

void setup(void) {
  SdFat sd;
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  if (!sd.begin(SD_CS, SPI_QUARTER_SPEED)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("OK!");

  lcd.begin();
  lcd.clrscr();
  
  bmp::draw("photo1.bmp", 0, 0);
}

void loop() {
}


