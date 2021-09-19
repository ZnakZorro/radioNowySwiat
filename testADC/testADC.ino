void setup() {
 Serial.begin(115200);
    Serial.println("Start...");
}


//19=led
//22=led
void loop() {
  //int arr [] = {32,33, 36,13,19,5,4 };
  int arr [] = {32,33, 36,13,18,19,5,4,21,22,23,24,25,26,27,28,29,30 };
 

 Serial.print("...");
 
    for (int i = 0; i < 17; i++){
      int a=digitalRead(arr[i]);
      Serial.print(arr[i]);
      Serial.print("=");
      Serial.print(a);
      Serial.print(", ");
    }
    delay(333);       
    Serial.println(" ");
}
