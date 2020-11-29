void led() {
  digitalWrite(statusLED, HIGH);        
  delay(200);
  digitalWrite(statusLED, LOW);
  delay(10);
  digitalWrite(statusLED, HIGH);
  delay(200);
  digitalWrite(statusLED, LOW);
}

/* 
  I would not usually use delay() however on this occasion we need the led to blink 
  before the sleep cycle is reenabled, if we used a millis() statement the ATmega will 
  be asleep before the led has had time to flash.
*/
