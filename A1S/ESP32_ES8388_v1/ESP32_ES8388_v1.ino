/*

*/
//Switch orig: 10100   //
//Switch S1  : 01100   //1-OFF, 2-ON, 3-ON, 4-OFF, 5-OFF
//sudo chmod a+rw /dev/ttyUSB0

#define DEBUG 1

#define TEREN
#ifdef TEREN
  #define VOLUMEdef 95
  #define cur_volume_DEF  10
#endif


#include "Arduino.h"
    #include "DNSServer.h"
    #include "ESPAsyncWebServer.h"    //https://github.com/me-no-dev/ESPAsyncWebServer
#include "WiFi.h"
#include "SPI.h"
#include "SD.h"
#include "FS.h"
#include "SPIFFS.h"
#include "Wire.h"
#include "ES8388.h"  // https://github.com/maditnerd/es8388
#include "Audio.h"   // https://github.com/schreibfaul1/ESP32-audioI2S
#include "Stations.h"
#include "index.h"
#include "web.h"
    #include "credentials.h"
    /*
    file: ~\Arduino\libraries\Credentials\Credentials.h
        char ssid[] =     "******";
        char password[] = "******";
    */
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

int cur_station = 1;
int cur_volume = cur_volume_DEF;
int volume = VOLUMEdef;                            // 0...100
char extraInfo[64];

ES8388 es;
Audio audio;
AsyncWebServer server(80);

// PINS
// 21, 22, 19, 23, 18, 5, GDN
#define LED_BUILTIN   22
//#define pinA    //19
//#define pinB    //21
//#define pinC    22
//#define pinD    //23
//#define pinE    //18
#define pin5    5

int ADC=0;
int lastADC=0;

//----------------------------------------------------------------------------------------------------------------------

void onScreens(const char *infoString, int lineNR){
  Serial.printf("---------------------------------------\n %u :: %s\n", lineNR,infoString);
      //snprintf(extraInfo, 64, infoString);
      //updateInfo();
}
/*void ledled(){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
} */

void radioinfo(){
  String CoJestGrane = "/"+String(stacje[cur_station].info)+".mp3"; 
}

void setup(){
    Serial.begin(115200);
    onScreens(String(cur_station).c_str(),0);
    onScreens("Restart...",0);
    onScreens(WIFIplace,0);
    //Serial.println("\r\nReset");
    //Serial.println(WIFIplace);
    //Serial.printf_P(PSTR("Free mem=%d\n"), ESP.getFreeHeap());
    //onScreens("Free mem="+((ESP.getFreeHeap())).c_str(),0);
    onScreens(("Free mem="+String(ESP.getFreeHeap())).c_str(),0);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    SPI.setFrequency(1000000);

    SD.begin(SD_CS);

    WiFi.mode(WIFI_STA);
    WiFi.mode(WIFI_OFF);
    WiFi.begin(ssid, password);

    
    onScreens("WIFI...",0);
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    onScreens(("Connected\r\nIP: "+String(WiFi.localIP())).c_str(),111);
    Serial.println(WiFi.localIP());
    onScreens(("RSSI: "+String(WiFi.RSSI())).c_str(),111);
  
    onScreens("Connect to AC101 codec... ",111);
    while (not es.begin(IIC_DATA, IIC_CLK)){
        onScreens("Failed!!!\n ",111);
        delay(1000);
    }
    onScreens("OK",111);
    
    // Enable amplifier
    pinMode(GPIO_PA_EN, OUTPUT);
    digitalWrite(GPIO_PA_EN, HIGH);
    es.mute(ES8388::ES_OUT1, true);
    es.mute(ES8388::ES_OUT2, true);
    es.mute(ES8388::ES_MAIN, true);

    audio.setPinout(I2S_BCLK, I2S_LRCK, I2S_SDOUT);
	  audio.i2s_mclk_pin_select(I2S_MCLK);
    audio.setTone(qqq[0].l, qqq[0].m, qqq[0].h);
    audio.setVolume(0); 
    playCurStation();
    installServer();
    
    es.volume(ES8388::ES_MAIN, volume);
    es.volume(ES8388::ES_OUT1, volume);
    es.volume(ES8388::ES_OUT2, volume);
    audio.setVolume(cur_volume); // 0...21
    es.mute(ES8388::ES_OUT1, false);
    es.mute(ES8388::ES_OUT2, false);
    es.mute(ES8388::ES_MAIN, false); 
}

/*
BUTTON    GPIO
* TP_VOL  17  n
* TP_SET   9  n
* TP_PLAY  8  n  
* BUT_REC  0  
* BUT_MODE 3  n
*/

/*22=LED*/

#define ADCpin    22

void loop(){
    audio.loop();
    
          ADC = analogRead(ADCpin);
          if (ADC!=lastADC){
              onScreens(String(ADC).c_str(),ADCpin);
              lastADC = ADC;
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
                                    if (odczytInt>0 && odczytInt<30) {
                                      Serial.println("0-30");
                                      cur_station=odczytInt;
                                      playCurStation();
                                      }
                                    if (odczyt == "+") {cur_station++;playCurStation();}
                                    if (odczyt == "-") {cur_station--;playCurStation();}
                                    if (odczyt == "*") {audio.setVolume(++cur_volume);}
                                    if (odczyt == "/") {audio.setVolume(--cur_volume);}
                                   
                                }
                                //----------------------------------------------------------------------------------------------------------------------
}

// optional


void audio_showstation(const char *info){
    onScreens("GPIO_PA_EN=HIGH",207);
    digitalWrite(GPIO_PA_EN, HIGH);
    es.mute(ES8388::ES_OUT1, !true);
    es.mute(ES8388::ES_OUT2, !true);
    es.mute(ES8388::ES_MAIN, !true);
    
    onScreens(("Station::     "+String(info)).c_str(),209);
    //Serial.print("station     ");Serial.println(info);
    snprintf(extraInfo, 64, info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
    snprintf(extraInfo, 64, info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
    snprintf(extraInfo, 64, info);
}


/*
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
    Serial.print("streaminfo  ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
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
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}
*/


String getRadioInfo(){
  const String      sep = "!"; 
  String v = String(cur_volume);
  String n = String(cur_station);
  String s = String(stacje[cur_station].info);
  String ri    = String(WiFi.RSSI());
  return n+sep+v+sep+ri+sep+s+sep+String(extraInfo);
}

void setCurVolume(){
    if (cur_volume < 0)  cur_volume = 0;
    if (cur_volume > 20) cur_volume = 21;
    audio.setVolume(cur_volume); // 0...21
    onScreens(String(cur_volume).c_str(),227);
}

void audio_ChangeVolume(String ParamValue){
    if (ParamValue=="p") cur_volume++;
    if (ParamValue=="m") cur_volume--;
    setCurVolume();
}

void playCurStation(){
    if (cur_station <  0)            cur_station = last_stations;
    if (cur_station > last_stations)  cur_station = 0;    
    onScreens(String(cur_station).c_str(),282);
    //audio.stopSong();
    //audio.setVolume(0);
    digitalWrite(GPIO_PA_EN, LOW);
    es.mute(ES8388::ES_OUT1, true);
    es.mute(ES8388::ES_OUT2, true);
    es.mute(ES8388::ES_MAIN, true);
    
    onScreens("GPIO_PA_EN=LOW",289);
    //es.mute(ES8388::ES_OUT1, true);
    //es.mute(ES8388::ES_OUT2, true);
    //es.mute(ES8388::ES_MAIN, true);     
      audio.connecttohost(stacje[cur_station].stream);
    //es.mute(ES8388::ES_OUT1, false);
    //es.mute(ES8388::ES_OUT2, false);
    //es.mute(ES8388::ES_MAIN, false);     
    //audio.setVolume(cur_volume > 2);
    //audio.setVolume(cur_volume > 1);
    //digitalWrite(GPIO_PA_EN, HIGH);
    //audio.setVolume(cur_volume);
}
void audio_ChangeStation(String ParamValue){
    if (ParamValue=="p") cur_station++;
    if (ParamValue=="m") cur_station--;
    playCurStation();
}
void audio_SetStationNr(String ParamValue){
    cur_station = ParamValue.toInt();
    playCurStation();
}

void audio_SetEQNr(String ParamValue){
    int q = ParamValue.toInt();
    onScreens(String(q).c_str(),305);    
    audio.setTone(qqq[q].l, qqq[q].m, qqq[q].h);
    /*
    if (q==0) audio.setTone(6, -6,6,550,1550,4500,30);
    if (q==1) audio.setTone(6,-12,6,550,1550,4500,30);
    if (q==2) audio.setTone(3,-18,6,550,1550,4500,30);
    if (q==3) audio.setTone(0,  0,0,550,1550,4500,30);
    */
}

void installServer(){
  onScreens("installServer",616);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("/");
    //request->send(SPIFFS, "/index.html", "text/html");
    request->send(200, "text/html", PAGE_HTML);
  });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("/index.html");
    //request->send(SPIFFS, "/index.html", "text/html");
    request->send(200, "text/html", PAGE_HTML);
  });
  server.on("/teren.web.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", TEREN_WEB_JSON);
  });
  server.on("/radio.svg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "image/svg+xml", RADIO_SVG);
  });
    
  /*server.on("/box.web.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, APPname, "application/manifest+json");
  });
  server.on("/box.webp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, APPimage, "image/webp");
  });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.ico", "image/x-icon");
  });
  server.on("/radio.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/radio.json", "application/json");
  });
  server.on("/stations.txt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain",getStations());
  });
  server.on("/box.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/box.js", "application/js");
  });
  */
  
  // AJAXY
  server.on("/radio", HTTP_GET, [](AsyncWebServerRequest *request){
    //Serial.println("/radio");
    //ledled();
    //last_get_url = "";
    /******************************/
     //List all parameters
     int params = request->params();
     for(int i=0;i<params;i++){
          AsyncWebParameter* p = request->getParam(i); 
          //Serial.print("get=");Serial.print(p->name().c_str()); Serial.print("="); Serial.println(p->value().c_str());
          
          String ParamName  = String(p->name());
          String ParamValue = p->value();
          onScreens((ParamName+"="+ParamValue).c_str(),326);
                     
           if (ParamName=="start") audio.pauseResume();
           if (ParamName=="r") playCurStation();
           if (ParamName=="v") audio_ChangeVolume(ParamValue);
           if (ParamName=="s") audio_ChangeStation(ParamValue);
           if (ParamName=="t") audio_SetStationNr(ParamValue);
           if (ParamName=="q") audio_SetEQNr(ParamValue); 
           if (ParamName=="z") ESP.restart();    
    }
 
    request->send(200, "text/plain",getRadioInfo());
  });   // EOF /radio
 
  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
      bool gramy = audio.pauseResume();
      #ifdef DEBUG
          onScreens((String("gramy= ")+String(gramy)).c_str(),743);
      #endif
      //audio_setStation(0);
    request->send(200, "text/plain",  getRadioInfo());
  });
 
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
  
}
