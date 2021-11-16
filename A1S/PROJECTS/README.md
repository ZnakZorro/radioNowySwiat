<h1>Projects ESP32</h1>



<h3>C:\Users\User\Documents\Arduino\libraries\Credentials\credentials.h</h3>


<pre>
String openWeatherMapApiKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxx";

struct credentialSTRUCT{
    char const *ssid;
    char const *pass;
};

credentialSTRUCT credential [1]={
    {"ssid","pass"},
    {"xxxx","yyyy"},
    {"aaaa","bbbb"}
}

const char* ssid   = credential[0].ssid;
const char* pass   = credential[0].pass;

</pre>
