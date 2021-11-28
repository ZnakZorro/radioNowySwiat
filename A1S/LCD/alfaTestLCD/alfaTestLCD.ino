#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// I2C GPIOs
#define IIC_CLK       23
#define IIC_DATA      18

byte customChar0[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

byte customChar1[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};

byte customChar2[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100
};
// Make custom characters:
byte Heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};
byte Bell[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};
byte Alien[] = {
  B11111,
  B10101,
  B11111,
  B11111,
  B01110,
  B01010,
  B11011,
  B00000
};
byte Check[] = {
  B00000,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
};
byte Speaker[] = {
  B00001,
  B00011,
  B01111,
  B01111,
  B01111,
  B00011,
  B00001,
  B00000
};
byte Sound[] = {
  B00001,
  B00011,
  B00101,
  B01001,
  B01001,
  B01011,
  B11011,
  B11000
};
byte Skull[] = {
  B00000,
  B01110,
  B10101,
  B11011,
  B01110,
  B01110,
  B00000,
  B00000
};
byte Lock[] = {
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};
byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};
LiquidCrystal_I2C lcd(0x20, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  Serial.begin(115200);
  Wire.begin(IIC_DATA, IIC_CLK); 
        lcd.init();                      // initialize the lcd 
          lcd.createChar(0, Heart);
          lcd.createChar(1, Bell);
          lcd.createChar(2, Alien);
          lcd.createChar(3, Check);
          lcd.createChar(4, Speaker);
          lcd.createChar(5, Sound);
          lcd.createChar(6, Skull);
          lcd.createChar(7, Lock);        
        lcd.createChar(8, customChar0); // create a new custom character (index 0)
        lcd.createChar(9, customChar1); // create a new custom character (index 1)
        lcd.createChar(10, customChar2); // create a new custom character (index 2)

  lcd.backlight();
    lcd.clear();
          lcd.write((byte)0);
          lcd.write((byte)1);
          lcd.write((byte)2);
         lcd.write((byte)3);
         lcd.write((byte)4);
         lcd.write((byte)5);
         lcd.write((byte)6);
         lcd.write((byte)7);
         lcd.write((byte)8);
         lcd.write((byte)9);
         lcd.write((byte)10);
         lcd.print((char) 223);
         lcd.print("9" "\xDF" "C");
  
}

void loop()
{
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
          lcd.write((byte)0);
          lcd.write((byte)1);
          lcd.write((byte)2);
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
}
