/*
Audio.h :: const size_t m_buffSizePSRAM = 4*300000;
https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/
// found this sketch on https://www.mikrocontroller.net/topic/474383      // thanks to Michael U. (amiga)

//Switch orig: 10100   //
//Switch S1  : 01100   //1-OFF, 2-ON, 3-ON, 4-OFF, 5-OFF

sudo chmod a+rw /dev/ttyUSB0
sudo chmod a+rw /dev/ttyUSB1

spotify znakzorro@gmail.com
*/



#define DEBUG 1
//#define WIFI_MENAGER
//#define NEOPIXEL

//tylko jedno proszę
#define SALON
//#define BLAT
//#define BOX
//#define JBL
//#define BUSH

#ifdef SALON
  #define APPimage  "/blat.webp"
  #define APPname   "/blat.web.json"
  #define VOLUMEdef      75
  #define cur_volume_DEF  3
  #define EQcounter_DEF   1
  #define sleep_DEF  false
#endif

#ifdef BLAT
  #define APPimage  "/blat.webp"
  #define APPname   "/blat.web.json"
  #define VOLUMEdef      50
  #define cur_volume_DEF  3
  #define EQcounter_DEF   1
  #define sleep_DEF  false
#endif
#ifdef BOX
  #define APPimage  "/box.webp"
  #define APPname   "/box.web.json"
  #define VOLUMEdef      40
  #define cur_volume_DEF  2
  #define EQcounter_DEF   0 
  #define sleep_DEF  false 
#endif
#ifdef JBL
  #define APPimage  "/jbl.webp"
  #define APPname   "/jbl.web.json"
  #define VOLUMEdef      40
  #define cur_volume_DEF  3
  #define EQcounter_DEF   0
  #define sleep_DEF  true
#endif
#ifdef BUSH
  #define APPimage  "/bush.webp"
  #define APPname   "/bush.web.json"
  #define VOLUMEdef      25
  #define cur_volume_DEF  3
  #define EQcounter_DEF   0 
  #define sleep_DEF  true
#endif

int VolHeadphone = VOLUMEdef;

#include "Arduino.h"



#include "DNSServer.h"
#include "AsyncTCP.h"             //https://github.com/me-no-dev/ESPAsyncTCP 
#include "ESPAsyncWebServer.h"    //https://github.com/me-no-dev/ESPAsyncWebServer
#include "HTTPClient.h"


#ifdef WIFI_MENAGER
  #include <WiFiManager.h>
#else
    #include "WiFi.h"     
    #include "WiFiMulti.h"
#endif


#include "SPI.h"
#include "SD.h"
#include "FS.h"
#include "SPIFFS.h"
#include "Wire.h"
#include "AC101.h"              //https://github.com/schreibfaul1/AC101 old=//https://github.com/Yveaux/AC101
#include "Audio.h"              //https://github.com/schreibfaul1/ESP32-audioI2S

//DNSServer dnsServer;  for captive portal

#ifdef WIFI_MENAGER
#else
  WiFiMulti wifiMulti;
#endif 

// SPI GPIOs
#define SD_CS         13
#define SPI_MOSI      15
#define SPI_MISO       2
#define SPI_SCK       14

// I2S GPIOs, the names refer on AC101
#define I2S_DSIN      25
#define I2S_BCLK      27
#define I2S_LRC       26
#define I2S_MCLK       0
#define I2S_DOUT      35

// I2C GPIOs
#define IIC_CLK       32
#define IIC_DATA      33


// Amplifier enable
#define GPIO_PA_EN    21


unsigned long timeSleep = millis();
unsigned long timeBUTTON = millis();
unsigned long licznikSleep = 1000*3600*4; // 2hour

static AC101 ac;
//int volume = 31;                                  // 0...63

Audio audio;

//#####################################################################
#include <EEPROM.h>
#define EEPROM_SIZE 4

#include "Stations.h"
#include "index.h"
#include "web.h"
#include "credentials.h"

int      cur_equalizer = 0;
bool     out_station = false;
int      cur_station = 1;
int      cur_volume  = cur_volume_DEF;
bool isSD        = false;
//char *extraInfo[64];
char extraInfo[64];

const int numParams = 8;
const char* CoRadomGRA;
//********************************************************************

/*bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb*/
// Klawisze

#define LED_BUILTIN   22

#ifdef NEOPIXEL
  #include <Adafruit_NeoPixel.h>
  // When we instal the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
  // Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED_BUILTIN, NEO_GRB + NEO_KHZ800);
#endif

long licznik = 0;
#define pinA    23
#define pinB    18
#define pinSW    19
bool ledState = false;

int buttonCounter = 0;
int encoderCounter = 3;
int encoderLast = 3;
int8_t EQcounter = EQcounter_DEF;

  struct Button {
    const uint8_t PIN;
    bool pressed;
  };

  Button button3 = {pinSW, false};
  
  void IRAM_ATTR isr3() {
    if (millis() - timeBUTTON < 500) return;
    button3.pressed = true;
    timeBUTTON = millis();
  }
 

#include <ESP32Encoder.h>
ESP32Encoder encoder;


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

/* WS * WS * WS * WS * WS * WS * WS * WS * WS * WS */
void notifyClients(String nota) {
  ws.textAll(nota);
}

/*void notifyClients(const char *info){
  snprintf(extraInfo, 64, info);
  ws.textAll(info);
}*/



void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  Serial.println((char*)data);
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.println(strcmp((char*)data, "toggle"));
    if (strcmp((char*)data, "eq+") == 0) nextEQ(1);
    if (strcmp((char*)data, "eq-") == 0) nextEQ(-1);
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      if (ledState) notifyClients("1"); else notifyClients("0");
      nextEQ(1);
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
/* ws * ws * ws * ws * ws * ws * ws * ws * ws * ws */

void updateInfo(){
  
    notifyClients(getRadioInfo());
}

/*bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb*/

void onScreens(const char *infoString, int lineNR){
    #ifdef DEBUG
      Serial.printf("---------------------------------------\n %u :: %s\n", lineNR,infoString);
    #endif
      //snprintf(extraInfo, 64, infoString);
      //updateInfo();
}


void ledled(){
  #ifdef NEOPIXEL
    pixels.setPixelColor(0, pixels.Color(millis()%255,millis()%255,millis()%255));
    pixels.show();
  #endif
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
} 

void updateEEPROM(){
  timeSleep = millis();
  onScreens("updateEEPROM",226);
     static bool wr = false;
     if (cur_station<0 || cur_volume<0) return;
     if (EEPROM.read(0) != cur_station) {EEPROM.write(0, cur_station); wr=true;}
     if (EEPROM.read(1) != cur_volume)  {EEPROM.write(1, cur_volume);  wr=true;}
     if (EEPROM.read(2) != EQcounter)   {EEPROM.write(2, EQcounter);  wr=true;}
     if (wr) EEPROM.commit(); 
}


//#####################################################################

void sendESPIP(){
  return;
  /*
   ledled();
   String response = "";
    //if(WiFi.status()== WL_CONNECTED){
        IPAddress ipA  = WiFi.localIP(); 
        //String      ip = String(ipA[0])+"."+String(ipA[1])+"."+String(ipA[2])+"."+String(ipA[3]);
        String ip=""; 
        for (int i=0; i<4; i++){ip.concat(ipA[i]); if(i<4) ip.concat(".");}          
        String    rssi = String(WiFi.RSSI()); 
        String    ssid = String(WiFi.SSID());
        String     ret = ip+".."+ssid+".."+rssi;
        #ifdef DEBUG
          onScreens(ret.c_str(),167);
        #endif
        HTTPClient http;   
        http.begin("https://zszczech.zut.edu.pl/e/ip/post/");
        http.addHeader("Content-Type", "text/plain");            
        int httpResponseCode = http.POST(ret);   
        if(httpResponseCode==200){response = http.getString();} else {response = (httpResponseCode);}
        http.end();
        ledled();
    } else {response = ("Error in WiFi connection");} 
    #ifdef DEBUG 
        onScreens(response.c_str(),178); 
    #endif
    */
}
//#####################################################################
void transCSV2Array(String (& arr) [numParams], String csv){
  for (int i = 0; i < numParams; i++)
     arr[i] = strSplit(csv, ';', i);
}

void zapiszSPIFFS(const char *name, const char *content){
    File file = SPIFFS.open(name, FILE_WRITE);
    if(!file){
         Serial.println("error writing");
         return;
    }
    if (file.print(content)) {
          //Serial.print("File was written  zapiszSPIFFS="); Serial.print(name);  Serial.println(content); 
    } else {
          Serial.println("write failed");
    }
    file.close();
}

String odczytajSPIFFS(const char *name){
    File file = SPIFFS.open(name, FILE_READ);
    String content = file.readStringUntil('\n');
    //Serial.println(name); 
    //Serial.println(content);    
    file.close();
    return content;
}
/*
String odczytajStacjeSPIFFS(){
    File file = SPIFFS.open("/s.txt", FILE_READ);
    String content = file.readStringUntil('\n');
    Serial.println(content);    
    file.close();
    return content;
}*/



//#####################################################################


void setup()
{
 #ifdef NEOPIXEL
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0,0,255));
  pixels.show();
#endif  
  Serial.begin(115200);
  #ifdef DEBUG
      onScreens("\n\n+++Reset+++",0);
      //onScreens(String(ESP.getFreeHeap()).c_str(),0);
      //Serial.printf_P(PSTR("Free mem=%d\n"), ESP.getFreeHeap());
  #endif

  pinMode(button3.PIN, INPUT_PULLUP);
  attachInterrupt(button3.PIN, isr3, RISING);  //CHANGE FALLING RISING LOW HIGH
  ESP32Encoder::useInternalWeakPullResistors=UP;
  //encoder.attachHalfQuad(pinA, pinB);   //180   23, 18
  //encoder.attachFullQuad(pinA, pinB); //80 stopni
  encoder.attachSingleEdge(pinA, pinB); //380
  
  Serial.println("Encoder Start = "+String((int32_t)encoder.getCount()));

  
/*bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb*/
  //pinMode(LED_BUILTIN, OUTPUT);
  //ledled();
 
/*bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb*/

     
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  SPI.setFrequency(1000000);
  isSD = SD.begin(SD_CS);
    #ifdef DEBUG
        onScreens(("isSD="+String(isSD)).c_str(),258);
    #endif
  if (!SPIFFS.begin(true)) {
    #ifdef DEBUG
        onScreens("Error SPIFFS",264);
    #endif     
  } 
  ledled();
                   #ifdef WIFI_MENAGER
                      WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
                               WiFiManager wm;
                               bool res;
                               res = wm.autoConnect("znakESP32","znakzorro"); 
                               if(!res) {Serial.println("Failed to connect"); } 
                               else {Serial.println("connected...yeey :)"); sendESPIP();  ledled(); }
                   #else

                                /*String ssidpass = odczytajSPIFFS("/ssid.txt");
                                String ap[numParams];
                                transCSV2Array(ap,ssidpass);
                                if(ap[0] != ""){wifiMulti.addAP(ap[0].c_str(),ap[1].c_str());}*/
                                for (int i = 0; i < 8; i++) {
                                   wifiMulti.addAP(credential[i].ssid, credential[i].pass);
                                } 
                                
                                delay(333);
                                ledled();
                                if(wifiMulti.run() == WL_CONNECTED) 
                                {                
                                      installServer();
                                      delay(333);
                                      ledled();
                                      sendESPIP();                
                                }  else {
                                       #ifdef DEBUG
                                          onScreens("###\n no conect to WiFi \n###",285);
                                          ledled();
                                       #endif
                               }
                   #endif
    //Serial.printf("Connect to AC101 codec... ");
    while (not ac.begin(IIC_DATA, IIC_CLK))
    {
        Serial.printf("AC101 Failed!\n");
        delay(2000);
    }

    #ifdef DEBUG
        onScreens("OK AC101 codec",297);
    #endif
    ledled();

    ac.SetVolumeSpeaker(VOLUMEdef);
    ac.SetVolumeHeadphone(VolHeadphone);
    //ac.DumpRegisters();

    // Enable amplifier
    pinMode(GPIO_PA_EN, OUTPUT);
    digitalWrite(GPIO_PA_EN, HIGH);
  
    // set I2S_MasterClock
    audio.i2s_mclk_pin_select(I2S_MCLK);

    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DSIN);
 
        EEPROM.begin(EEPROM_SIZE);
        int8_t ee0 = EEPROM.read(0);  // cur_station
        int8_t ee1 = EEPROM.read(1);  // cur_volume
        int8_t ee2 = EEPROM.read(2);  // EQcounter
            if (ee0<0) ee0=0;
            if (ee1<0) ee1=4;
            if (ee2<0) ee2=0;
 
        #ifdef DEBUG
            onScreens(("ee0="+String(ee0)).c_str(),399);  // cur_station
            onScreens(("ee1="+String(ee1)).c_str(),400);  // cur_volume
            onScreens(("ee2="+String(ee2)).c_str(),401);  // EQcounter
        #endif
        if (ee0<last_stations) cur_station = ee0;
        if (ee1>0 && ee1<15)   cur_volume  = ee1;
        //if (cur_volume==0)     cur_volume  = 2;
        if (cur_station<-1)    cur_station = 0;
    encoder.setCount(cur_volume);
    buttonCounter = cur_station;
    audio.setVolume(cur_volume); // 0...21
            
            //String odczyt = odczytajSPIFFS("/eq.txt");
            //onScreens(odczyt.c_str(),350);
            /*String as[numParams];
            transCSV2Array(as,odczyt);
            //for (int i = 0; i < numParams; i++) onScreens(as[i].c_str(),10+i);
            //qqq//audio.setTone(as[3].toInt(),as[4].toInt(),as[5].toInt(),as[0].toFloat(),as[1].toFloat(),as[2].toFloat(),as[6].toFloat()); 
            //audio.setTone(as[3].toInt(),as[4].toInt(),as[5].toInt(),as[0].toFloat(),as[1].toFloat(),as[2].toFloat(),as[6].toFloat()); 
            */ 
            EQcounter = ee2;
            setToneNR(EQcounter);
            //audio.setTone( 6, -4, 6, 570, 1500, 8500, 35);       
 
    playCurStation();
    #ifdef NEOPIXEL
      pixels.setPixelColor(0, pixels.Color(64,64,0));
      pixels.show(); 
    #endif   
    
} // EOF setup()


//-----------------------------------------------------------------------

void setToneNR(int8_t EQcoun){
  EQcounter = EQcoun % 12;
    audio.setTone(qqq[EQcounter].l,qqq[EQcounter].m,qqq[EQcounter].h);//,500,1500,4000,2.5);
    //snprintf(extraInfo, 64, ("EQ= "+String(EQcounter)).c_str());
    //onScreens(String(EQcounter).c_str(),54);
    //onScreens(("EQ= "+String(EQcounter)).c_str(),591);
     #ifdef DEBUG
        snprintf(extraInfo, 64, ("eq="+String(EQcounter)+"# "+String(qqq[EQcounter].l)+" | "+String(qqq[EQcounter].m)+" | "+String(qqq[EQcounter].h)).c_str());

        Serial.printf("##473 EQcounter=%d | l=%d | m=%d | h=%d \n", EQcounter, qqq[EQcounter].l,qqq[EQcounter].m,qqq[EQcounter].h);
        //Serial.println("...");
        //Serial.println(qqq[EQcounter].l);
        //Serial.println(qqq[EQcounter].m);
        //Serial.println(qqq[EQcounter].h);
     #endif

}

void playRadomMusic(){
    const int los = random(1,500);
    const String CoJestRadom = "/m/"+String(los)+".mp3"; 
    if (CoRadomGRA=="") CoRadomGRA="/m/0.mp3";
    else CoRadomGRA = CoJestRadom.c_str();
    #ifdef DEBUG
        onScreens(CoRadomGRA,446);
    #endif

    if (isSD==1){
         #ifdef DEBUG
            onScreens(CoRadomGRA,451);
            onScreens(String(SD.exists(CoRadomGRA)).c_str(),452);
        #endif

        if (SD.exists(CoRadomGRA)) audio.connecttoSD(CoRadomGRA);
        //if (SD.exists(CoRadomGRA)) audio.connecttoFS(SD,"/m/0.mp3");
        
    } else {
        CoRadomGRA = "";
        playCurStation();
    }
}
//???????????????????????????????????????????????????????????????????
void playCurStation(){
  onScreens(("cur_station=" + String(cur_station)).c_str(),530);
  if (cur_station <0)cur_station=0;
  out_station=false;
          #ifdef DEBUG
              onScreens(("cur_station=" + String(cur_station)).c_str(),534);            
          #endif
    if (cur_station == -1) {
      playRadomMusic();
      return;
    }
    else {
      CoRadomGRA="";
      String CoJestGrane = "/"+String(stacje[cur_station].info)+".mp3"; 
          #ifdef DEBUG
              onScreens(("CoJestGrane=" + String(CoJestGrane)).c_str(),373);
          #endif
      // tu się wieszał
      const char* stacjaNazwa = CoJestGrane.c_str();
      audio.stopSong();
      //updateEEPROM();
      delay(100);
      audio.connecttoFS(SPIFFS, stacjaNazwa);
      //bool connecttoFS(fs::FS &fs, const char* file);
    }
}


void encoderLoop(){
  encoder.resumeCount();
  // Loop and read the count
  int encoderCounter = encoder.getCount();
  
  // encoder changed 
  if (encoderCounter != encoderLast){
      timeBUTTON = millis();
      timeSleep = millis();
      button3.pressed = false;
      encoder.pauseCount();
        cur_volume = encoderCounter + stacje[cur_station].ampli;
        if (encoderCounter>21) {encoder.setCount(20); cur_volume = 20;}
        if (encoderCounter<0)  {encoder.setCount(-1); cur_volume = 0;}
        encoderLast = encoderCounter;
        Serial.println("#485 Enc= "+String(encoderCounter));       
        audio.setVolume(cur_volume);
        notifyClients(getRadioInfo());
  }
  // button pressed
  if (button3.pressed) {
      timeBUTTON = millis();
      timeSleep = millis();
      button3.pressed = false;
      buttonCounter++;
      cur_station = buttonCounter % 5;
      onScreens(("B3=" + String(buttonCounter)).c_str(),559);     
      playCurStation();
      notifyClients(getRadioInfo());
  }

  //SLEEP

  
  if (millis() - timeSleep > licznikSleep){
      //Serial.println("alarm sleep");
      //gpio_num_t pi=19;
      //e/sp_sleep_enable_ext0_wakeup(pi, 0);
      //esp_sleep_enable_ext1_wakeup(19, ESP_EXT1_WAKEUP_ALL_LOW); //ESP_EXT1_WAKEUP_ANY_HIGH
      if (sleep_DEF){
        Serial.println("!sleep");
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_19,0);
        esp_deep_sleep_start();
      } else {
        
        if (audio.isRunning()){
          Serial.println("!alarm");
            //audio.pauseResume();
            audio.stopSong();
            buttonCounter--;
            delay(1000);
            timeSleep = millis();
        }
      }
  }
  
    /*if(licznik % 300 == 0){ 
        unsigned long zostalo =  (millis() - timeSleep)/(1000);   
        Serial.print("minęło sekund=");
        Serial.println(zostalo);
        //Serial.println((millis()/1000));
        //Serial.println((timeSleep/1000));
        //Serial.println((licznikSleep/1000));
        //ws.cleanupClients();
        //notifyClients("zostalo="+String(zostalo));       
    }*/

 

}





//uint8_t keyFlagLast    = 0;
//uint8_t keyFlagCounter = 0;

void loop()
{
    licznik++;
    if(licznik % 8 == 0) encoderLoop();
    audio.loop();

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
                                    if (odczytInt>0 && odczytInt<30) {Serial.println("0-30");cur_station=odczytInt;playCurStation();}
                                    
                                    /*
                                    if (odczyt == "a") {audio.setTone( 6, -3, -9,  600, 2000, 5000, 25); }
                                    if (odczyt == "b") {audio.setTone( 6, -3, -12, 600, 2000, 5000, 25); }
                                    if (odczyt == "c") {audio.setTone( 6, -3, -15, 600, 2000, 5000, 25); }
                                    if (odczyt == "d") {audio.setTone( 6, -3, -9,  450, 2000, 5000, 25); }
                                    if (odczyt == "e") {audio.setTone( 6, -3, -12, 450, 2000, 5000, 25); }
                                    if (odczyt == "f") {audio.setTone( 6, -3, -15, 450, 2000, 5000, 25); }
                                    if (odczyt == "g") {audio.setTone( 6, -3, -9,  300, 2000, 5000, 25); }
                                    if (odczyt == "h") {audio.setTone( 6, -3, -12, 300, 2000, 5000, 25); }
                                    if (odczyt == "i") {audio.setTone( 6, -3, -15, 300, 2000, 5000, 25); }
                                    */
                                }



  
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------




void nextEQ(int step){
    EQcounter+=step;
    setToneNR(EQcounter);
    updateInfo();
}
/*
void audio_byteAction(const byte byteFlag){
      //Serial.print("\n==");Serial.println(byteFlag);
      int cur_station_last = cur_station;
      int cur_volume_last  = cur_volume;
      switch (byteFlag) {
          case 0b00000001:  {cur_volume--;    if (cur_volume  <  0)            cur_volume  =  0; out_station = false;} break;
          case 0b00000010:  {cur_volume++;    if (cur_volume  > 21)            cur_volume  = 21; out_station = false;} break;
          case 0b00000100:  {cur_station--;   if (cur_station < -1)            cur_station = -1;} break;
          case 0b00001000:  {cur_station++;   if (cur_station > last_stations) cur_station =  0;} break;
          case 0b00000011:  {nextEQ(1);}      break;
          case 0b00001100:  {audio.pauseResume(); delay(200);}     break;
          //default: {}  break;
      }
     
     if (cur_station_last != cur_station) {playCurStation();}
     
     if (cur_volume_last  != cur_volume)  audio.setVolume(cur_volume);
     
     updateEEPROM();
     #ifdef DEBUG
          onScreens(("b="+String(byteFlag)+" S:"+String(cur_station)+" V:"+String(cur_volume)).c_str(),459);
     #endif
}
*/
void audio_eof_mp3(const char *info){ 
    if (info=="/test.mp3") {Serial.print("looop=");Serial.println(info); }
        onScreens(("eof_mp3= "+String(info)).c_str(),689);
        //onScreens(("out_station= "+String(out_station)).c_str(),612);
    if (CoRadomGRA != "") {
      playRadomMusic();
    } else {
      if (out_station == false) 
      audio.connecttohost(stacje[cur_station].stream);    
    }
}


// optional
void audio_showstation(const char *info){
    //notifyClients(String(info));
    snprintf(extraInfo, 64, info);
    #ifdef DEBUG
        //onScreens(("station = "+String(info)).c_str(),706);
        onScreens(info,707);
    #endif
}
void audio_showstreamtitle(const char *info){
   // *WS*
    snprintf(extraInfo, 64, info);    
    #ifdef DEBUG
        //onScreens(("streamtitle= "+String(info)).c_str(),714);
        onScreens(info,715);
    #endif
    notifyClients(getRadioInfo());
}
void audio_lasthost(const char *info){  //stream URL played
    snprintf(extraInfo, 64, info);
    #ifdef DEBUG
        //onScreens(("lasthost = "+String(info)).c_str(),722);
        onScreens(info,723);
        //int playDuration = audio.getTotalPlayingTime();
        //onScreens(("playDuration="+String(playDuration)).c_str(),725);
    #endif    
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
    snprintf(extraInfo, 64, info);
    notifyClients(getRadioInfo());
}
/*
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}

void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}

void audio_info(const char *info){
    Serial.print("info        "); 
    //Serial.println(info);
    Serial.println(audio.getAudioFileDuration());   
}
*/


/*WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW */
/*WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW */
/*WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW WWW */

String getStations(){
    String out = "";
    for (int s=0; s<max_stations; s++){
         out += String(stacje[s].info);
         out += ";";
         out += String(stacje[s].stream);
         out += "\n";
    }
    return out;
}

String getRadioInfo(){
  const String      sep = "!"; 
  String v = String(cur_volume);
  String n = String(cur_station);
  String s = String(stacje[cur_station].info);
  String ri    = String(WiFi.RSSI());
  return n+sep+v+sep+ri+sep+s+sep+String(extraInfo);
}

void goToSleep(){
      esp_sleep_enable_gpio_wakeup();
      esp_deep_sleep_start();
}

void specialAction(int cur_station_TMP){
  //-1=Music   -2=Sleep   -9=Reboot
        #ifdef DEBUG
          onScreens("specialAction",558);
          onScreens(String(cur_station_TMP).c_str(),559);
        #endif
        audio.stopSong();
        delay(1000);
        if (cur_station_TMP == -1){
            onScreens("tu będzie muza",747);
        }
        if (cur_station_TMP == -2){
            goToSleep();
        }
        if (cur_station_TMP == -9){
            ESP.restart();
        }
}

void playFromSPIFFS(String ParamValue){
        String file_stac = "/z"+ParamValue+".txt";
        file_stac = odczytajSPIFFS(file_stac.c_str());
        
        #ifdef DEBUG
          //onScreens("playFromSPIFFS",581);
          //onScreens(String(ParamValue).c_str(),582);
          onScreens(String(file_stac).c_str(),586);
        #endif
        out_station=true;
        audio.connecttohost(String(file_stac).c_str());
}


void audio_ChangeStation(const String ParamValue){
  out_station = false;
  //-1=Music   -2=Sleep   -9=Reboot
  int cur_station_TMP = ParamValue.toInt(); 
  if (cur_station_TMP >100 ) {playFromSPIFFS(ParamValue); return;}  
  if (cur_station_TMP < 0 ) {specialAction(cur_station_TMP); return;}  
    //onScreens(("audio_ChangeStation= "+String(ParamValue)).c_str(),553);
    if (ParamValue=="p")      cur_station++;
    else if (ParamValue=="m") cur_station--;
    else cur_station = ParamValue.toInt();
    if (cur_station <  -1)            cur_station = -1;
    if (cur_station > last_stations)  cur_station = 0;
    //onScreens(("audio_ChangeStation= "+String(ParamValue)).c_str(),559);
    playCurStation();   
}

void installAP(const String ParamValue){
    onScreens(("installAP= "+String(ParamValue)).c_str(),785);
}

void audio_SaveEEPROM(const String ParamValue){
    if (ParamValue=="z")  {
      updateEEPROM();
      return;
    }   
    if (ParamValue=="p")      cur_volume++;
    else if (ParamValue=="m") cur_volume--;
    else cur_volume = ParamValue.toInt();
    if (cur_volume  <  0 )           cur_volume =  0;
    if (cur_volume  > 21)            cur_volume = 21;
    audio.setVolume(cur_volume);
    updateInfo();
}

void  playMp3(String mp3){
     snprintf(extraInfo, 64, mp3.c_str());
     const String CoJestPlay = "/mp3/"+String(mp3);     
     onScreens((String(mp3)).c_str(),605);
     //onScreens((String(CoJestPlay)).c_str(),606);
     audio.connecttoFS(SD,CoJestPlay.c_str());
}



void alarmColor(String hexa){
  #ifdef NEOPIXEL
    onScreens(("hexa"+String(hexa)).c_str(),857);
      //long long number = strtoll( &hexstring[1], NULL, 16);   
      long number = strtoll(&hexa[0], NULL, 16);
      long r = number >> 16;
      long g = number >> 8 & 0xFF;
      long b = number & 0xFF;
      Serial.println(r);
      Serial.println(g);
      Serial.println(b);
      pixels.setPixelColor(0, pixels.Color(r,g,b));
      pixels.show();
   #endif
}


void alarmState(String state){
    #ifdef NEOPIXEL
      //digitalWrite(LED_BUILTIN, state.toInt());
      onScreens(("alarm"+String(state)).c_str(),828);
      //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      //GRB
      if (state=="1")      pixels.setPixelColor(0, pixels.Color(255,0,0));
      else if (state=="0") pixels.setPixelColor(0, pixels.Color(0,128,0));
      else                 pixels.setPixelColor(0, pixels.Color(0,0,255));
      pixels.show();
    #endif
}

void audio_SetEQNr(String ParamValue){
    cur_equalizer = ParamValue.toInt();
    onScreens(String(cur_equalizer).c_str(),420);    
    audio.setTone(qqq[cur_equalizer].l, qqq[cur_equalizer].m, qqq[cur_equalizer].h);

}

void installServer(){
  #ifdef DEBUG
      onScreens("installServer",616);
      onScreens(APPimage,617);
  #endif
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", PAGE_HTML);
    //request->send(SPIFFS, "/index.html", "text/html");
    //request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", PAGE_HTML);
    //request->send(SPIFFS, "/index.html", "text/html");
  });
    
  server.on("/teren.web.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", TEREN_WEB_JSON);
    //request->send(SPIFFS, APPname, "application/manifest+json");
  });
  
  /*server.on("/index1.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index1.html", "text/html");
  });
  server.on("/box.web.json", HTTP_GET, [](AsyncWebServerRequest *request){
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
  server.on("/teren.web.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", TEREN_WEB_JSON);
  });
  server.on("/radio.svg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "image/svg+xml", RADIO_SVG);
  });

  
  // AJAXY
  server.on("/radio", HTTP_GET, [](AsyncWebServerRequest *request){
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
                //#ifdef DEBUG
                   //if(ParamName !="n") onScreens((String("PName | PValue= ")+String(ParamName)+" | "+String(ParamValue)).c_str(),662);
               // #endif  
           
           if (ParamName=="start") {audio.pauseResume();}
           //replay
           
               if (ParamName=="a") {alarmState(ParamValue); }
               if (ParamName=="c") {alarmColor(ParamValue); }
           if (ParamName=="q") audio_SetEQNr(ParamValue); 
           
           if (ParamName=="r") {playCurStation(); }
                   
           if (ParamName=="s")  audio_ChangeStation(ParamValue);       
           
           if (ParamName=="v")  audio_SaveEEPROM(ParamValue);           
            
           if (ParamName=="ap")  installAP(ParamValue);           
                     
           if (ParamName=="mp3")  {
                playMp3(ParamValue);
           }

          if (ParamName=="w") {
              zapiszSPIFFS("/ssid.txt",p->value().c_str());
              Serial.print("#748 radio?w="); Serial.println(ParamValue);
          }

         // prog mem prog mem prog mem prog mem prog mem prog mem
          if (ParamName=="p") {
            Serial.println("ppp3");
                String as[numParams];
                transCSV2Array(as,(String(ParamValue)).c_str());
                        //Serial.println(as[0]);
                       // Serial.println(as[1]);
                        //Serial.println(as[2]);
                        //Serial.println(as[3]); 
                        if (as[3]) {
                          String nnn = "/z"+as[0]+".txt";
                          //Serial.println(nnn); 
                          zapiszSPIFFS(nnn.c_str(),String(as[3]).c_str());
                        }                              
          }

          if (ParamName=="iir") {
            
                    String as[numParams];
                    transCSV2Array(as,p->value());
                    //for (int i = 0; i < numParams; i++) onScreens(as[i].c_str(),100+i);
                    
                    //audio.setTone(as[3].toInt(),as[4].toInt(),as[5].toInt(),as[0].toFloat(),as[1].toFloat(),as[2].toFloat(),as[6].toFloat()); 
                    //324;626;5041;-7;6;-12;2.6;  
                    //audio.setTone(as[3].toInt(),as[4].toInt(),as[5].toInt());//,as[0].toInt(),as[1].toInt(),as[2].toInt(),as[6].toFloat()); 
                   String z = as[7];
                    if(z!=""){
                        #ifdef DEBUG
                          onScreens(("ZapisEQ="+String(z)).c_str(),911); 
                        #endif
                        zapiszSPIFFS("/eq.txt",p->value().c_str());
                    }
          } 
          if (ParamName=="n") { // drugi parametr
              //onScreens(("ParamName="+String(ParamName)).c_str(),917);
              const char* czyLoop = p->value().c_str();
              //onScreens(("czyLoop="+String(czyLoop)).c_str(),919);
          }
                  
          if (ParamName=="u") {
            //snprintf(extraInfo, 64, p->value().c_str());
            //sprintf(main_url,p->value());
            //String main_url     = p->value();
            const char* out_url = p->value().c_str();
            snprintf(extraInfo, 64, out_url);
               #ifdef DEBUG                 
                  onScreens(out_url,929);
              #endif
                out_station=true;
                audio.stopSong();
                delay(100);
                audio.connecttohost(out_url);
            }

          // IRRfilter Biquad Filter Coefficient
          /*if (ParamName=="q") {
              #ifdef DEBUG
                  onScreens("filter IIR",621);
                  onScreens(String(ParamValue).c_str(),622);
              #endif
                       
          }*/
           
          
           
    }
 
    request->send(200, "text/plain",getRadioInfo());
  });
 
  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
      bool gramy = audio.pauseResume();
      #ifdef DEBUG
          onScreens((String("gramy= ")+String(gramy)).c_str(),743);
      #endif
      //audio_setStation(0);
    request->send(200, "text/plain",  getRadioInfo());
  });
 
/*
  server.on("/ap", HTTP_GET, [](AsyncWebServerRequest *request){
            Serial.println("AP===");
            String RedirectUrl = "http://";
            if (ON_STA_FILTER(request)) {
              RedirectUrl += WiFi.localIP().toString();
            } else {
              RedirectUrl += WiFi.softAPIP().toString();
            }
            RedirectUrl += "/index.htm";
            request->redirect(RedirectUrl);
  });
*/
  
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();
  initWebSocket();  
}
