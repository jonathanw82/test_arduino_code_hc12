void soundFreq() {
  for (int i = 0; i < SAMPLES; i++) {       // sample SAMPLES times.
    microSeconds = micros();                // Returns the number of microseconds since the Atmega chip began running.
    vReal[i] = analogRead(microphone);      // Reads the value from the microphone and quantize it and save it as a real term.
    vImag[i] = 0;                           // Makes Imaginary term 0 always.
    while (micros() < (microSeconds + samplingPeriod)) // remaining wait time between samples if necessary
    {
      // do nothing
    }
  }
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD); // Perform FFT on Samples
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  frequency = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);   // Find peak frequency and print peak
}
