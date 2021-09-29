
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// I2C GPIOs
#define IIC_CLK       23
#define IIC_DATA      18

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

void piszLED4alfa(uint8_t a, uint8_t b){
      //Serial.print(a);Serial.print(", "); Serial.print(b);Serial.print(" = ");  Serial.println(b/10);
      uint8_t a1 = char((a/10)+48);
      uint8_t b1 = char((b/10)+48);
      uint8_t a2 = char((a%10)+48);
      uint8_t b2 = char((b%10)+48);
      if (a1=='0') a1= ' ';
      if (b1=='0') b1= ' ';
    //alpha4.clear();
    alpha4.writeDigitAscii(0, a1);
    alpha4.writeDigitAscii(1, a2,true);
    alpha4.writeDigitAscii(2, b1);
    alpha4.writeDigitAscii(3, b2);
    alpha4.writeDisplay();
}

void piszLED4num(uint8_t a, uint8_t b){
   Serial.print(a);Serial.print(", "); Serial.println(b);
    //writeDigitNum(uint8_t x, uint8_t num, bool dot = false);
    alpha4.clear();
    //alpha4.writeDigitNum(0, b,true);
    alpha4.writeDisplay();
}


void setup() {
  Serial.begin(115200);
  Serial.println("HT16K33 test");
  Wire.begin(IIC_DATA, IIC_CLK);
  alpha4.begin(0x70);
  Serial.println("Start leddisplay!");
  alpha4.setBrightness(5);
}


void loop() {
  for (int i=0; i<60; i++){
    for (int j=0; j<60; j++){
      piszLED4alfa(i,j); 
      delay(1000);
    }
  }
}
