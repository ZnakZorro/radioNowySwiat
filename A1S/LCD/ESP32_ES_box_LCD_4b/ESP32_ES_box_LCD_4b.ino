

#include "Arduino.h"
#include "WiFi.h"
#include <HTTPClient.h>
#include "SPI.h"
//#include "SD.h"
//#include "FS.h"
#include "Wire.h"
#include "WiFiMulti.h"

#include "ES8388.h"  // https://github.com/maditnerd/es8388
#include "Audio.h"   // https://github.com/schreibfaul1/ESP32-audioI2S
#include "credentials.h"
#include "extra.h"
#include "ESPio.h"
#include "serWi.h"



#define SD_CS         21

// GPIOs for SPI
#define SPI_MOSI      13
#define SPI_MISO      12
#define SPI_SCK       14

// I2S GPIOs
#define I2S_SDOUT     26
#define I2S_BCLK       5
#define I2S_LRCK      25
#define I2S_MCLK       0

// I2C GPIOs
#define IIC_CLK       23
#define IIC_DATA      18

// Amplifier enable
#define GPIO_PA_EN    19

#define LED_BUILTIN   22
unsigned long licznik = 0;

int volume        = 64;  // 0...100
int cur_volume    = 6+0;
int cur_station   = 1;
int last_stations = 2;

unsigned long secundDelay = 1000*2;  
unsigned long lastSecundDelay = 0;  

unsigned long timerDelay15m = 1000 * 60 *15*2;  // 15minut *2
unsigned long lastTime = 0;

String radioBuffer;
String jsonBuffer;
int loopa=0;
String linia2="";
char   znakS = (char)223;

WiFiMulti wifiMulti;
ES8388 es;
Audio audio;
ESPio clio;

//----------------------------------------------------------------------------------------------------------------------
void onScreens(const char *linia1, const char *linia2, int lineNR){
  clio.drukLCD(linia2);
  Serial.printf("======================================\n %u :: %s:: %s\n", lineNR,linia1,linia2);
}



void pogoda2LCD(){
      loopa = (loopa+1) % 12;
      Serial.println(loopa);
      //Serial.println(jsonBuffer);
      String linia1 = getValue(jsonBuffer, loopa);
      if (loopa==1)  linia1 = linia1+" "+znakS+"C";
      if (loopa==2)  linia1 += " hPa";
      if (loopa==3)  linia1 += " m/s";
      if (loopa==4)  linia1 += " %";
      if (loopa==5)  linia1 += " mm 1h";
      if (loopa==6)  linia1 += " mm 6h";
      if (loopa==7)  linia1 += " mm 12h";
      if (loopa==8)  linia1 += " za 1h";
      if (loopa==9)  linia1 += " za 6h";
      if (loopa==10) linia1 += " za 12h";
      if (loopa==11) linia1 = radioBuffer;
      
      clio.clear(); 
      clio.println(linia1,1);
      clio.println(linia2,0);
      /*
      lcd.setCursor(0, 0);
      lcd.print(linia2);
      lcd.setCursor(0, 1);
      lcd.print(linia1);*/

      linia2=linia1;
      lastSecundDelay = millis();
}


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



void Pogoda(){
  Serial.println("Pogoda...");
     if(WiFi.status()== WL_CONNECTED){
      String serverPath = proxyHost;
      Serial.println(serverPath);
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  
}


void setup()
{
  Serial.begin(115200);
    clio.initLED(LED_BUILTIN);
    clio.ledled();    
    clio.initLCD(IIC_DATA, IIC_CLK);
    clio.drukLCD("clio.drukLCD");
 
    Serial.println("\r\nReset");
    Serial.printf_P(PSTR("Free mem=%d\n"), ESP.getFreeHeap());

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    SPI.setFrequency(1000000);
    SD.begin(SD_CS);
    //WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        licznik++;
        clio.drukLCD("WiFi::"+String(licznik));
        delay(333);
    }

    Serial.printf_P(PSTR("Connected\r\nRSSI: "));
    Serial.print(WiFi.RSSI());
    Serial.print(" IP: ");
    Serial.println(WiFi.localIP());

    Serial.printf("Connect to ES8388 codec... ");
    while (not es.begin(IIC_DATA, IIC_CLK))
    {
        Serial.printf("Failed!\n");
        delay(1000);
    }
    Serial.printf("OK\n");

    es.volume(ES8388::ES_MAIN, 0);
    es.volume(ES8388::ES_OUT1, 0);
    es.volume(ES8388::ES_OUT2, 0);
    
    es.mute(ES8388::ES_OUT1, false);
    es.mute(ES8388::ES_OUT2, false);
    es.mute(ES8388::ES_MAIN, false);

    // Enable amplifier
    pinMode(GPIO_PA_EN, OUTPUT);
    digitalWrite(GPIO_PA_EN, HIGH);

    audio.setPinout(I2S_BCLK, I2S_LRCK, I2S_SDOUT);
    audio.i2s_mclk_pin_select(I2S_MCLK);
    audio.setVolume(cur_volume); // 0...21
    //audio.setTone(4,-2,6);
    audio.setTone(6,-3,6);
    
     /* lcd.clear(); 
      lcd.setCursor(0, 0);
      lcd.print(WiFi.localIP());
      lcd.setCursor(0, 1);
      lcd.print(WiFi.RSSI());*/
      clio.drukiLCD(String(WiFi.localIP()).c_str(),  String(WiFi.RSSI()).c_str() );
       //clio.drukiLCD("1111","222");
       Pogoda(); 
      delay(500);
      playCurStation();
      
}
//----------------------------------------------------------------------------------------------------------------------

void playCurStation(){
    if (cur_station < 0) cur_station = 4;
    if (cur_station > 4) cur_station = 0; 
    es.volume(ES8388::ES_MAIN, 0);
    es.volume(ES8388::ES_OUT1, 0);
    es.volume(ES8388::ES_OUT2, 0);
    audio.connecttohost(clio.radia[cur_station].stream);   
    onScreens("cur_station",("Station="+String(cur_station)).c_str(),144);
    Serial.print("clio.radia=");
    Serial.print(clio.radia[cur_station].info);
    Serial.println(clio.radia[cur_station].stream);
}
void setCurVolume(){
    if (cur_volume < 0)  cur_volume = 0;
    if (cur_volume > 19) cur_volume = 20;
    int ampli = clio.radia[cur_station].ampli;
    if (cur_volume<2) ampli = 0;
    audio.setVolume(cur_volume + ampli); // 0...21
    onScreens("cur_volume",("Volume="+String(cur_volume)).c_str(),144);
    //savePreferences();
}

void loop()
{
    audio.loop();

  if ((millis() - lastTime) > timerDelay15m) {
      Pogoda(); 
      Serial.println("Pogoda --------------");  
      lastTime = millis();
  }
  
  if ((millis() - lastSecundDelay) > secundDelay) {
    Serial.println("pogoda2LCD --------------"); 
      pogoda2LCD();
      lastSecundDelay = millis();
  }
    

                                //----------------------------------------------------------------------------------------------------------------------
                                if (Serial.available() > 0) {
                                    String odczyt = Serial.readString(); 
                                    odczyt.trim();
                                    int odczytInt = odczyt.toInt();                                     
                                      //Serial.println("\noooooooooooooooo");
                                      Serial.println(odczyt);
                                      //Serial.println(odczytInt);
                                    if(odczyt.length()>5) {
                                      audio.stopSong();
                                      audio.connecttohost(odczyt.c_str());
                                      log_i("free heap=%i", ESP.getFreeHeap());
                                    }
                                    if (odczyt == "-9") {Serial.println("-9x");ESP.restart();}
                                    if (odczytInt>0 && odczytInt<3) {
                                      Serial.println(odczytInt);
                                      cur_station = odczytInt-1;
                                      playCurStation();
                                      }
                                    if (odczyt == "+") {cur_station++;playCurStation();}
                                    if (odczyt == "-") {cur_station--;playCurStation();}
                                    if (odczyt == "*") {cur_volume++;setCurVolume();}
                                    if (odczyt == "/") {cur_volume--;setCurVolume();}
                                    
                                      if (odczyt == "pl") audio.connecttospeech("Dzie?? dobry. Litwo! Ojczyzno moja! ty jeste?? jak zdrowie: Ile ci?? trzeba ceni??, ten tylko si?? dowie, Kto ci?? straci??. Dzi?? pi??kno???? tw?? w ca??ej ozdobie Widz?? i opisuj??, bo t??skni?? po tobie.", "pl");
                                      if (odczyt == "q1")  audio.setTone(0,0,0);
                                      if (odczyt == "q2")  audio.setTone(0,4,0);
                                      if (odczyt == "q3")  audio.setTone(6,0,6);
                                      if (odczyt == "q4")  audio.setTone(6,-3,6);
                                      if (odczyt == "q5")  audio.setTone(6,-6,6);
                                      if (odczyt == "q6")  audio.setTone(6,-9,6);
                                      if (odczyt == "q7")  audio.setTone(6,-12,6);
                                      if (odczyt == "q8")  audio.setTone(6,-24,6);
                                      if (odczyt == "q9")  audio.setTone(6,-40,6);
                                  
                                }
                                //----------------------------------------------------------------------------------------------------------------------


    
}
//----------------------------------------------------------------------------------------------------------------------

// optional
/*void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}*/
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    //Serial.print("station     ");Serial.println(info);
    onScreens("Station::",String(info).c_str(),183);
    radioBuffer = String(info);
    es.volume(ES8388::ES_MAIN, volume);
    es.volume(ES8388::ES_OUT1, volume);
    es.volume(ES8388::ES_OUT2, volume);
    
}
void audio_showstreaminfo(const char *info){
    //Serial.print("streaminfo  ");Serial.println(info);
    onScreens("streaminfo::",String(info).c_str(),187);
    radioBuffer = String(info);
}
void audio_showstreamtitle(const char *info){
    //Serial.print("streamtitle ");Serial.println(info);
    onScreens("Streamtitle::",String(info).c_str(),191);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    //Serial.print("lasthost    ");Serial.println(info);
    onScreens("Lasthost::",String(info).c_str(),203);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}
/*
void serialLoop(){
    //----------------------------------------------------------------------------------------------------------------------
    if (Serial.available() > 0) {
        String odczyt = Serial.readString(); 
        odczyt.trim();
        int odczytInt = odczyt.toInt();                                     
          //Serial.println("\noooooooooooooooo");
          Serial.println(odczyt);
          //Serial.println(odczytInt);
        if(odczyt.length()>5) {
          audio.stopSong();
          audio.connecttohost(odczyt.c_str());
          log_i("free heap=%i", ESP.getFreeHeap());
        }
        if (odczyt == "-9") {
            Serial.println("-9x");
            audio.stopSong();
            es.volume(ES8388::ES_MAIN, 0);
            es.volume(ES8388::ES_OUT1, 0);
            es.volume(ES8388::ES_OUT2, 0);
              es.mute(ES8388::ES_OUT1, true);
              es.mute(ES8388::ES_OUT2, true);
              es.mute(ES8388::ES_MAIN, true);            
            
            delay(1500);
            ESP.restart();
        }
        if (odczytInt>-1 && odczytInt<3) {
          Serial.println(odczytInt);
          cur_station = odczytInt;
          //playCurStation();
          }
        //if (odczyt == "+") {cur_station++;playCurStation();}
        //if (odczyt == "-") {cur_station--;playCurStation();}
        //if (odczyt == "*") {cur_volume++;setCurVolume();}
        //if (odczyt == "/") {cur_volume--;setCurVolume();}
          Serial.print("odczyt=");Serial.println(odczyt);
          if (odczyt == "a") audio.connecttohost("http://stream.rcs.revma.com/ypqt40u0x1zuv");
          if (odczyt == "b") audio.connecttohost("http://stream.rcs.revma.com/ye5kghkgcm0uv");
          if (odczyt == "c") audio.connecttohost("http://pl-play.adtonos.com/tok-fm");
            // maxx 200 chars
            if (odczyt == "p") audio.connecttospeech("Dzie?? dobry. Tu radio z mchu i paproci.", "pl");
            if (odczyt == "r") audio.connecttospeech("Litwo! Ojczyzno moja! ty jeste?? jak zdrowie: Ile ci?? trzeba ceni??, ten tylko si?? dowie, Kto ci?? straci??. Dzi?? pi??kno???? tw?? w ca??ej ozdobie Widz?? i opisuj??, bo t??skni?? po tobie.", "pl");
            if (odczyt == "s") audio.connecttospeech("Dzie?? dobry. Litwo! Ojczyzno moja! ty jeste?? jak zdrowie: Ile ci?? trzeba ceni??, ten tylko si?? dowie, Kto ci?? straci??. Dzi?? pi??kno???? tw?? w ca??ej ozdobie Widz?? i opisuj??, bo t??skni?? po tobie.", "pl");
            if (odczyt == "t") audio.connecttospeech("Panno ??wi??ta, co Jasnej bronisz Cz??stochowy I w Ostrej ??wiecisz Bramie! Ty, co gr??d zamkowy Nowogr??dzki ochraniasz z jego wiernym ludem!", "pl");
          if (odczyt == "q1")  audio.setTone(0,0,0);
          if (odczyt == "q2")  audio.setTone(0,4,0);
          if (odczyt == "q3")  audio.setTone(6,0,6);
          if (odczyt == "q4")  audio.setTone(6,-3,6);
          if (odczyt == "q5")  audio.setTone(6,-6,6);
          if (odczyt == "q6")  audio.setTone(6,-9,6);
          if (odczyt == "q7")  audio.setTone(6,-12,6);
          if (odczyt == "q8")  audio.setTone(6,-24,6);
          if (odczyt == "q9")  audio.setTone(6,-40,6);
          
       
    }
    //----------------------------------------------------------------------------------------------------------------------
}  
*/

/*
Szkic u??ywa 1 074 257 bajt??w (34%) pami??ci programu. Maksimum to 3145728 bajt??w.
Zmienne globalne u??ywaj?? 50 808 bajt??w (15%) pami??ci dynamicznej, pozostawiaj??c 276872 bajt??w dla zmiennych lokalnych. Maksimum to 327680 bajt??w.

Szkic u??ywa 1 074 249 bajt??w (34%) pami??ci programu. Maksimum to 3145728 bajt??w.
Zmienne globalne u??ywaj?? 50 800 bajt??w (15%) pami??ci dynamicznej, pozostawiaj??c 276880 bajt??w dla zmiennych lokalnych. Maksimum to 327680 bajt??w.



 */
