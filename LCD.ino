void scrollListUp(){
  if (scrollIndexPosition > -2){
    scrollIndexPosition -= 2;   // subract 2 from itself
    if (scrollIndexPosition < -2) scrollIndexPosition = -2; // prevent scrolling higher than -2 position (summary data)
    lcdPrint2LineData(scrollIndexPosition);
  }
}

void scrollListDown(){
  if (scrollIndexPosition < tempSensorCount - 2){
    scrollIndexPosition += 2;
    if (scrollIndexPosition > tempSensorCount - 2) scrollIndexPosition = tempSensorCount - 2; // prevent scrolling lower than the last temp sensor
    lcdPrint2LineData(scrollIndexPosition);
  }
}

void lcdPrint2LineData(int index){
  lcd.clear();
  lcd.setCursor(0,0); // beginning of first line
  for (int i = index; i < index + 2; i++){   // loop through twice, printing 2 lines of info, one sensor per line
    if (i >= 0){
      lcd.print(tempSensorReadings[i].bin);
      lcd.print(":");
      lcd.print(tempSensorReadings[i].sensor);
      lcd.print(" ");
      if (displayUnitsCelcuis){
        lcd.print(tempSensorReadings[i].tempC / tempPrecision);       // print number before decimal point as a whole
        lcd.print(".");                                               // print decimal point
        lcd.print(abs(tempSensorReadings[i].tempC % tempPrecision));  // print number after decimal point as a whole
        lcd.print((char)223);                                         // degrees '°' character
        lcd.print("C");
      } else {
        lcd.print(tempSensorReadings[i].tempF / tempPrecision);
        lcd.print(".");
        lcd.print(abs(tempSensorReadings[i].tempF % tempPrecision));
        lcd.print((char)223);
        lcd.print("F");
      }
    } else if (i == -1){
      printNumberOfSensors(&lcd);
    } else if (i == -2){
      printNumberOfBins(&lcd);
    }
    lcd.setCursor(0,1); // beginning of 2nd line
  }
}

void printNumberOfBins(Print *destination){
  destination->print("Bins: ");
  destination->print(binCount);
}

void printNumberOfSensors(Print *destination){
  destination->print("Sensors: ");
  destination->print(tempSensorCount);
}

void checkButtons(){
  int buttonRead = analogRead(0); // IO shared by buttons
  if (buttonRead < 1000){         // continue only if a button is being pressed
    Serial.print("\r\nButton press detected: ");
    Serial.print(buttonRead);    
    while (analogRead(0) < 1000){}       // debouce, wait until stop pressing button, otherwise it might loop rapido
    if (buttonRead < inRightRef){         // Right
      displayUnitsCelcuis = !displayUnitsCelcuis;
      lcdPrint2LineData(scrollIndexPosition);
      Serial.print(" Right: ");
      Serial.print(inRightRef);
    } else if(buttonRead < inUpRef){      // Up
      Serial.print(" Up: ");
      Serial.print(inUpRef);
      scrollListUp();
    } else if(buttonRead < inDownRef){    // Down
      Serial.print(" Down: ");
      Serial.print(inDownRef);
      scrollListDown();
    } else if(buttonRead < inLeftRef){    // Left
      Serial.print(" Left: ");
      Serial.print(inLeftRef);
    } else if(buttonRead < inSelectRef){  // Select
      Serial.print(" Select: ");
      Serial.print(inSelectRef);
    } else {                        // no match, maybe a malfunction
      // do nothing
    }
  }
}

