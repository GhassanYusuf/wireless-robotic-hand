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
      fingerValue[i].position = analogRead(fingerValue[i].pin);

      // If The Value Is Greater Than Max
      if(fingerValue[i].max < fingerValue[i].position) {
        fingerValue[i].max = fingerValue[i].position;      // Set Maximum
      }

      // If The Value Is Lower Than Min
      if(fingerValue[i].min > fingerValue[i].position) {
        fingerValue[i].min = fingerValue[i].position;      // Set Minimum
      }

      // Print Values
      Serial.print(
                    "[ (" +
                    String(map(fingerValue[i].position, fingerValue[i].min, fingerValue[i].max, 0, 100), DEC) + 
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
//  Setup Fingers
//==================================================================

  void setup_fingers() {

    // Thumb Finger   : Setting Pins Of The Values
    fingerValue[0].pin = finger_thumb;
    fingerValue[0].min = 150;
    fingerValue[0].max = 298;

    // Fore Finger    : Setting Pins Of The Values
    fingerValue[1].pin = finger_fore;
    fingerValue[1].min = 111;
    fingerValue[1].max = 259;
    
    // Middle Finger  : Setting Pins Of The Values
    fingerValue[2].pin = finger_middle;
    fingerValue[2].min = 76;
    fingerValue[2].max = 179;
    
    // Ring Finger    : Setting Pins Of The Values
    fingerValue[3].pin = finger_ring;
    fingerValue[3].min = 85;
    fingerValue[3].max = 178;
    
    // Little Finger  : Setting Pins Of The Values
    fingerValue[4].pin = finger_little;
    fingerValue[4].min = 41;
    fingerValue[4].max = 80;

  }

//==================================================================
//  Setup Servos
//==================================================================

  void setup_servos() {

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
//  Read From EEPROM
//==================================================================

  void read_boundaries() {

    // Read and print the stored data from EEPROM
    for (int i = 0; i < 5; i++) {
      uint16_t value;
      EEPROM.get(address_start + (i * 2), value);
      Serial.print("Data ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(value);
    }

  }

//==================================================================
//  Store To EEPROM
//==================================================================

  void store_boundaries() {

    // Perform your calibration process and obtain the data values
    uint16_t data[5] = {123, 456, 789, 101, 202};

    // Store The Data In EEPROM
    for (int i = 0; i < 5; i++) {
      EEPROM.put(address_start + (i * 2), data[i]);
    }

    // A Message To Show Data Is Stored
    Serial.println("Data stored in EEPROM.");

  }

//==================================================================
//  END
//==================================================================