void Battvoltage(){

  int batteryVoltageValue = analogRead(A0); //read the A0 pin value
  float voltage = batteryVoltageValue * (5.00 / 1023.00) * 2; //convert the value to a true voltage.
 
 }
