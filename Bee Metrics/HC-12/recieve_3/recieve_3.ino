
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX, TX

const byte numChars = 90;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
char DataA[numChars] = {0};
char DataB[numChars] = {0};
char DataC[numChars] = {0};
char DataD[numChars] = {0};
float humidity = 0.0;
float temperature = 0.0;
float sound = 0.0;
float batteryTransmitter = 0.00;


boolean newData = false;

void setup() {
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
  mySerial.begin(9600);
}

void loop() {

  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    // because strtok() used in parseData() replaces the commas with \0
    parseData();
    showParsedData();
    newData = false;
  }
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (mySerial.available() > 0 && newData == false) {
    rc = mySerial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}


void showParsedData() {
  //    Serial.println(receivedChars);
  //    Serial.println(DataB);
  Serial.print(DataA);
  Serial.print(" ");
  Serial.print(humidity);
  Serial.print("%  ");
  Serial.print(DataB);
  Serial.print(" ");
  Serial.print(temperature);
  Serial.print("°C  ");
  Serial.print(DataC);
  Serial.print(" ");
  Serial.print(sound);
  Serial.print("Hz ");
  Serial.print(DataD);
  Serial.print(" ");
  Serial.print(batteryTransmitter);
  Serial.println(" volts");
}
