//==================================================================
//  LIBRARIES
//==================================================================

  #include        <SoftwareSerial.h>  // Software
  SoftwareSerial  Bluetooth(11, 10);  // Software Bluetooth

  #include        <NewServo.h>        // Ghassan Library
  NewServo        finger[5];          // 5 Servos
  unsigned int    prvValue[5];        // Previous Value Of The Servo Control

  unsigned long   scanPeriod;         // Used For Count Time Between Operations

//==================================================================
//  SETUP
//==================================================================

  void setup() {

    // Serial Port
    Serial.begin(38400);        // Serial Communication
    Bluetooth.begin(38400);     // Bluetooth ComPort
    
    // Servo Setup Loop
    for(int i=0; i<5; i++) {
      finger[i].setPin(i+2);    // Set Finger Servo Pin
      finger[i].setInit(0);     // Set Finger Servo Initial Position
      finger[i].setMin(0);      // Set Finger Servo Minimum Position
      finger[i].setMax(130);    // Set Finger Servo Maximum Position
      finger[i].begin();        // Start Finger Servo Operation
      finger[i].goInit();       // Move Servo To Initial Position
    }

  }

//==================================================================
//  Loop
//==================================================================

  void loop() {

    // Min Reading Value (724 - 725) , Max Reading Value (800, 835)
    if(millis() - scanPeriod >= 10) {
      controlFingers(finger[0], prvValue[0], A0, 724, 835, 0, 130);
      controlFingers(finger[1], prvValue[1], A1, 724, 835, 0, 130);
      controlFingers(finger[2], prvValue[2], A2, 724, 835, 0, 130);
      controlFingers(finger[3], prvValue[3], A3, 724, 835, 0, 130);
      controlFingers(finger[4], prvValue[4], A4, 724, 835, 0, 130);
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
//  END
//==================================================================