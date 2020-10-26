//==================================================
//           "setupTout"
//==================================================

void setupTout(){
    Wire.beginTransmission(0x68);
    Wire.write(0x07); 
    Wire.write(0x10);
    Wire.endTransmission();
}//End of setupTout function
//==================================================
//        End of file "setupTout"
//==================================================

