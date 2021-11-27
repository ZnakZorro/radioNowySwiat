

#include "Arduino.h"
#include "WiFi.h"
#include "SPI.h"
//#include "SD.h"
//#include "FS.h"
#include "Wire.h"
#include <LiquidCrystal_I2C.h>
#include "ES8388.h"  // https://github.com/maditnerd/es8388
#include "Audio.h"   // https://github.com/schreibfaul1/ESP32-audioI2S
#include "credentials.h"
//#include "extra.h"
#include "ESPio.h"
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x20, lcdColumns, lcdRows); 

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

int volume        = 64;  // 0...100
int cur_volume    = 6;
int cur_station   = 1;
int last_stations = 2;

ES8388 es;
Audio audio;
ESPio clio;

//----------------------------------------------------------------------------------------------------------------------
void onScreens(const char *linia1, const char *linia2, int lineNR){
  Serial.println(clio.radia[0].info);
  Serial.println(linia1);
  Serial.println(linia2);
  //linia2.replace('Ś','S');
  //linia2.replace('ł','l');
  //linia2.replace(' - ','-');
  
    //return;
    Serial.printf("======================================\n %u :: %s:: %s\n", lineNR,linia1,linia2);
    //snprintf(extraInfo, 64, infoString);
    //updateInfo();
    //linia2 =clio.radia[cur_station].info
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print(String(linia2).substring(0,16));
      //String linia22 = getSecondLine(linia2);
      //Serial.print("linia22=");Serial.println(linia22);
      lcd.setCursor(0, 1); lcd.print(String(linia2).substring(16,32));
    
}



void setup()
{
  clio.initLED(LED_BUILTIN);
  clio.ledled();
    Serial.begin(115200);
    Wire.begin(IIC_DATA, IIC_CLK); 
      lcd.init();                    
      lcd.backlight();
      lcd.setCursor(0, 0); lcd.print("1");
      lcd.setCursor(0, 1); lcd.print("2");
    Serial.println("\r\nReset");
    Serial.printf_P(PSTR("Free mem=%d\n"), ESP.getFreeHeap());

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    SPI.setFrequency(1000000);
    SD.begin(SD_CS);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
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
    audio.setTone(4,-2,6);
    
      lcd.clear(); 
      lcd.setCursor(0, 0);
      lcd.print(WiFi.localIP());
      lcd.setCursor(0, 1);
      lcd.print(WiFi.RSSI());
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
                                    if (odczytInt>-1 && odczytInt<3) {
                                      Serial.println(odczytInt);
                                      cur_station = odczytInt;
                                      playCurStation();
                                      }
                                    if (odczyt == "+") {cur_station++;playCurStation();}
                                    if (odczyt == "-") {cur_station--;playCurStation();}
                                    if (odczyt == "*") {cur_volume++;setCurVolume();}
                                    if (odczyt == "/") {cur_volume--;setCurVolume();}
                                   
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
    es.volume(ES8388::ES_MAIN, volume);
    es.volume(ES8388::ES_OUT1, volume);
    es.volume(ES8388::ES_OUT2, volume);
    
}
void audio_showstreaminfo(const char *info){
    //Serial.print("streaminfo  ");Serial.println(info);
    onScreens("streaminfo::",String(info).c_str(),187);
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
Szkic używa 1 074 257 bajtów (34%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 50 808 bajtów (15%) pamięci dynamicznej, pozostawiając 276872 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.

Szkic używa 1 074 249 bajtów (34%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 50 800 bajtów (15%) pamięci dynamicznej, pozostawiając 276880 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.



 */
