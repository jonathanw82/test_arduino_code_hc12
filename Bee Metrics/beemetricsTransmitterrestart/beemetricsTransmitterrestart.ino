#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>


//--------------- Declare Software serial pins -----------------------

SoftwareSerial mySerial(2, 3); //RX, TX

//-------------- Declare sleep wake varaibales ------------------------

volatile char sleepCnt = 4;  // makes the arduino sleep for ex amount of seconds 8 max.
char sleepDelay = 0;  // makes the arduino sleep for ex amount of seconds 8 max.
bool wake = false;    // Initialy set wake false

//---------------- Millis statment declarations ------------------------

unsigned long currentTime;
unsigned long previousTime = 0;

//--------------------- General variables ------------------------------

int connectionCount = 0;
char serialGo;              // read the serial port if the correct caracter is recieved start sending data.
bool sendData = false;      // Failsafe to stop data being sent at the wrong time.
const int statusLED = 7;    // output pin for the LED

void setup() {

  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(statusLED, OUTPUT);
}

void loop() {
  currentTime = millis();

  if (sleepDelay <= 2 && wake == false) {
    sleepDelay ++;
    Sleeping();
  }
  else if (sleepDelay > 2 && wake == false) {
    wake = true;
    sleepDelay = 0;                             // Reset the sleep delay to zero
    serialGo = mySerial.read();
  }
    if (wake == true) {
      while (mySerial.available() <= 0) {
        mySerial.println('A');   // send a capital A
        Serial.print("A");
        delay(300);
        connectionCount ++;
        if (connectionCount > 30) {
          connectionCount = 0;
          wake = false;
          sleepDelay = 0; 
          Serial.print(wake);
          Sleeping();
          break;
        }
        else if (serialGo == 'B') {
          sendData = true;
          wake = false;
          connectionCount = 0;
          Serial.print("I have received a B");
          break;
        }
      }
    }
  
  if (sendData == true) {
    Serial.print("I made it to send data");
    delay(1000);
    Sleeping();
  }
}

void Sleeping() {

  // Disable the ADC (Analog to digital converter, pins A0 [14] to A5 [19])
  static byte prevADCSRA = ADCSRA;
  ADCSRA = 0;

  /* Set the type of sleep mode we want. Can be one of (in order of power saving):

     SLEEP_MODE_IDLE (Timer 0 will wake up every millisecond to keep millis running)
     SLEEP_MODE_ADC
     SLEEP_MODE_PWR_SAVE (TIMER 2 keeps running)
     SLEEP_MODE_EXT_STANDBY
     SLEEP_MODE_STANDBY (Oscillator keeps running, makes for faster wake-up)
     SLEEP_MODE_PWR_DOWN (Deep sleep)
  */
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

  while (sleepCnt < 4) {
    // Turn of Brown Out Detection (low voltage). This is automatically re-enabled upon timer interrupt
    sleep_bod_disable();

    // Ensure we can wake up again by first disabling interrupts (temporarily) so
    // the wakeISR does not run before we are asleep and then prevent interrupts,
    // and then defining the ISR (Interrupt Service Routine) to run when poked awake by the timer
    noInterrupts();

    // clear various "reset" flags
    MCUSR = 0;  // allow changes, disable reset
    WDTCSR = bit (WDCE) | bit(WDE); // set interrupt mode and an interval
    WDTCSR = bit (WDIE) | bit(WDP3) | bit(WDP1) | bit(WDP0);    // set WDIE, and 1 second delay
    wdt_reset();

    // Send a message just to show we are about to sleep
    led();
    Serial.println("Good night!");
    Serial.flush();

    // Allow interrupts now
    interrupts();

    // And enter sleep mode as set above
    sleep_cpu();
  }

  // --------------------------------------------------------
  // �Controller is now asleep until woken up by an interrupt
  // --------------------------------------------------------

  // Prevent sleep mode, so we don't enter it again, except deliberately, by code
  sleep_disable();

  // Wakes up at this point when timer wakes up �C
    Serial.println("I'm awake!");

  // Reset sleep counter
  sleepCnt = 0;

  // Re-enable ADC if it was previously running
  ADCSRA = prevADCSRA;
  }

  // When WatchDog timer causes �C to wake it comes here
  ISR (WDT_vect) {

  // Turn off watchdog, we don't want it to do anything (like resetting this sketch)
  wdt_disable();

  // Increment the WDT interrupt count
  sleepCnt++;

  // Now we continue running the main Loop() just after we went to sleep
}
