void establishContact()
{
  
  while (mySerial.available() <= 0)
  {
    serialGo = mySerial.read();
    mySerial.println('A');   // send a capital A
    Serial.print("A");
    delay(300);
    connectionCount ++;
    if (connectionCount > 30){
      connectionCount = 0;
      wake = false;
    //  mySerial.end();
      Sleeping();
      break;
      }
    else if (serialGo == "B") {
      Serial.print("I have recieved B");
      sendData = true;
      connectionCount = 0;
      
    }
  }
}
