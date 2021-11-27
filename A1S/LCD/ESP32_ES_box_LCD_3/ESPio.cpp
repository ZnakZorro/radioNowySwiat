#include <Arduino.h>
#include "ESPio.h"

void ESPio::initLED(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void ESPio::ledled()
{
  digitalWrite(_pin, !digitalRead(_pin));
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
