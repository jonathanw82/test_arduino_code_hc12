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
float t;
float h;

//-------------- Declare the Battery voltage sensor ------------------

int batteryVoltageValue;
float Vout = 0.00;
float Vin = 0.00;
float R1 = 100000.00; // resistance of R1 (10K)
float R2 = 10000.00; // resistance of R2 (1K)

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
volatile char sleepDelay = 0;  // makes the arduino sleep for ex amount of seconds 8 max.
bool wake = false;    // Initialy set wake false

//---------------- Millis statment declarations ------------------------

unsigned long currentTime;
unsigned long previousTime = 0;

//--------------------- General variables ------------------------------

int connectionCount = 0;
char serialGo;              // read the serial port if the correct caracter is recieved start sending data.
bool sendData = false;      // Failsafe to stop data being sent at the wrong time.
const int statusLED = 7;    // output pin for the LED
const int ledRed = 6;
const int ledBlue = 5;
const int powerPin = 10;

void setup() {
  dht.begin();
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);
  pinMode(statusLED, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(batteryVoltageValue, INPUT);
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY)); // Period in microseconds
}

void loop() {
  serialGo = mySerial.read();
  currentTime = millis();

  if (sleepDelay <= 8 && wake == false) {       // Sleep delay is to extend the sleep period past the usual 8 seconds.
    sleepDelay++;
    Sleeping();
  }
  else if (sleepDelay > 8  && wake == false) {
    digitalWrite(powerPin, HIGH);
    wake = true;                                // make wake true
    sleepDelay = 0;                             // Reset the sleep delay to zero
    soundFreq();                                // Read the sound frequency
    battvoltage();                              // Read the Battery voltage
    h = dht.readHumidity();               // Read Humidity
    t = dht.readTemperature();            // Read temperature as Celsius
  }
  if (wake == true) {                          // on Wake Establish a connection by sending capital A when the recerver gets the A it sends B.
    while (mySerial.available() <= 0) {
      mySerial.print('A');   // send a capital A
      Serial.print("A");
      delay(300);
      digitalWrite(ledRed, HIGH);
      connectionCount ++;
      if (connectionCount > 30) {
        connectionCount = 0;
        digitalWrite(ledRed, LOW);
        wake = false;
        sleepDelay = 0;
        digitalWrite(powerPin, LOW);
        Sleeping();
        break;
      }
      else if (serialGo == 'B') {
        sendData = true;
        wake = false;
        connectionCount = 0;
        digitalWrite(ledRed, LOW);
        Serial.print("I have received a B");
        break;
      }
    }
  }

  if (sendData == true) {
    while (mySerial.available() >= 1)
    {
      for (int i = 0; i < 2; i++) {
        digitalWrite(ledBlue, HIGH);
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
      digitalWrite(ledBlue, LOW);
      wake = false;
      digitalWrite(powerPin, LOW);
      break;
    }
    Sleeping();
  }
}
