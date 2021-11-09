#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "credentials.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Czas.h"

#define IIC_CLK       23
#define IIC_DATA      18

String city = "Szczecin";
String countryCode = "PL";


unsigned long secundDelay = 1000;  
unsigned long lastSecundDelay = 0;  

unsigned long timerDelay = 1000 * 60 *15*2;  // 15minut *2
unsigned long lastTime = 0;

String jsonBuffer;

int lcdColumns = 16;
int lcdRows = 2;

int nr=0;
String pusto="       ";
String pola[7] = { "       ","       ","       ","       ","       ","       "};
char   znakS = (char)223;

LiquidCrystal_I2C lcd(0x20, lcdColumns, lcdRows);  


String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  return payload;
}



String rounda(double valD, unsigned char precyzja){
  if (isnan(valD)) return "";
  else
  {
    String ret = String((((double)(int(valD*10))+0.5)/10),precyzja);
    ret.trim();
    if (precyzja==0){
      ret.replace(",","");
    }
    return ret; 
  }
}

String make_str(String str){
    if (str.length()>7) return str;
    String plus="";
    for(int i = 0; i < (7 - str.length()); i++)
        plus += ' ';  
    return plus+str;
}

void Pogoda(){
  Serial.println("Pogoda...");
     if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey+ "&units=metric&lang=PL";
      Serial.println(serverPath);
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      /*Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.print("feels_like:  ");
      Serial.println(myObject["main"]["feels_like"]);
      Serial.print("Pressure:    ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity:    ");
      Serial.println(myObject["main"]["humidity"]);
      Serial.print("Wind Speed:   ");
      Serial.println(myObject["wind"]["speed"]);
      Serial.print("Rain:         ");
      Serial.println(myObject["rain"]["1h"]);*/
      
          String str_rain = "";
          str_rain = rounda(double(myObject["rain"]["1h"]),1);
          if (str_rain != "") str_rain = " R:"+str_rain; else str_rain ="0";
          
        String str_temp = rounda(double(myObject["main"]["temp"]),1);
        String str_feel = rounda(double(myObject["main"]["feels_like"]),1);
            float float_temp = str_temp.toFloat();
            String tz="";  if (float_temp>0) tz="+";
            float float_feel = str_feel.toFloat();
            String fz="";  if (float_feel>0) fz="+";
        
      
        String str_wind = rounda(double(myObject["wind"]["speed"]),0);
        //String linia1 = tz+str_temp+znakS+"C~"+str_feel+" "+str_wind+str_rain+"m/s";
        String linia1 = tz+str_temp+znakS+"C";

        String str_humi = rounda(double(myObject["main"]["humidity"]),0);
        String str_pres = rounda(double(myObject["main"]["pressure"]),0);
          Serial.println(myObject["weather"][0]["description"]);
          Serial.println(myObject["weather"][0]["main"]);
          String opis = myObject.stringify(myObject["weather"][0]["main"]);
          opis.replace("\"","");
          
        //String linia0 = str_pres+" "+str_humi+"% "+opis;
        String linia0 = str_pres+" "+opis;
        Serial.println(linia0);
        Serial.println(linia1);
        
      lcd.clear();      
      lcd.setCursor(9, 0); lcd.print(linia0);
      lcd.setCursor(0, 1); lcd.print(linia1);   
       pola[0] = make_str(fz+str_feel+znakS+"C");  
       pola[1] = make_str(str_wind+"m/s");   
       pola[2] = make_str(str_humi+"%");   
       pola[3] = str_pres+"hPa";
       pola[4] = make_str(opis);
       pola[5] = make_str(str_rain+"mm");  
 
       //for(int i = 0; i < 6; i++) Serial.println(pola[i]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  
}


void setup() {
  Serial.begin(115200);
  Serial.println("\n\n");
  Wire.begin(IIC_DATA, IIC_CLK);
  lcd.init();                    
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Pogoda?");

  WiFi.begin(credential[0].ssid, credential[0].pass);
  Serial.println("\nConnecting");
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(":");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
      lcd.clear(); 
      lcd.setCursor(0, 0);
      lcd.print(WiFi.localIP());
      lcd.setCursor(0, 1);
      lcd.print(WiFi.RSSI());
      delay(3000);
  TimersSetup();
  Pogoda();
  dajCzas();
  lastSecundDelay = millis();
  lastTime        = millis();
}

int loopa=0;

void dajCzas(){
  //epochTime = getTimeStamp();
  //Serial.print("epochTime=");Serial.println(epochTime);
  String czas = getTime();
  //Serial.print("czas=");Serial.println(czas);
  lcd.setCursor(0, 0); 
  lcd.print(czas);
  if (loopa%2==0){
      nr = (nr+1) % 6; 
      lcd.setCursor(9, 0); lcd.print(pusto);
      lcd.setCursor(9, 1); lcd.print(pusto);
      lcd.setCursor(9, 0); lcd.print(pola[nr]);
      lcd.setCursor(9, 1); lcd.print(pola[nr+1]);
    //lcd.setCursor(7, 1);lcd.print(nr);
      if (nr==5) {lcd.setCursor(9, 1); lcd.print(pola[0]);} 
  }
  loopa++;
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
      Pogoda();   
      lastTime = millis();
  }
  
  if ((millis() - lastSecundDelay) > secundDelay) {
      dajCzas();   
      lastSecundDelay = millis();
  }
}



/*
UWAGA: biblioteka LiquidCrystal_I2C działa na architekturze(/architekturach) avr i może nie być kompatybilna z obecną płytką która działa na architekturze(/architekturach) esp32.
Szkic używa 813254 bajtów (25%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 39644 bajtów (12%) pamięci dynamicznej, pozostawiając 288036 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.

{
  "coord": {
    "lon": 14.553,
    "lat": 53.4289
  },
  "weather": [{
    "id": 500,
    "main": "Rain",
    "description": "słabe opady deszczu",
    "icon": "10d"
  }],
  "base": "stations",
  "main": {
    "temp": 8.57,
    "feels_like": 5.19,
    "temp_min": 7.78,
    "temp_max": 9.12,
    "pressure": 1006,
    "humidity": 90
  },
  "visibility": 10000,
  "wind": {
    "speed": 6.71,
    "deg": 210,
    "gust": 13.41
  },
  "rain": {
    "1h": 0.36
  },
  "clouds": {
    "all": 100
  },
  "dt": 1636268504,
  "sys": {
    "type": 2,
    "id": 2034571,
    "country": "PL",
    "sunrise": 1636265532,
    "sunset": 1636298326
  },
  "timezone": 3600,
  "id": 3083829,
  "name": "Szczecin",
  "cod": 200
}
*/
