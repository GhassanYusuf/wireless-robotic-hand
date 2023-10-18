//==================================================================
//  HEADER
//==================================================================

  // LED Library
  #include        <LED.h>

  LED             led(13);

  // Software Serial Library
  #include        <SoftwareSerial.h>

  // Serial Port
  #define         serial          Serial
  #define         pinDiviation    13
  #define         minValue        0
  #define         maxValue        255

  // Bluetooth Port
  SoftwareSerial  bluetooth(11, 10);

  // Received Data
  byte            received_data[5];

  // Sensor Data
  struct Sensor {
    byte      data;   // Byte Data
    uint8_t   pin;    // Pin Number
    uint8_t   min;    // Min Value
    uint8_t   max;    // Max Value
  };

  // Sensor Data
  Sensor          sensor_data[5];

  // Send Interval
  unsigned long   prvSend = 0;

//==================================================================
//  START UP PROGRAM
//==================================================================

  void setup() {
    
    // Starting Ports
    serial.begin(9600);
    bluetooth.begin(9600);

    // LED OFF
    led.off();

    // Thumb 0
    sensor_data[0].pin = A0;
    sensor_data[0].min = 31;
    sensor_data[0].max = 73;

    // Thumb 1
    sensor_data[1].pin = A1;
    sensor_data[1].min = 23;
    sensor_data[1].max = 63;

    // Thumb 2
    sensor_data[2].pin = A2;
    sensor_data[2].min = 13;
    sensor_data[2].max = 47;

    // Thumb 3
    sensor_data[3].pin = A3;
    sensor_data[3].min = 17;
    sensor_data[3].max = 46;

    // Thumb 4
    sensor_data[4].pin = A4;
    sensor_data[4].min = 8;
    sensor_data[4].max = 26;

    // Calibration
    getMinMax();

  }

//==================================================================
//  MAIN PROGRAM
//==================================================================

  void loop() {

    // Reading Sensors Loop
    for(int i=0; i<5; i++) {

      // Reading Data From Analog Pins
      int rawValue  = analogRead(sensor_data[i].pin);
      int min       = sensor_data[i].min;
      int max       = sensor_data[i].max;

      // Reading Data And Mapping
      rawValue = constrain(rawValue, min, max);
      rawValue = map(rawValue, min, max, minValue, maxValue);

      // Reading Sensor Data And Mapping It To 0 - 255 max
      sensor_data[i].data = (byte)rawValue;

    }

    // Send data
    sendData(sensor_data);

    // Print Data
    printData(sensor_data);

    // Delay for a specific duration
    delay(100);

  }

//==================================================================
//  FUNCTIONS
//==================================================================
//  SEND DATA
//------------------------------------------------------------------

  void sendData(Sensor* data) {
    
    // Write Data Through Bluetooth Module
    for (size_t i=0; i<5; i++) {
      bluetooth.write(data[i].data);
    }

  }

//------------------------------------------------------------------
//  RECEIVE DATA
//------------------------------------------------------------------

  void receiveData() {

    // If Data Available
    if (bluetooth.available()) {
      
      // Read 1st 5 Bytes
      for(int i=0; i<5; i++) {
        received_data[i] = bluetooth.read();
      }
      
      // Empting Buffer
      while(bluetooth.available()) {
        bluetooth.read();
      }

    }

  }

//------------------------------------------------------------------
//  PRINT DATA
//------------------------------------------------------------------

  void printData(Sensor* data) {

    // Print Data
    for(size_t i=0; i<5; i++) {

      // Convert The Byte To An Integer
      int value = (int)data[i].data;

      // Value
      serial.print(value);

      // White Space
      serial.print(" ");

    }

    // Print New Line
    serial.println();
    
  }

//------------------------------------------------------------------
//  1 Min Get Min And Max Value
//------------------------------------------------------------------

  void getMinMax() {

    // End Of Calibration
    serial.println("-----------------------------------------------------------");
    serial.println("    Start Of Calibration");
    serial.println("-----------------------------------------------------------");

    // Reset Min Max Values Before Calibration
    for(int i=0; i<5; i++) {
      sensor_data[i].min = 1023;
      sensor_data[i].max = 0;
    }

    // Reading Before Starting To Get Data
    for(int i=0; i<20; i++) {
      for(int j=0; j<5; j++) {
        sensor_data[j].data = analogRead(sensor_data[j].pin);
        delayMicroseconds(200);
      }
      delay(49);
    }

    // Serial
    serial.println("Get Ready To Calibrate - Keep You Hand Open To Max");

    // Calibration Start Blink Indicator
    for(int i=0; i<3; i++) {
      led.on();
      delay(250);
      led.off();
      delay(250);
    }

    // Calibration Start
    led.on();
    serial.println("Calibrate Start For 30 Seconds ..... Close Your Hand And Open It Continously");

    // Take Snap Shot
    unsigned int  count   = 0;
    unsigned long prvTime = millis();

    // Wait
    while(count < 30) {

      // Reading Sensors And Capturing Min & Max
      for(int i=0; i<5; i++) {

        // Reading Raw Value
        unsigned int value = analogRead(sensor_data[i].pin);

        // Detect Min
        if(sensor_data[i].max < value) {
          sensor_data[i].max = value;
        }

        // Detect Max
        if(sensor_data[i].min > value) {
          sensor_data[i].min = value;
        }

      }

      // Count Time
      if(millis() - prvTime >= 1000) {
        prvTime = millis();
        serial.println(count);
        count++;
      }

    }

    // Calibration End
    led.off();
    serial.println("Calibrate Ended");

    // Print Results
    for(int i=0; i<5; i++) {
      // Finger Number
      serial.print(i+1);
      serial.println("( " + String(sensor_data[i].min) + " , " + String(sensor_data[i].max) + " )");
    }

    // End Of Calibration
    serial.println("-----------------------------------------------------------");
    serial.println("    End Of Calibration");
    serial.println("-----------------------------------------------------------");

  }

//==================================================================
//  END OF HEADER
//==================================================================