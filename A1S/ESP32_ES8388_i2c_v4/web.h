
const char RADIO_SVG[] PROGMEM = R"=====(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512"><path fill="navy" d="M73.8 141.9c-15.2 6-25.8 21.8-25.8 39.5v256c0 23.5 18.5 42.7 41.6 42.7h332.8c23.1 0 41.6-19.2 41.6-42.7v-256c0-23.7-18.5-42.7-41.6-42.7H179l171.8-71.3L336.7 32 73.8 141.9zM160 438c-35.4 0-64-28.6-64-64s28.6-64 64-64 64 28.6 64 64-28.6 64-64 64zm256-171.3h-32v-46.2h-44.8v46.2H96v-85.3h320v85.3z"/></svg>
)=====";

const char TEREN_WEB_JSON[] PROGMEM = R"=====(
{
    "name": "radioTeren",
    "short_name": "radioTeren",
    "description": "radioTeren",
    "start_url": "/",
    "display": "standalone",
    "background_color": "#369",
    "theme_color": "#369",
    "icons": [ 
        {
            "src": "radio.svg",
            "sizes": "48x48 72x72 96x96 128x128 150x150 256x256 512x512 1024x1024",
            "type": "image/svg+xml",
            "purpose": "any"
        }
    
    ]
}
)=====";



/*

Szkic używa 1 200 758 bajtów (91%) pamięci programu. Maksimum to 1310720 bajtów.
Zmienne globalne używają 52 072 bajtów (15%) pamięci dynamicznej, pozostawiając 275608 bajtów dla zmiennych lokalnych. Maksimum to 327680 bajtów.




*/
