struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

unsigned long millisBUTTON = millis();
unsigned long plusBUTTON   = 0;

#define LED_BUILTIN   22
#define pinKEY1    23
#define pinKEY2    18
#define pinKEY3    19
#define pinKEY4    5

Button button1 = {pinKEY1, 0, false};
Button button2 = {pinKEY2, 0, false};
Button button3 = {pinKEY3, 0, false};
Button button4 = {pinKEY4, 0, false};

void IRAM_ATTR isr1() {button1.numberKeyPresses += 1;button1.pressed = true;}
void IRAM_ATTR isr2() {button2.numberKeyPresses += 1;button2.pressed = true;}
void IRAM_ATTR isr3() {button3.numberKeyPresses += 1;button3.pressed = true;}
void IRAM_ATTR isr4() {button4.numberKeyPresses += 1;button4.pressed = true;}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);  
  pinMode(button1.PIN, INPUT_PULLUP);
  pinMode(button2.PIN, INPUT_PULLUP);
  pinMode(button3.PIN, INPUT_PULLUP);
  pinMode(button4.PIN, INPUT_PULLUP);
      //LOW, HIGH, CHANGE, FALLING, RISING
  attachInterrupt(button1.PIN, isr1, RISING);
  attachInterrupt(button2.PIN, isr2, RISING);
  attachInterrupt(button3.PIN, isr3, RISING);
  attachInterrupt(button4.PIN, isr4, RISING);
}

void loop_BUTTONS() {
  byte bitState = 0;
  if (button1.pressed) {bitSet(bitState, 0); button1.pressed = false;}
  if (button2.pressed) {bitSet(bitState, 1); button2.pressed = false;}
  if (button3.pressed) {bitSet(bitState, 2); button3.pressed = false;}
  if (button4.pressed) {bitSet(bitState, 3); button4.pressed = false;}
  
  if (bitState != 0) {
     digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.println(bitState);
    plusBUTTON = 500;
    if (bitState == 1) {}
    if (bitState == 2) {}
    if (bitState == 4) {}
    if (bitState == 8) {}
    if (bitState == 10) {}

  }
}

void loop() {

    //audio_loop();
  
    if (millis() - millisBUTTON > 200 + plusBUTTON) {
      loop_BUTTONS();
      millisBUTTON = millis();
      plusBUTTON = 0;
    }

}
