void battvoltage() {
  
  batteryVoltageValue = analogRead(voltPin);     // Read the voltPin to get its value.
  Vout = (batteryVoltageValue * 5.00) / 1024.00; // Convert the value to a true voltage value.
  Vin = Vout / (R2 / (R1 + R2));                 
  if (Vin < 0.09)                                
  {
    Vin = 0.00;                                  
  }
}

/*
Formula for calculating voltage:
Vout = (batteryVoltageValue * 5.0) / 1024.00;

Here in these formula batteryVoltageValue is the value that is read by ATmega as analog input, which is further
multiplied by the voltage that is been supplied by ATmega and thus to get the Vout it is divided 
by the cycle of time that is covered after every bit to get the value.

Vin = Vout / (R2/R1+R2)
By this formula we can find the Vin that will be around 0, which indirectly means we are creating ground.
 
 */
