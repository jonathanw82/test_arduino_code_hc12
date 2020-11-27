void parseData() {      // split the data into its parts

  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, ",");     // get the first part - the string
  strcpy(DataA, strtokIndx);               // copy it to DataA

  strtokIndx = strtok(NULL, ",");          // get the second part - the string
  strcpy(DataB, strtokIndx);               // copy it to DataB

  strtokIndx = strtok(NULL, ",");          // get the third part - the string
  strcpy(DataC, strtokIndx);               // copy it to DataC
  
  strtokIndx = strtok(NULL, ",");          // get the forth part - the string
  strcpy(DataD, strtokIndx);               // copy it to DataD

  strtokIndx = strtok(NULL, ",");          // this continues where the previous call left off
  humidity = atof(strtokIndx);             // cconvert this part to a float

  strtokIndx = strtok(NULL, ",");
  temperature = atof(strtokIndx);          // convert this part to a float

  strtokIndx = strtok(NULL, ",");
  sound = atof(strtokIndx);                // convert this part to a float

  strtokIndx = strtok(NULL, ",");
  batteryTransmitter = atof(strtokIndx);   // convert this part to a float

}
