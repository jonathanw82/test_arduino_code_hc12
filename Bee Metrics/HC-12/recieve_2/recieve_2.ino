
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX, TX

const byte numChars = 90;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char DataA[numChars] = {0};
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

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(DataA, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    humidity = atof(strtokIndx);     // cconvert this part to a float

    strtokIndx = strtok(NULL, ",");
    temperature = atof(strtokIndx);     // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    sound = atof(strtokIndx);     // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    batteryTransmitter = atof(strtokIndx);     // convert this part to a float

}

void showParsedData() {
//    Serial.println(receivedChars);
//    Serial.println(DataB);
    Serial.println(DataA);
    Serial.print("Humid");
    Serial.print(" ");
    Serial.print(humidity);
    Serial.print("%  ");
    Serial.print("Temp");
    Serial.print(" ");
    Serial.print(temperature);
    Serial.print("°C  ");
    Serial.print("Sound Level");
    Serial.print(" ");
    Serial.print(sound);
    Serial.print(" Dbspl ");
    Serial.print("Battery Level");
    Serial.print(" ");
    Serial.print(batteryTransmitter);
    Serial.println(" volts");
}
