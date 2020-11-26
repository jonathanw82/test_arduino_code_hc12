#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "DHT.h"
#include <SoftwareSerial.h>
#include <arduinoFFT.h>;

//--------------- Declare Software serial pins -----------------------

SoftwareSerial mySerial(2, 3); //RX, TX

//-------------- Declare the temp humid sensor -----------------------

#define DHTPIN 8                          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11                     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//-------------- Declare the Battery voltage sensor ------------------

int batteryVoltageValue;
float Vout = 0.00;
float Vin = 0.00;
float R1 = 100000.00; // resistance of R1 (100K)
float R2 = 10000.00; // resistance of R2 (10K)

//-------------- Declare sound frequency varaiables -------------------

#define SAMPLES 128                // samples-pt FFT Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 1000    // ts = base on Nyquist, must be 2 times the highest expected frequency. 
arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];              // create vector of size of samles to hold real values
double vImag[SAMPLES];              // create vector of size of samles to hold imaginary values
double frequency;

//-------------- Declare sleep wake varaibales ------------------------

volatile char sleepCnt = 8;  // makes the arduino sleep for ex amount of seconds 8 max.
char sleepDelay = 0;  // makes the arduino sleep for ex amount of seconds 8 max.
bool wake = false;    // Initialy set wake false

//---------------- Millis statment declarations ------------------------

unsigned long currentTime;
unsigned long previousTime = 0;

//--------------------- General variables ------------------------------

int connectionCount = 0;    
char serialGo;              // read the serial port if the correct caracter is recieved start sending data.
bool sendData = false;      // Failsafe to stop data being sent at the wrong time.

void setup() {
  dht.begin();
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(batteryVoltageValue, INPUT);
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY));  // Period in microseconds
}

void loop() {
  
if(sleepDelay < 1 && wake == false){         // Sleep delay is to extend the sleep period past the usual 8 seconds. 
  sleepDelay++;
  Sleeping();
 }
 else{
  sleepDelay = 0;                             // Reset the sleep delay to zero
  wake = true;                                // make wake true
  currentTime = millis();
  soundFreq();                                // Read the sound frequency
  battvoltage();                              // Read the Battery voltage
  float h = dht.readHumidity();               // Read Humidity
  float t = dht.readTemperature();            // Read temperature as Celsius
  establishContact();                         // on Wake Establish a connection by sending capital A when the recerver gets the A it sends B.
  serialGo = mySerial.read();

  if (sendData == true) {
    while (mySerial.available() >= 0)
    {
      Serial.println("I am sending data to the reciever");
      for (int i = 0; i < 2; i++) {
        Serial.println("Sending Data to reciever");
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
        delay(1000);
      }
      sendData = false;
      wake = false;
      break;
     }
      Sleeping();
  }
 }
}
