void getVRef(){
  float vRef = readVcc();         // get Arduino's Vcc, should be 5V or 3.3V, used in keypad shield's button detection function
  Serial.print("\r\nVcc: ");
  Serial.print(vRef / 1000);
  vRef = 5000 / vRef;
  Serial.print("\r\nScaler (5 ");
  Serial.print((char)247);      // '÷' symbol
  Serial.print(" Vcc): ");
  Serial.print(vRef);

  // Scale analog input references according to Vcc Ref, lower Vcc mean higher analog reading because Keypad shield runs at 5V
  //inRightRef  *= vRef;  // RIGHT doesn't need scaling, it pulls to 0 regards of Vcc
  inUpRef *= vRef;
  inDownRef *= vRef;
  inLeftRef *= vRef;
  inSelectRef *= vRef;

}

// the following readVcc code was used from https://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  //result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  result = 1132916L / result; // calibrated for Due
  return result; // Vcc in millivolts
}
