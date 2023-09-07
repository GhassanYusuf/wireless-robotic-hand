//==================================================================
//  HEADER
//==================================================================

  #include "header.h"

//==================================================================
//  SETUP
//==================================================================

  void setup() {

    // Serial Port
    Serial.begin(38400);        // Serial Communication
    Bluetooth.begin(38400);     // Bluetooth ComPort

    // Setting Up Fingers
    setup_fingers();

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
        fingerValue[i].position = constrain(analogRead(fingerValue[i].pin), fingerValue[i].min, fingerValue[i].max);

        // Mapping The Value
        unsigned int percentage = map(fingerValue[i].position, fingerValue[i].min, fingerValue[i].max, 0, 100);

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
//  END
//==================================================================