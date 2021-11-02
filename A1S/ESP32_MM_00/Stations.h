#define last_stations  7+4  //17
#define max_stations  last_stations+1

struct stationsSTRUCT  {
  int  const  ampli;  
  char const *alfa;
  char const *info;
  char const *stream;
  
};

stationsSTRUCT stacje [last_stations+1]={
  {0, "TF", "TOK-FM",    "http://pl-play.adtonos.com/tok-fm"},
  {0, "NS", "NowySwiat", "http://stream.rcs.revma.com/ypqt40u0x1zuv"},
  {0, "35", "357",       "http://stream.rcs.revma.com/ye5kghkgcm0uv"},
  {1, "SJ", "Sws-Jaz",  "http://stream.srg-ssr.ch/m/rsj/aacp_96"},
  {1, "SC", "Sws-Cla",  "http://stream.srg-ssr.ch/m/rsc_de/aacp_96"},
  {0, "SP", "Sws-Pop",  "http://stream.srg-ssr.ch/m/rsp/aacp_96"},
  {1, "CF", "ClassFM",   "http://media-the.musicradio.com/ClassicFM"},
  {1, "1j", "101_Jazz",  "https://101smoothjazz.cdnstream1.com/b22139_128mp3"},
  {0, "RS", "RockServ",  "http://stream.open.fm/379"},
  {0, "No", "NewOnce",   "http://stream.open.fm/374"},
  {0, "NP", "Niezapom",  "http://195.150.20.242:8000/rmf_niezapomniane_melodie"},
  {0, "SZ", "Szczecin",  "http://stream4.nadaje.com:11986/prs.aac"}
};

struct sQQQ  {
  int8_t l;  
  int8_t m;
  int8_t h;
};


sQQQ qqq[5]={
    { 4, -4, 4},
    { 6, -6, 6},
    { 6,-12, 6},
    { 6,-18, 6},
    { 0,2,0}
};
