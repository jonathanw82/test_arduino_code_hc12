void battvoltage() {
  batteryVoltageValue = analogRead(A0); //read the A0 pin value
  Vout = (batteryVoltageValue * 5.00 / 1023.00); //convert the value to a true voltage.
  Vin = Vout / (R2 / (R1 + R2));
  if (Vin < 0.09)
  {
    Vin = 0.00;
  }
}
