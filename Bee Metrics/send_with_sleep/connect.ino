void establishContact()
{
  while (mySerial.available() <= 0)
  {
    mySerial.println('A');   // send a capital A
    Serial.print("A");
    delay(300);
    connectionCount ++;
    if (connectionCount > 30){
      connectionCount = 0;
      Sleeping();
      }
    else if (serialGo == 'B') {
      sendData = true;
      connectionCount = 0;
      break;
    }
  }
}
