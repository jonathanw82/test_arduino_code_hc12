#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "DHT.h"
#include <SoftwareSerial.h>
#include <arduinoFFT.h>;

//-------------- Declare Software serial pins ------------------------

SoftwareSerial mySerial(2, 3);            //RX, TX.

//-------------- Declare the temp humid sensor -----------------------

#define DHTPIN 8                          // Digital pin connected to the DHT sensor.
#define DHTTYPE DHT11                     // DHT 11.
DHT dht(DHTPIN, DHTTYPE);
float t;
float h;

//-------------- Declare the Battery voltage sensor ------------------

const int voltPin = A0;
int batteryVoltageValue;
float Vout = 0.00;
float Vin = 0.00;
float R1 = 100000.00;                     // Resistance of R1 (10K).
float R2 = 10000.00;                      // Resistance of R2 (1K).

//-------------- Declare sound frequency varaiables -------------------

#define SAMPLES 128                       // Samples-pt FFT Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 1000           // ts = base on Nyquist, must be 2 times the highest expected frequency. 
arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];                    // Create vector of size of samles to hold real values.
double vImag[SAMPLES];                    // Create vector of size of samles to hold imaginary values.
double frequency;
const int microphone = A2; 

//-------------- Declare sleep wake varaibales ------------------------

volatile char sleepCnt = 2;               // Makes the arduino sleep for ex amount of seconds 8 max.
volatile char sleepDelay = 0;             // Makes the arduino sleep for ex amount of seconds 8 max.
bool wake = false;                        // Initialy set wake false

//---------------- Millis statment declarations ------------------------

unsigned long currentTime;
unsigned long previousTime = 0;

//--------------------- General variables ------------------------------

int connectionCount = 0;
char serialGo;                            // Read the serial port if the correct caracter is recieved start sending data.
bool sendData = false;                    // Failsafe to stop data being sent at the wrong time.
const int statusLED = 7;                  // Output pin declaration for each of the LEDs.
const int ledRed = 6;
const int ledBlue = 5;
const int powerPin = 10;                  // Output pin declaration for the transistor that supplies power to the peripheral sensors.   

void setup() {
  dht.begin();                            // Start the Humidity and temp sensor.
  Serial.begin(9600);                     // Start the Serial ports at a baud rate of 9600.
  mySerial.begin(9600);
  pinMode(powerPin, OUTPUT);              // Set the power pin to an output.
  digitalWrite(powerPin, LOW);            // Set the pin Low.
  pinMode(statusLED, OUTPUT);             // Set the led pins to an outputs.
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(batteryVoltageValue, INPUT);    // Set the Volate detection pin to an input.
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY)); // Period in microseconds.
}

void loop() {
  serialGo = mySerial.read();
  currentTime = millis();

  if (sleepDelay <= 2 && wake == false) {       // Sleep delay is to extend the sleep period past the usual 8 seconds.
    sleepDelay++;
    Sleeping();
  }
  else if (sleepDelay > 2  && wake == false) {
    digitalWrite(powerPin, HIGH);               // Set the power pin High to send power from the transistor that powers the sensors and turn them on.
    wake = true;                                // Make wake true.
    sleepDelay = 0;                             // Reset the sleep delay to zero.
    soundFreq();                                // Read the sound frequency.
    battvoltage();                              // Read the Battery voltage.
    h = dht.readHumidity();                     // Read Humidity.
    t = dht.readTemperature();                  // Read temperature as Celsius.
  }
  if (wake == true) {                           // On Wake Establish a connection by sending capital A when the recerver gets the A it sends B.
    while (mySerial.available() <= 0) {
        mySerial.print('A');                    // Send a capital A.
        Serial.print(F("A"));
        digitalWrite(ledRed, HIGH);             // Turn on the red led to show it is sending A.
        delay(300);                             // Wait 300 milliseconds
        connectionCount ++;                     // Increase the connection coint by 1 every time through the loop.
      if (connectionCount > 30) {               // If the connection couint is over 30 run this code.
        connectionCount = 0;                    // Set the connection count back to zero.
        digitalWrite(ledRed, LOW);              // Turn off the red led.
        wake = false;                           // Set wake false so this code block can no longer run.                  
        digitalWrite(powerPin, LOW);            // Set the power pin low to cut the power from the transistor that powers the sensors and turn them off to save battery power.
        Sleeping();                             // Send everthing to sleep and start all over again.
        break;                                  // Break out of the while loop without this break the sleep interupt gets confused.
      }
      else if (serialGo == 'B') {               // When we recieve a B we run this code.
        sendData = true;                        // Send data variable to true.
        wake = false;                           // Set wake false so this code block can no longer run.
        connectionCount = 0;                    // Set the connection count back to zero.
        digitalWrite(ledRed, LOW);              // Turn off the red led.
        Serial.print("I have received a B");    // Confirm we have recieved a B.
        break;                                  // Break out of the while loop and continue on.
      }
    }
  }

  if (sendData == true) {                       // If the wake variable is true run this code.
    while (mySerial.available() >= 1)
    {
      for (int i = 0; i < 2; i++) {             // Send the data twice.
        digitalWrite(ledBlue, HIGH);            // Turn on the blue LED.
        Serial.println("Sending Data to reciever"); // Send the relevent data. 
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
        delay(1000);                              // Delay the loop for 1 second slow so give it time to react.
      }
      sendData = false;                           // Set the sendData variable false so no more data can be sent.
      digitalWrite(ledBlue, LOW);                 // Turn the blue led off.
      wake = false;                               // Set the wake variable false.
      digitalWrite(powerPin, LOW);                // Set the power pin low to cut the power from the transistor that powers the sensors and turn them off to save battery power.
      break;                                      // Brteak out of the while loop.
    }
    Sleeping();                                   // Send everthing to sleep and start all over again.
  }
}
