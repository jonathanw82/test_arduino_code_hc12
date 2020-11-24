

const int sampleWindow = 50;  // sample window width in ms (50ms = 20Hz)
unsigned int sample;
unsigned long startMillis = millis();   // Start of sample window


void setup() {
  Serial.begin(9600);
}


void loop() {
//------------------------ Sound Meter -------------------------------  
  float peakToPeak = 0;                         // peak-to-peak level
  unsigned int signalMin = 0;                   // min value
  unsigned int signalMax = 1024;                // max value
                                                // collect data for 50ms
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(A2);                     // get reading from mic
    if (sample < 1024)                           // get rid of random reading
    {
      if (sample > signalMax)
      {
        signalMax = sample;                      // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;                      // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMix;            // max - min = peak-peak amplitude
  float db = map(peakToPeak, 20, 900, 26.5, 90); // Calibrate for decibels ptp min max dbmin dbmax
  Serial.print("Sound Level = ");
  Serial.println(db);
  Serial.println(" db");
//------------------------ Sound End -----------------------------------
}

//https://www.youtube.com/watch?v=149TXc_bPeA
