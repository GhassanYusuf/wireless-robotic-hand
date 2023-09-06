//==================================================================
//  LIBRARIES
//==================================================================

  #include        <SoftwareSerial.h>  // Software
  SoftwareSerial  Bluetooth(11, 10);  // Software Bluetooth

  #include        <NewServo.h>        // Ghassan Library
  NewServo        finger[5];          // 5 Servos

  struct AnaInput {
    uint8_t pin = 0;
    unsigned int prv = 0;
    unsigned int max = 0;
    unsigned int min = 1024;
  };

  AnaInput        fingerValue[5];
  unsigned int    prvValue[5];        // Previous Value Of The Servo Control
  unsigned int    BluetoothData[5];   // To Collect The Information To Be Sent Via Blue Tooth

  unsigned long   scanPeriod;         // Used For Count Time Between Operations
  // [ pin : 14, min : 150, max : 298 ] - [ pin : 15, min : 111, max : 259 ] - [ pin : 16, min : 76, max : 179 ] - [ pin : 17, min : 85, max : 178 ] - [ pin : 18, min : 41, max : 80 ]

//==================================================================
//  SETUP
//==================================================================

  void setup() {

    // Serial Port
    Serial.begin(38400);        // Serial Communication
    Bluetooth.begin(38400);     // Bluetooth ComPort
    
    // Thumb Finger   : Setting Pins Of The Values
    fingerValue[0].pin = 14;
    fingerValue[0].min = 150;
    fingerValue[0].max = 298;

    // Fore Finger    : Setting Pins Of The Values
    fingerValue[1].pin = 15;
    fingerValue[1].min = 111;
    fingerValue[1].max = 259;
    
    // Middle Finger  : Setting Pins Of The Values
    fingerValue[2].pin = 16;
    fingerValue[2].min = 76;
    fingerValue[2].max = 179;
    
    // Ring Finger    : Setting Pins Of The Values
    fingerValue[3].pin = 17;
    fingerValue[3].min = 85;
    fingerValue[3].max = 178;
    
    // Little Finger  : Setting Pins Of The Values
    fingerValue[4].pin = 18;
    fingerValue[4].min = 41;
    fingerValue[4].max = 80;

    // Servo Setup Loop
    for(int i=0; i<5; i++) {
      finger[i].setPin(i+2);  // Set Finger Servo Pin
      finger[i].setInit(fingerValue[i].max);  // Set Finger Servo Initial Position
      finger[i].setMin(fingerValue[i].min);   // Set Finger Servo Minimum Position
      finger[i].setMax(fingerValue[i].max);   // Set Finger Servo Maximum Position
      finger[i].begin();      // Start Finger Servo Operation
      finger[i].goInit();     // Move Servo To Initial Position
    }

  }

//==================================================================
//  Loop
//==================================================================

  void loop() {

    // Min Reading Value (724 - 725) , Max Reading Value (800, 835)
    if(millis() - scanPeriod >= 100) {
      
      // Update Period
      scanPeriod = millis();

      // Loop Through Fingers
      for(int i=0; i<5; i++) {

        // Reading New Value
        fingerValue[i].prv        = constrain(analogRead(fingerValue[i].pin), fingerValue[i].min, fingerValue[i].max);

        // Mapping The Value
        unsigned int  percentage  = map(fingerValue[i].prv, fingerValue[i].min, fingerValue[i].max, 0, 100);

        // Print Value
        Serial.print(percentage, DEC);
        Serial.print(" ");

        // Finger Move
        finger[i].percent(percentage);

        // Prepare Data For Bluetooth
        BluetoothData[i] = percentage;

      }

      // New Line
      Serial.println();

      // Send Values Via Bluetooth
      // Bluetooth.write(BluetoothData, 5);

    }
    
  }

//==================================================================
//  Serial Control Fingers
//==================================================================

  void controlFingers(NewServo &servo, unsigned int &previous, uint8_t analogValue, unsigned int inMinValue, unsigned int inMaxValue, unsigned int outMinValue, unsigned int outMaxValue) {

      // Scan Period
      scanPeriod = millis();

      // Condition The Value
      unsigned int  value = constrain(analogRead(analogValue), inMinValue, inMaxValue);
                    value = map(value, inMinValue, inMaxValue, outMinValue, outMaxValue);

      if(previous != value) {
        previous = value;
        Serial.println(previous);
        servo.move(previous);
      }

  }

//==================================================================
//  Serial Control Fingers
//==================================================================

  void serialControlFingers() {

    // Serial Command For Fingers
    if(Serial.available()) {

      // Serial Port Reading
      String x = Serial.readStringUntil('\n');

      // Serial
      while(Serial.available()) { Serial.read(); }

      // Trim Extra Spaces
      x.trim();

      // Reading
      uint8_t value = x.toInt();

      // Print 
      Serial.println("Move  : " + String(value));

      // Move The Motors
      for(int i=0; i<5; i++) {
        
        if(i == 2) {
          finger[i].move(map(value, 180, 0, 0, 180));
        } else {
          finger[i].move(value);
        }

      }

    }

  }

//==================================================================
//  Serial Bluetooth Setup
//==================================================================

  void setupBluetooth() {

    // Serial Command For Fingers
    if(Serial.available()) {

      // Serial Port Reading
      String x = Serial.readStringUntil('\n');

      // Serial
      while(Serial.available()) { Serial.read(); }

      // Trim Extra Spaces
      x.trim();

      // Display Print 
      Serial.println("Serial      : " + String(x));

      // Send To Blue Tooth
      Bluetooth.println(x);

    }

    // Serial Command For Fingers
    if(Bluetooth.available()) {

      // Serial Port Reading
      String x = Bluetooth.readStringUntil('\n');

      // Serial
      while(Bluetooth.available()) { Bluetooth.read(); }

      // Trim Extra Spaces
      x.trim();

      // Print 
      Serial.println("Bluetooth   : " + String(x));

    }

  }

//==================================================================
//  Get Analog Min Max Values
//==================================================================

  void getMinMaxValues() {

    // Looping
    for(int i=0; i<5; i++) {

      // Reading The Value
      fingerValue[i].prv = analogRead(fingerValue[i].pin);

      // If The Value Is Greater Than Max
      if(fingerValue[i].max < fingerValue[i].prv) {
        fingerValue[i].max = fingerValue[i].prv;      // Set Maximum
      }

      // If The Value Is Lower Than Min
      if(fingerValue[i].min > fingerValue[i].prv) {
        fingerValue[i].min = fingerValue[i].prv;      // Set Minimum
      }

      // Print Values
      Serial.print(
                    "[ (" +
                    String(map(fingerValue[i].prv, fingerValue[i].min, fingerValue[i].max, 0, 100), DEC) + 
                    ") pin : " +
                    String(fingerValue[i].pin, DEC) + 
                    ", min : " +
                    String(fingerValue[i].min, DEC) +
                    ", max : " +
                    String(fingerValue[i].max, DEC) + 
                    " ]"
                  );

      // To Print Only 4 Time
      if(i < 4) {
        Serial.print(" - ");
      }

    }

    // Print New Line
    Serial.println();

  }

//==================================================================
//  END
//==================================================================
