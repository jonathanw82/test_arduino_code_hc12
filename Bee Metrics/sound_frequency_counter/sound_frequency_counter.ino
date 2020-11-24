/*

  Simple Frequency Meter for Arduino Zero

  Demonstrates how to sample an input signal and get back its frequency

  This example code is in the public domain

  http://arduino.cc/en/Tutorial/SimpleAudioFrequencyMeter

  created by Arturo Guadalupi <a.guadalupi@arduino.cc>

  10 Nov 2015

*/

#include <AudioFrequencyMeter.h>

AudioFrequencyMeter meter;

void setup() {

  Serial.begin(9600);

  Serial.println("started");

  meter.setBandwidth(60.00, 1500);    // Ignore frequency out of this range

  meter.begin(A2, 45000);             // Intialize A0 at sample rate of 45kHz
}

void loop() {

  float frequency = meter.getFrequency();

  if (frequency > 0)

  {

    Serial.print(frequency);

    Serial.println(" Hz");

  }
}
