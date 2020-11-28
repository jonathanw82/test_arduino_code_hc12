void soundFreq(){
  
  // sample SAMPLES times
    for(int i = 0; i < SAMPLES; i++){
        microSeconds = micros();   // Returns the number of microseconds since the arduino board began running.

        vReal[i] = analogRead(A3); // Reads the value from A3 pin and quantize it and save it as a real term.
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
      frequency = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
  }
