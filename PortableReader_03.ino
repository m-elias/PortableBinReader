#include <LiquidCrystal.h>
#include <OneWire.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // select the pins used on the LCD panel
OneWire ds(A5);   // IO for OneWire sensors

// The following 'TempReadingClass' code was provided by Arduino forum user 'int2str' http://forum.arduino.cc/index.php?topic=189005.msg1398504#msg1398504
class TempReadingClass{
  public:
    byte sensor;        // stores sensor's SENSOR ID number, read from sensor's low alarm setpoint
    byte bin;           // stores sensor's BIN ID number, read from sensor's high alarm setpoint
    int tempC;          // temperature in Celcuis
    int tempF;          // temperature in Fahrenheit
    // Comparison operator for two TempReadingClass objects, will sort according to '>'. Without it, the IDE does not know what to compare
    // Sort priority 1 = bin ID, priority 2 = sensor ID
    boolean operator> (const TempReadingClass &rhs){
      return ((bin << 8) + sensor) > ((rhs.bin << 8) + rhs.sensor); // combine the bin ID and sensor ID and compare both together
    }
};

const byte maxNumTempSensors = 50; // max number of sensors, keep low enough so that Arduino does not run out of RAM/Memory
TempReadingClass tempSensorReadings[maxNumTempSensors]; // array to store detected sensor readings
byte tempSensorCount = 0;          // counts number of detected sensors
byte tempPrecision = 100;          // 100 - for 2 decimal places, 10 - for 1 decimal place
boolean displayUnitsCelcuis = 1;   // 1 - Celcuis, 0 - Fahrenheit
byte binList[8];                   // stores list of bin numbers for the purpose of displaying a bin count
byte binCount = 0;                 // counts the unique number of bin detected
int scrollIndexPosition = -2;      // keeps track of lcd display position index

// Keypad shield's analog button press references for 5V Vcc
long inRightRef  = 50;
long inUpRef     = 195;
long inDownRef   = 380;
long inLeftRef   = 555;
long inSelectRef = 790;

void setup(){
  Serial.begin(115200);
  lcd.begin(16, 2);         // start the library
  lcd.setCursor(0,0);       // set cursor position
  lcd.print("Startup...");  // print startup msg
  getVRef();                // get Vcc, and scale analog read references for the keypad shield
  
  delay(1000);
  readSensors();            // try to read all sensors once
}

void loop(){  
  checkButtons();           // check for button presses
}


