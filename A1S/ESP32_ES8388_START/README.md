Istalacja Arduino <br />
https://www.arduino.cc/en/software

Instalacja płytki dla ESP32<br />
-W plik->preferencje->Dodatkowe adresy URL dla menadżera płytek wklejasz:<br />
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json<br />
jest też na stronie: https://github.com/espressif/arduino-esp32<br />


-Narzędzia->Płytka->Menadżer płytek w pole szukaj wpisujesz ESP32 i wybierasz ESP32 wersja 1.0.5 rc6


Instalacja biblioteki I2Saudio<br />
Pobierasz zipa ze strony<br />
https://github.com/schreibfaul1/ESP32-audioI2S


Code->Download zip
Instalujesz bibliotekę ww Arduino:<br />
Szkic->Dołącz bibliotekę->Dodaj z zip-> wskazujesz zipa na dysku i OK<br />

W zależności od wersji płytki dla versji v2.2 2957<br />Instalujesz bibliotekę dla dla ES8388<br />
https://github.com/maditnerd/es8388

<br />
Zworki na płytce ustawiasz jako 01100
<br />

W zipie są przykłady
* examples/ESP32-ES8388/ESP32_ES8388.ino
