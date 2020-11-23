#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX, TX
const byte led = 13,
input =0;

void setup() {
  Serial.begin(9600);
  Serial.println("Booted!");
  mySerial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite (led, false);
  

}

void loop() {
  if(mySerial.available() > 0){
    String input = mySerial.readString();
    Serial.print(input);
    digitalWrite (led, true);
    
  };

}
