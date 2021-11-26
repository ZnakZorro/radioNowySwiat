struct stationsSTRUCT  {
  int  const  ampli;  
  char const *info;
  char const *stream;
};

stationsSTRUCT stacje [5]={
  {0, "TOK-FM",    "http://pl-play.adtonos.com/tok-fm"},
  {0, "NowySwiat", "http://stream.rcs.revma.com/ypqt40u0x1zuv"},
  {0, "357",       "http://stream.rcs.revma.com/ye5kghkgcm0uv"},
  {1, "Sws-Jzz",   "http://stream.srg-ssr.ch/m/rsj/aacp_96"},
  {1, "Sws-Cla",   "http://stream.srg-ssr.ch/m/rsc_de/aacp_96"}
};

/*
String make_str(String str){
    if (str.length()>7) return str;
    String plus="";
    for(int i = 0; i < (7 - str.length()); i++)
        plus += ' ';  
    return plus+str;
}
*/
/*
String getSecondLine(String str){
    if (str.length()<17) return "";
    else return str.substring(16, str.length());
}*/
