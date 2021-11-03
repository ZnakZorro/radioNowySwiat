#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include "AC101.h"
// I2S GPIOs          die Pinnamen beziehen sich auf den AC101
#define I2S_DSIN      25           
#define I2S_BCLK      27
#define I2S_LRC       26
#define I2S_MCLK       0
#define I2S_DOUT      35
// I2C GPIOs
#define IIC_CLK       32
#define IIC_DATA      33
#define GPIO_PA_EN    21
int8_t volume = 40;
String ssid =     "xxxxxxxxx";
String password = "xxxxxxxxx";
bool wifi_flag = false;
static AC101 ac;
Audio audio;
//#####################################################################
void setup()
{
  Serial.begin(115200);
  Serial.println("\r\nReset");
  Serial.printf_P(PSTR("Free mem=%d\n"), ESP.getFreeHeap());
  Serial.printf("Total heap: %d \r\n", ESP.getHeapSize());
  Serial.printf("Free heap: %d \r\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM: %d \r\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM: %d \r\n", ESP.getFreePsram());
// Start WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  wifi_flag = true;
  
  Serial.printf_P(PSTR("Connected\r\nRSSI: "));
  Serial.print(WiFi.RSSI());
  Serial.print(" IP: ");
  Serial.println(WiFi.localIP()); 
  // Start AC101
  Serial.printf("Connect to AC101 codec... ");
  while (not ac.begin(IIC_DATA, IIC_CLK))
  {
    Serial.printf("Failed!\n");
    delay(1000);
  }
  Serial.printf("OK\n");
  ac.SetVolumeSpeaker(volume);
  ac.SetVolumeHeadphone(volume);
//  ac.DumpRegisters();
  // Enable amplifier
  pinMode(GPIO_PA_EN, OUTPUT);
  digitalWrite(GPIO_PA_EN, HIGH);
// Start I2S Aduio  
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DSIN);
  audio.setVolume(10); // 0...21
  audio.connecttohost("http://mp3channels.webradio.antenne.de:80/oldies-but-goldies");
}
//------------------------------------------------------------------
void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.reconnect();
    Serial.println("WiFi verloren...");
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(100);
    } 
  }  
  audio.loop();
}
//################################################################
//---------------------- Audio Tools -----------------------------
//################################################################
void audio_showstreamtitle(const char *info)
{
    Serial.println(info);
}
//------------------------------------------------------------------
