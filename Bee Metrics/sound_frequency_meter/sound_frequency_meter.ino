#include <fix_fft.h>

int i = 0, val;  // counters


void setup() {
  Serial.begin(9600);
  analogReference(DEFAULT);   // Use default (5v) aref voltage.

}

void loop() {
  int min=1024, max = 0;      // Set min and max ADC values
  for(i = 0; i < 128; i++){   // take 128 samples
    val = analogRead(A2);
    data[i] = val / 4 - 128;  // each element of the array is val/4-128
    im[i] = 0;
    if(val>max) max=val;      // capture max level
    if(val<min) min=val;      // capture min level
    };

}


// https://www.researchgate.net/publication/229224190_Monitoring_of_swarming_sounds_in_bee_hives_for_early_detection_of_the_swarming_period
