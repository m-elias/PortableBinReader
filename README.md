# PortableBinReader

# from Notes.ino

 * Arduino 168/328 based DS18B20 portable reader for use with LCD Keypad shield
 * 
 * created 2017 04 04
 * by Matt Elias
 * modified 2017 04 12
 * 
 * 
 * Reads all detected DS18B20 or DS28EA00 OneWire sensors at boot up
 * Stores the list of sensor data in an array
 *  data byte 0 - least significant temperature byte
 *  data byte 1 - most significant temperature byte
 *  data byte 2 - BIN ID number
 *  data byte 3 - SENSOR ID number
 * Sorts the array of sensors by bin/sensor ID numbers
 * Allows user to scroll through list via LCD Keypad shield's UP/DOWN buttons
 * Changes displayed units with RIGHT button
 * 
 * 
 * Buttons:
 *  UP: scrolls up in the list of sensors
 *  DOWN: scrolls down in the list of sensors
 *  RIGHT: toggles temperature units between C and F
 *  LEFT: does nothing
 *  SELECT: does nothing
 *  
 *  
 * To-do:
 *  use left button to change displayed bin number (sideways scolling through bins)?
 *  add ability to reprogram sensors' bin ID?
 *  
 *  
 * v03
 *  added code to detect Arduino's Vcc and scale Keypad shield analog button detected accordingly
 *    The Keypad shield operates at 5V
 *      SELECT button does not work on 3.3V systems as it cannot pull down enough on the analog input to register a change
 *  changed temp units display toggle to RIGHT button
 *  
 * v02
 *  added code to change displayed temperature units with the Select button
 * 
 * v01
 *  selectable precision in program
 *  reads sensors once at boot up
 *  uses up/down buttons to scroll through list of sensors
 * 

