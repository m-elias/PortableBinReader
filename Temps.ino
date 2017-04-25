void readSensors(){
  int tReading, negBit=1, Tf_100, Tc_100;
  byte data1, data2;
  byte addr[8];
  tempSensorCount = 0;

  Serial.print("\r\n\nReading...");
  lcd.setCursor(0,0);
  lcd.print("Reading...");  

  ds.reset();
  ds.write(0xCC,1);   // skip rom command
  ds.write(0x44,1);   // start conversion, with parasite power
  delay(850);         // wait for sensors to finished conversion

  while(ds.search(addr)){   // loop through, until all detected sensors are read
    //verify addr is uncorrupted and device is part of the DS18B20 or DS28EA00 family
    if (OneWire::crc8(addr,7) == addr[7] && (addr[0] == 0x28 || addr[0] == 0x42)){
      ds.reset();
      ds.select(addr);    
      ds.write(0xBE);   // Issue read scratchpad cmd
      data1=ds.read();  // Read only the first 2 bytes which contain temperature data
      data2=ds.read();
  
      tReading = (data2 << 8) + data1;  // combine the 2 temp data bytes, LSB came first, so assemble in reverse order
      if (tReading & 0x8000){           // determine if temp is a negative value
        negBit = -1;
        tReading = (tReading ^ 0xffff) + 1;
      } else {
        negBit = 1;
      }
      // convert reading to Celcius and Fahrenheit to desired decimal places
      Tc_100 = ((6 * tReading) + (tReading / 4)) / (100 / tempPrecision) * negBit;
      Tf_100 = Tc_100 * 9 / 5 + (32 * tempPrecision); 
  
      /*
      for (byte i=0;i<9;i++){
        Serial.print(addr[i],HEX);
        if (i<8) Serial.print("-");
      }
      Serial.print(" => ");
      */

      tempSensorReadings[tempSensorCount].bin = ds.read();     // read the next byte with bin number and save to sensor array
      tempSensorReadings[tempSensorCount].sensor = ds.read();  // read the next byte with sensor number and save to sensor array
      tempSensorReadings[tempSensorCount].tempC = Tc_100;      // save Celcius value to sensor array
      tempSensorReadings[tempSensorCount].tempF = Tf_100;      // save Fahrenheit value to sensor array

      printTempReading(&Serial, tempSensorReadings[tempSensorCount]);

      // track how many unique bins are detected
      if (binCount == 0){
        binList[binCount] = tempSensorReadings[tempSensorCount].bin;
        binCount++;
      } else {
        boolean noMatch = 1;
        for (byte i=0;i<binCount;i++){
          if (binList[i] == tempSensorReadings[tempSensorCount].bin){
            noMatch = 0;
            break;
          }
        }
        if (noMatch){
          binList[binCount] = tempSensorReadings[tempSensorCount].bin;
          binCount++;          
        }
      }
      
      tempSensorCount++;    // advance count for next sensor

    }
  }
  ds.reset_search();    // reset OneWire bus search
  Serial.println();

  Serial.print("\r\nPre-sorting");
  printTempReadingList(&Serial);
  sortTempSensors();
  Serial.print("\r\n\nPost-sorting");
  printTempReadingList(&Serial);

  lcd.clear();
  lcd.setCursor(0,0);
  printNumberOfBins(&lcd);
  lcd.setCursor(0,1);
  printNumberOfSensors(&lcd);
  scrollIndexPosition = -2;
}

// The following sortTempSensors code was provided by Arduino forum user 'int2str' http://forum.arduino.cc/index.php?topic=189005.msg1398504#msg1398504
void sortTempSensors(){
  boolean swapped;
  TempReadingClass temp;
  do{
    swapped = false;
    for (byte i = 0; i < tempSensorCount - 1; ++i){
      if (tempSensorReadings[i] > tempSensorReadings[i+1]){
        temp = tempSensorReadings[i];
        tempSensorReadings[i] = tempSensorReadings[i + 1];
        tempSensorReadings[i + 1] = temp;
        swapped = true;
      }
    }
  } while (swapped);
}

void printTempReadingList(Print *destination){
  for (byte i=0; i<tempSensorCount; ++i){
    printTempReading(destination, tempSensorReadings[i]);
  }
}

void printTempReading(Print *destination, TempReadingClass temp){
    destination->print("\r\n");
    destination->print("Sensor ");
    destination->print(temp.bin);
    destination->print(":");
    destination->print(temp.sensor);
    destination->print(" = ");
    destination->print(temp.tempC / tempPrecision);
    destination->print(".");
    destination->print(abs(temp.tempC % tempPrecision));
    destination->print((char)176);
    destination->print("C");
    destination->print(" / ");
    destination->print(temp.tempF / tempPrecision);
    destination->print(".");
    destination->print(abs(temp.tempF % tempPrecision));
    destination->print((char)176);
    destination->print("F");
}
