void led() {
  digitalWrite(statusLED, LOW);        // I would not usually use delay() however i need the led to blink before the sleep cycle is reenabled
  delay(200);
  digitalWrite(statusLED, HIGH);
  delay(10);
  digitalWrite(statusLED, LOW);
  delay(200);
  digitalWrite(statusLED, HIGH);
}
