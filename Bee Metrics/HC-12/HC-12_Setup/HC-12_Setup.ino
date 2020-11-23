#include <SoftwareSerial.h>;
// Type AT+RX to see all current settings.
#define RX 2 // connect to the TX pin of the HC-12 
#define TX 3 // connect to the RX pin of the HC-12

SoftwareSerial mySerial(RX, TX);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

}

void loop() {
  if (mySerial.available()){
    Serial.write(mySerial.read());
    }
  if (Serial.available()){
    mySerial.write(Serial.read());
    }
}
