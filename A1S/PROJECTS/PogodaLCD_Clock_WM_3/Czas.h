#include "time.h"
const char* ntpServer = "3.pl.pool.ntp.org";
const long  gmtOffset_sec      = 3600;   //12600;
const int   daylightOffset_sec = 3600;

/*
      long epochTime;
      const char powitanie[] PROGMEM = {"Radio blacha"};
      
      const char dzien_0[] PROGMEM = "Niedziela";
      const char dzien_1[] PROGMEM = "Poniedziałek";
      const char dzien_2[] PROGMEM = "Wtorek";
      const char dzien_3[] PROGMEM = "środa";
      const char dzien_4[] PROGMEM = "Czwartek";
      const char dzien_5[] PROGMEM = "Piątek";
      const char dzien_6[] PROGMEM = "Sobota";
      
      const char *const daysOFweek[] PROGMEM = {dzien_0,dzien_1,dzien_2,dzien_3,dzien_4,dzien_5,dzien_6};
      char buffer[16];
*/
  




String pad(int liczba){
  String p="";
  if (liczba<10) p="0";
  return p+String(liczba);
}

/*
 long getTimeStamp() {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      return(0);
    }
    time(&now);
    return now;
}


String getTimeAll(int typ=0){
       struct tm timeinfo;
       if(!getLocalTime(&timeinfo)){
          Serial.println("Failed to obtain time");
          return "...!";
      }
      //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      int weekDay = timeinfo.tm_wday;
      int dzien   = timeinfo.tm_mday;
      int miesiac = timeinfo.tm_mon + 1;
      int rok     = timeinfo.tm_year + 1900;
      int godzina = timeinfo.tm_hour;
      int minuta  = timeinfo.tm_min;
      int sekunda = timeinfo.tm_sec;
      int dzienrok= timeinfo.tm_yday;
      snprintf(buffer, 16, daysOFweek[weekDay]);
      if (typ==0) return pad(godzina)+":"+pad(minuta)+":"+pad(sekunda);
      if (typ==1) return pad(rok)+"-"+pad(miesiac)+"-"+pad(dzien)+", "+pad(godzina)+":"+pad(minuta);
      if (typ==2) return pad(rok)+"-"+pad(miesiac)+"-"+pad(dzien)+", "+pad(godzina)+":"+pad(minuta)+":"+pad(sekunda)+", "+ String(buffer);
      return "";
}
*/
String getTime(){
       struct tm timeinfo;
       if(!getLocalTime(&timeinfo)){
          Serial.println("Failed time");
          return "?";
      }
      int godzina = timeinfo.tm_hour;
      int minuta  = timeinfo.tm_min;
      int sekunda = timeinfo.tm_sec;     
      return pad(godzina)+":"+pad(minuta)+":"+pad(sekunda);
}



void TimersSetup(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //epochTime = getTimeStamp();
}





/*
Szkic używa 858 121 bajtów (27%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 39 632 bajtów (12%) pamięci dynamicznej, pozostawiając 288048 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.


Szkic używa 857 009 bajtów (27%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 39 616 bajtów (12%) pamięci dynamicznej, pozostawiając 288064 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.

Szkic używa 856 949 bajtów (27%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 39 616 bajtów (12%) pamięci dynamicznej, pozostawiając 288064 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.

Szkic używa 856 933 bajtów (27%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 39 616 bajtów (12%) pamięci dynamicznej, pozostawiając 288064 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.




*/
