#include "DHT.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX, TX
#define DHTPIN 8        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int hum;
int temp;
int batteryVoltageValue;

float Vout = 0.00;
float Vin = 0.00;
float R1 = 100000.00; // resistance of R1 (100K)
float R2 = 10000.00; // resistance of R2 (10K)
int inByte = 0;

void setup() {
  dht.begin();
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(batteryVoltageValue, INPUT);
}

void loop() {

  battvoltage();
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  float frequency = 200.00;

  mySerial.print('<');
  mySerial.print("Humidity:");
  mySerial.print(',');
  mySerial.print("Temperature:");
  mySerial.print(',');
  mySerial.print("Sound Frequency:");
  mySerial.print(',');
  mySerial.print("Battery Voltage:");
  mySerial.print(',');
  mySerial.print(h);
  mySerial.print(',');
  mySerial.print(t);
  mySerial.print(',');
  mySerial.print(frequency);
  mySerial.print(',');
  mySerial.print(Vin);
  mySerial.print('>');

  delay(1000);//delay little for better serial communication
}

void battvoltage() {

  batteryVoltageValue = analogRead(A0); //read the A0 pin value
  Vout = (batteryVoltageValue * 5.00 / 1023.00); //convert the value to a true voltage.
  Vin = Vout / (R2 / (R1 + R2));
  if (Vin < 0.09)
  {
    Vin = 0.00;
  }
}
