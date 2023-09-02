//==================================================================
//  LIBRARIES
//==================================================================

  #include        <SoftwareSerial.h>  // Software
  SoftwareSerial  Bluetooth(11, 10);  // Software Bluetooth

  #include        <NewServo.h>        // Ghassan Library
  NewServo        finger[5];          // 5 Servos
  uint8_t         prvValue[5];        // Previous Value Of The Servo Control

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

      // Looping Through Fingers
      for(int i=0; i<5; i++) {
        controlFingers(finger[i], prvValue[i], i+14, 724, 835, 0, 130);
      }

    }
    
  }

//==================================================================
//  Function To Control A Finger
//==================================================================

  void controlFingers(NewServo &servo, uint8_t &previous, uint8_t analogValue, unsigned int inMinValue, unsigned int inMaxValue, unsigned int outMinValue, unsigned int outMaxValue) {

      // Scan Period
      scanPeriod = millis();

      // Condition The Value
      unsigned int  value = constrain(analogRead(analogValue), inMinValue, inMaxValue);
                    value = map(value, inMinValue, inMaxValue, outMinValue, outMaxValue);

      // Do Change If Not Maching
      if(previous != value) {
        previous = value;
        Bluetooth.write(prvValue, 5);
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
//  Receive Data From Bluetooth
//==================================================================

  void receiveData() {

    if (Bluetooth.available() >= 5) {
      
      // Read the received data into an array
      byte receivedData[5];
      Bluetooth.readBytes(receivedData, 5);

      // Move the servos based on the received byte values
      Serial.print("Received data : ");
      for(int i=0; i<5; i++) {
        // Print the received byte values
        Serial.print(receivedData[i]); Serial.print(" ");
        finger[i].move(receivedData[i]);       // Move Servo To Initial Position
      }
      Serial.println();

    }

  }

//==================================================================
//  END
//==================================================================