# radioNowySwiat
radio Nowy Świat on ESP32

ESP32 Audio Kit V2.2 2957
<pre>
BUTTON    GPIO
* TP_VOL  17
* TP_SET   9
* TP_PLAY  8
* BUT_REC  0
* BUT_MODE 3
</pre>

<pre>
//Switch org : 10100   //
//Switch S1  : 01100   //1-OFF, 2-ON, 3-ON, 4-OFF, 5-OFF
//sudo chmod a+rw /dev/ttyUSB0

#include "ESPAsyncWebServer.h"    //https://github.com/me-no-dev/ESPAsyncWebServer
#include "ES8388.h"               // https://github.com/maditnerd/es8388
#include "Audio.h"                // https://github.com/schreibfaul1/ESP32-audioI2S
</pre>
