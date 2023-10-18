//==================================================================
//  HEADER
//==================================================================

  // LED Library
  #include        <LED.h>
  LED             led(13);

  #include        <NewServo.h>
  NewServo        finger[5];

  // Software Serial Library
  #include        <SoftwareSerial.h>
  // Bluetooth Port
  SoftwareSerial  bluetooth(11, 10);

  // Serial Port
  #define         serial          Serial
  #define         pinOffset       2
  #define         minValue        0
  #define         maxValue        100

  // Received Data
  byte            received_data[5];
  byte            swap_data[5];

  // Receive Interval
  unsigned long   prvReceive = 0;

//==================================================================
//  START UP PROGRAM
//==================================================================

  void setup() {
    
    // Starting Ports
    serial.begin(9600);
    bluetooth.begin(9600);

    // LED OFF
    led.off();

    for(int i=0; i<5; i++) {

      // Start From Pin 2
      finger[i].setPin(i+pinOffset);

      // By Setting The Bounderies Of Servo Movment, The Servo Will Not Move Out Of This Bounderies
      finger[i].setMin(minValue);   // The Servo Min Position Is Going To Be 15
      finger[i].setMax(maxValue);   // The Servo Max Position In Goint To Be 148
      finger[i].setInit(minValue);  // The Servo Initial Position

      // Starting The Servo Activity
      finger[i].begin();

      // Go To Initial Position
      finger[i].goInit();

    }

  }

//==================================================================
//  MAIN PROGRAM
//==================================================================

  void loop() {

    receiveData();
    printData();
    controlHand();
    delay(100);

  }

//==================================================================
//  FUNCTIONS
//==================================================================
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

      for(int i=0; i<5; i++) {
        received_data[i] = map(received_data[i], 0, 255, maxValue, minValue);
      }

    }

  }

//------------------------------------------------------------------
//  PRINT DATA
//------------------------------------------------------------------

  void printData() {

    for(int i=0; i<5; i++) {
      serial.print((int)received_data[i]);
      serial.print(" ");
    }

    serial.println();
    
  }

//------------------------------------------------------------------
//  CONTROL SERVO
//------------------------------------------------------------------

  void controlHand() {

    swap_data[4] = received_data[0];
    swap_data[3] = received_data[1];
    swap_data[2] = received_data[2];
    swap_data[1] = received_data[3];
    swap_data[0] = received_data[4];

    for(int i=0; i<5; i++) {
      finger[i].move((int)swap_data[i]);
    }
  }

//==================================================================
//  END OF HEADER
//==================================================================