void Sleeping() {

  static byte prevADCSRA = ADCSRA;            // Disable the Analog to digital converter, pins A0 to A5.
  ADCSRA = 0;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);        // Set the type of sleep to full power down.
  sleep_enable();                             // Enable the sleep cycle.

  while (sleepCnt < 2) {
    sleep_bod_disable();                      // Turn of Brown Out Detection (low voltage). This is automatically re-enabled upon timer interrupt.
    noInterrupts();                           // Ensure we can wake up again by first disabling interrupts temporarily.
    
    // clear various "reset" flags
    MCUSR = 0;                                // allow changes, disable reset
    WDTCSR = bit (WDCE) | bit(WDE);           // set interrupt mode and an interval
    WDTCSR = bit (WDIE) | bit(WDP3) | bit(WDP1) | bit(WDP0);    // set WDIE, and 1 second delay
    wdt_reset();                              // Reset the watch dog timer.
    led();                                    // Flash the status led.
    Serial.println("Good night!");            // Send a message just to show we are about to sleep.
    Serial.flush();                           // Empty the serial output buffer.
    interrupts();                             // Allow interrupts now.
    sleep_cpu();                              // Enter sleep mode as set above.
  }

  // ---------------------------------------------------------
  //  Controller is now asleep until woken up by an interrupt
  // ---------------------------------------------------------

  sleep_disable();                            // Prevent sleep mode, so we don't enter it again, except deliberately, by code.
  Serial.println("I'm awake!");               // Wakes up at this point when timer wakes up and tells us its awake.
  sleepCnt = 0;                               // Reset sleep counter.
  ADCSRA = prevADCSRA;                        // Re-enable Analog to digital converter.
}

ISR (WDT_vect) {                              // When WatchDog timer causes the Controller to wake it comes here
  wdt_disable();                              // Turn off watchdog, we don't want it to do anything at this time.
  sleepCnt++;                                 // Increment the WDT interrupt count.
}
