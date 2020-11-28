#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "DHT.h"
#include <SoftwareSerial.h>
#include <arduinoFFT.h>;

//--------------- Declare Software serial pins -----------------------

SoftwareSerial mySerial(2, 3);      //RX, TX

//-------------- Declare the temp humid sensor ---------------------

#define DHTPIN 8                    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11               // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float t;
float h;

//-------------- Declare the Battery voltage sensor -----------------

int batteryVoltageValue;
float Vout = 0.00;
float Vin = 0.00;
float R1 = 100000.00;               // resistance of R1 (10K)
float R2 = 10000.00;                // resistance of R2 (1K)

//-------------- Declare sound frequency variables ------------------

#define SAMPLES 128                // samples-pt FFT Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 1000    // ts = base on Nyquist, must be 2 times the highest expected frequency. 
arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];              // create vector of size of samles to hold real values
double vImag[SAMPLES];              // create vector of size of samles to hold imaginary values
double frequency;

//-------------- Declare Sleep Wake variables ------------------------

volatile char sleepCnt = 2;         // makes the arduino sleep for ex amount of seconds 8 max before the code checks the sleep delay.
volatile char sleepDelay = 0;       // The current sleep delay.
int sleepDelayCycle = 2;            // How maney times the processor is to go back to sleep before running the rest of the code.
bool wake = false;                  // Initialy set wake false.

//---------------- Millis statment declarations ----------------------

unsigned long currentTime;
unsigned long previousTime = 0;
int sendAdelay = 300;               // Sets the amount of time between each A is sent. 

//--------------------- General variables ----------------------------

int connectionCount = 0;
char serialGo;                      // read the serial port if the correct caracter is recieved start sending data.
bool sendData = false;              // Failsafe to stop data being sent at the wrong time.
const int statusLED = 7;            // output pin declaration for each of the LEDs.
const int ledRed = 6;
const int ledBlue = 5;
const int powerPin = 10;            // output pin declaration for the transistor that supplies power to the peripheral sensors.     

void setup() {
  dht.begin();                      // Start the Humidity and temp sensor.
  Serial.begin(9600);               // Start the Serial ports at a baud rate of 9600.
  mySerial.begin(9600);
  pinMode(powerPin, OUTPUT);        // Set the power pin to an output.
  digitalWrite(powerPin, LOW);      // Set the pin Low.
  pinMode(statusLED, OUTPUT);       // Set the led pins to an outputs.
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(batteryVoltageValue, INPUT); // Set the Volate detection pin to an input.
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY)); // Period in microseconds
}

void loop() {
  serialGo = mySerial.read();
  currentTime = millis();

  if (sleepDelay <= sleepDelayCycle && wake == false) {       // Sleep delay is to extend the sleep period past the usual 8 seconds.
    sleepDelay++;
    Sleeping();
  }
  else if (sleepDelay > sleepDelayCycle  && wake == false) {
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
      currentTime = millis();
      if (currentTime - previousTime >= sendAdelay) {
        previousTime = currentTime;
        mySerial.print('A');   // send a capital A
        Serial.print("A");
        digitalWrite(ledRed, HIGH);
        connectionCount ++;
      }
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
