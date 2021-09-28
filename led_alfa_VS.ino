
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// I2C GPIOs
#define IIC_CLK       23
#define IIC_DATA      18

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

void piszLED(uint8_t a, uint8_t b){
  Serial.print(a);Serial.print(", "); Serial.println(b);
  alpha4.clear();
  alpha4.writeDisplay();
  
  uint8_t a1 = (a<10) ? ' ' : '1';
  uint8_t b1 = (b<10) ? ' ' : '1';
  uint8_t a2 = char((a%10)+48);
  uint8_t b2 = char((b%10)+48);
  //if (a>9) a1 = char(49);//(a/10)+48;
  //if (b>9) b1 = char(49);//(b/10)+48;

    alpha4.writeDigitAscii(0, a1);
    alpha4.writeDigitAscii(1, a2);
    alpha4.writeDigitAscii(2, b1);
    alpha4.writeDigitAscii(3, b2);
    alpha4.writeDisplay();
}

void setup() {
  Serial.begin(115200);
  Serial.println("HT16K33 test");
  Wire.begin(IIC_DATA, IIC_CLK);
  alpha4.begin(0x70);  // pass in the address
  Serial.println("Start leddisplay!");
}


void loop() {
  piszLED(0,0); delay(1500);
  piszLED(0,1); delay(1500);
  piszLED(1,0); delay(1500);
  piszLED(1,1); delay(1500);
  piszLED(2,1); delay(1500);
  piszLED(3,1); delay(1500);
  piszLED(4,1); delay(1500);
  piszLED(15,1); delay(1500);
  piszLED(16,1); delay(1500);
  piszLED(17,1); delay(1500);
  piszLED(18,1); delay(1500);
  piszLED(9,2); delay(1500);
  piszLED(9,3); delay(1500);
  piszLED(9,4); delay(1500);
  piszLED(9,15); delay(1500);
  piszLED(9,16); delay(1500);
  piszLED(9,17); delay(1500);
  piszLED(9,18); delay(1500);
  
}
