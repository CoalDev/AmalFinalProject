 //===============================================
//	             "timeRtc"
//===============================================
   byte bcdToDec(byte val){
         val=val/16*10 + val%16; 
        return (val);
    }
    
   byte decToBcd(byte val){
     val= val/10*16 + val%10 ;
   return  val;
}
   void setupTime() {
        Wire.beginTransmission(0x68);
        Wire.write(0x0); //stop Oscillator
        Wire.write(decToBcd(second));
        Wire.write(decToBcd(minute));
        Wire.write(decToBcd(hour));
        Wire.endTransmission();
    }//End of setupTime function 
      
 void readTime(){
       Wire.beginTransmission(0x68);
       Wire.write(0);
       Wire.endTransmission();
       Wire.requestFrom(0x68,3);
       if(Wire.available()){ 
           second = bcdToDec(Wire.read());
           minute = bcdToDec(Wire.read());
           hour = bcdToDec(Wire.read()&0b111111); 
        }
    }//End of readTime function 
           
//===============================================
//	  End of file  "timeRtc"
//===============================================
     
      
      
      
      
