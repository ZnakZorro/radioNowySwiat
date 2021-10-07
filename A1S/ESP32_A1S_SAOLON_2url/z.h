/*

Szkic używa 1 297 826 bajtów (41%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 53660 bajtów (16%) pamięci dynamicznej, pozostawiając 274020 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.

*ws*
Szkic używa 1 326 682 bajtów (42%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 53516 bajtów (16%) pamięci dynamicznej, pozostawiając 274164 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.

Szkic używa 1 327 422 bajtów (42%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 53540 bajtów (16%) pamięci dynamicznej, pozostawiając 274140 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.

add neoPixels
Szkic używa 1 339 770 bajtów (42%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 53644 bajtów (16%) pamięci dynamicznej, pozostawiając 274036 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.




Zmiany w audio.h
//void IIR_calculateCoefficients();
void setTone(float ab[5] );
const size_t m_buffSizePSRAM = 8*300000; // most webstreams limit the advance to 100...300Kbytes

Zmiay w audio.cpp
usuń:
void Audio::IIR_calculateCoefficients()

zablokuj:
bool Audio::setSampleRate(uint32_t sampRate) {
    i2s_set_sample_rates((i2s_port_t)m_i2s_num, sampRate);
    m_sampleRate = sampRate;
    //IIR_calculateCoefficients(); // must be recalculated after each samplerate change
    return true;
}


zmień:
void Audio::setTone(float ab[5] ){
      m_filter[LEFTCHANNEL].a0 = ab[0];
            m_filter[LEFTCHANNEL].a1 = ab[1];
            m_filter[LEFTCHANNEL].a2 = ab[2];
            m_filter[LEFTCHANNEL].b1 = ab[3];
            m_filter[LEFTCHANNEL].b2 = ab[4];
       
            m_filter[RIGHTCHANNEL].a0 = ab[0];
            m_filter[RIGHTCHANNEL].a1 = ab[1];
            m_filter[RIGHTCHANNEL].a2 = ab[2];
            m_filter[RIGHTCHANNEL].b1 = ab[3];
            m_filter[RIGHTCHANNEL].b2 = ab[4];
    int16_t tmp[2];
    IIR_filterChain(tmp, true ); // flush the filter       
}







                  //gpio_wakeup_enable(GPIO_NUM_18,GPIO_INTR_LOW_LEVEL);
                  //esp_sleep_enable_gpio_wakeup();
                  //esp_deep_sleep_start();
                  // 5, 18, 19, 23
                  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_5,0);
                  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_18,0);
                  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_19,0);
                  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_23,0);
                  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_5,1);
                  







 */



 

/* Example serial.printf:
 *   serial.printf(Serial, "Sensor %d is %o and reads %1f\n", d, d, f) will
 *   output "Sensor 65 is on and reads 123.5" to the serial port.
 * 
 * Formatting strings <fmt>
 * %B    - binary (d = 0b1000001)
 * %b    - binary (d = 1000001)  
 * %c    - character (s = H)
 * %d/%i - integer (d = 65)\
 * %f    - float (f = 123.45)
 * %3f   - float (f = 123.346) three decimal places specified by %3.
 * %o    - boolean on/off (d = On)
 * %s    - char* string (s = Hello)
 * %X    - hexidecimal (d = 0x41)
 * %x    - hexidecimal (d = 41)
 * %%    - escaped percent ("%")
 * Thanks goes to @alw1746 for his %.4f precision enhancement 
 */

 /*




*/


/* 
    //dwojka http://stream3.polskieradio.pl:8952/listen.pls

  //{0, "NowySwiat", "http://stream.rcs.revma.com/5ws0kfn1x1zuv.m3u"},  
  //{0, "357",       "http://stream.rcs.revma.com/ye5kghkgcm0uv.m3u"}, 
 //{0, "RMF_Class", "http://195.150.20.6/rmf_classic"},
  //{1, "101_Jazz",  "http://101smoothjazz.cdnstream1.com/b22139_128mp3"},

*/
 
/*
add neoPixels
Szkic używa 1 339 770 bajtów (42%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 53 644 bajtów (16%) pamięci dynamicznej, pozostawiając 274036 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.

Szkic używa 1307 990 bajtów (41%) pamięci programu. Maksimum to 3145728 bajtów.
Zmienne globalne używają 53 516 bajtów (16%) pamięci dynamicznej, pozostawiając 274164 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.


 */
