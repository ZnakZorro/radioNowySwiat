#define last_stations  17+3  //17
#define max_stations  last_stations+1

struct stationsSTRUCT  {
  int  const  ampli;  
  char const *info;
  char const *stream;
};

stationsSTRUCT stacje [last_stations+1]={
  {0, "TOK-FM",    "http://pl-play.adtonos.com/tok-fm"},
  {0, "NowySwiat", "http://stream.rcs.revma.com/ypqt40u0x1zuv"},
  {0, "357",       "http://stream.rcs.revma.com/ye5kghkgcm0uv"},
  {1, "SwissJaz",  "http://stream.srg-ssr.ch/m/rsj/mp3_128"},
  {2, "SwissCls",  "http://stream.srg-ssr.ch/m/rsc_de/mp3_128"},
  {0, "RMF_Class",  "http://www.rmfon.pl/n/rmfclassic.pls"},
  {1, "ClassFM",   "http://media-the.musicradio.com/ClassicFM"},
  {1, "Dwojka",   "http://mp3.polskieradio.pl:8952/;"}, 
  {0, "Niezapom",  "http://195.150.20.242:8000/rmf_niezapomniane_melodie"},
  {0, "RockServ",  "http://stream.open.fm/379"},
  //{0, "NewOnce",   "http://stream.open.fm/374"},
  {1, "101_Jazz",  "https://ais-sa2.cdnstream1.com/b22139_128mp3"},                   
  {0, "Szczecin",  "http://stream4.nadaje.com:11986/prs.aac.m3u"},
  {0, "RMF_FM",  "http://www.rmfon.pl/n/rmffm.pls"},
  //{0, "Zetka",  "http://zet-net-01.cdn.eurozet.pl:8400/listen.pls"},
  {0, "Zetka",  "https://zt03.cdn.eurozet.pl/zet-net.mp3"}, 
  {0, "AntyRadio",  "https://an.cdn.eurozet.pl/ant-waw.mp3"},
  {0, "Eska",  "http://waw01-01.ic.smcdn.pl:8000/2260-1.aac.m3u"},
  {0, "RadioPlus",  "http://waw01-01.ic.smcdn.pl:8000/4260-1.aac.m3u"},
  {0, "WaWa",  "http://waw01-01.ic.smcdn.pl:8000/1260-1.aac.m3u"},  
  //{0, "Jazz24",  "https://prod-104-198-103-195.wostreaming.net/ppm-jazz24aac-ibc1?session-id=1b8d5b17b54e51f8c94b25beb59bc109"},
  {0, "Jazz24",  "https://live.wostreaming.net/direct/ppm-jazz24aac-ibc1"},
  {0, "Jazz1FM",  "http://sc-sjazz.1.fm:9010"},
  {0, "Jazz01",  "http://0n-jazz.radionetz.de/0n-jazz.aac"}
  //WDCB https://wdcb-ice.streamguys1.com/wdcb128
};

struct sQQQ  {
  int8_t l;  
  int8_t m;
  int8_t h;
};


sQQQ qqq[12]={
     { 3,-3, 3},
   { 6,-9, 4},
    { 4,-6, 4},
    { 3, 0, 3},
    { 0, 0, 0},
    {-3, 0,-3},
    {-3, 3,-3},
    {-3, 6,-6},
    { 6,-12, 6},
    { 6,-12, 3},
    { 3,-12, 6},
    { 0,-12, 6}

};



String strSplit(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
