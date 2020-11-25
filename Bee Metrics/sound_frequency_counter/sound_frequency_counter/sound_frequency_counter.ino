https://www.youtube.com/watch?v=wbeV0J30LGQ

#include <arduinoFFT.h>;

#define SAMPLES 128                // samples-pt FFT Must be a base 2 number. Max 128 for Arduino Uno.
#define SAMPLING_FREQUENCY 1000    // ts = base on Nyquist, must be 2 times the highest expected frequency. 

arduinoFFT FFT = arduinoFFT();

unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES];              // create vector of size of samles to hold real values
double vImag[SAMPLES];              // create vector of size of samles to hold imaginary values

void setup() {
  Serial.begin(9600);
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY));  // Period in microseconds
}

void loop() {

      // sample SAMPLES times
    for(int i = 0; i < SAMPLES; i++){
        microSeconds = micros();   // Returns the number of microseconds since the arduino board began running.

        vReal[i] = analogRead(A2); // Reads the value from A2 pin and quantize it and save it as a real term.
        vImag[i] = 0;              // Makes Imaginary term 0 always.

        // remaini9ng wait time between samples if necessary
        while(micros() < (microSeconds + samplingPeriod))
        {
          // do nothing
          }
      }
      // Perform FFT on Samples
      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

      // Find peak frequency and print peak
      double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
      Serial.print(peak);         // print out most dominant frequency
      Serial.println("Hz");
       
      while(1);

      
}
