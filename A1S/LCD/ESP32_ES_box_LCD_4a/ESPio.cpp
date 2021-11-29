#include <Arduino.h>
#include "Wire.h"
#include "ESPio.h"
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x20, 16, 2); 
LiquidCrystal_I2C lcd(ooolcdADR, ooolcdColumns, ooolcdRows); 



void ESPio::initLED(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void ESPio::initLCD(int IIC_DATA, int IIC_CLK)
{
      Wire.begin(IIC_DATA, IIC_CLK); 
      //LiquidCrystal_I2C lcd(0x20, 16, 2); 
      lcd.init();                    
      lcd.backlight();
      //lcd.setCursor(0, 0); lcd.print("liniaA");
      //lcd.setCursor(0, 1); lcd.print("liniaB");
      //delay(333);
}

void ESPio::clear()
{
     lcd.clear();
     lcd.setCursor(0, 0); 
}

void ESPio::print(String linia)
{
     lcd.clear();
     lcd.setCursor(0, 0); 
     lcd.print(String(linia).substring(0,16));
     lcd.setCursor(0, 1); 
     lcd.print(String(linia).substring(16,32));
}
void ESPio::drukLCD(String linia)
{
     lcd.clear();
     lcd.setCursor(0, 0); 
     lcd.print(String(linia).substring(0,16));
     lcd.setCursor(0, 1); 
     lcd.print(String(linia).substring(16,32));
}
void ESPio::drukiLCD(char const *linia1, char const *linia2)
{
     lcd.clear();
     lcd.setCursor(0, 0); 
     lcd.print(linia1);
     lcd.setCursor(0, 1); 
     lcd.print(linia2);
}




void ESPio::ledled()
{
  if(_pin) digitalWrite(_pin, !digitalRead(_pin));
}



void ESPio::dot()
{
  digitalWrite(_pin, HIGH);
  delay(50);
  digitalWrite(_pin, LOW);
  delay(50);  
}

void ESPio::dash()
{
  digitalWrite(_pin, HIGH);
  delay(250);
  digitalWrite(_pin, LOW);
  delay(250);
}

bool ESPio::inArray(int pin, byte piny[])
{
  bool ret=false;
  int i=0;
  while (piny[i]) {
    if (piny[i]==pin) ret=true;
    i++;
   }
  return ret;
}

String ESPio::array2json(byte arr[],int len)
{
    String json_str = "[";
    for (int i=0; i<len; i++){
      json_str += String(arr[i]);
      if (i<len-1) json_str += ","; else json_str += "]";
    }
  return json_str;
}

/*
void pisz(const char *linia1, const char *linia2, int lineNR){
  Serial.println(linia1);
  Serial.println(linia2);
}
*/
