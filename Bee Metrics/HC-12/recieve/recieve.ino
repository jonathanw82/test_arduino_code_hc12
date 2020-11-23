void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Booted!");

}

void loop() {
  if(Serial.available() > 0){
    Serial.println("Got Serial");
    String input = Serial.readString();
    if(input == "Hello World"){
      Serial.println("Recieved");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      }
    }
}
