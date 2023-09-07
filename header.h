//==================================================================
//  LIBRARIES
//==================================================================
//  Pin Definitions
//------------------------------------------------------------------

  // EEPROM Address
  #define   address_eecal       0     // Storage Of Calibration Flag
  #define   address_start       1     // Start Of EEPROM Data

  // Analog Pins
  #define   finger_thumb        14    // 14 is A0
  #define   finger_fore         15    // 15 is A1
  #define   finger_middle       16    // 16 is A2
  #define   finger_ring         17    // 17 is A3
  #define   finger_little       18    // 18 is A4

  // LED Pins
  #define   led_run             4     // GREEN
  #define   led_calibration     5     // RED

  // Digital Pins
  #define   button_calsave      3     // Button

  // Bluetooth Pins
  #define   bluetooth_txd_pin   11    // Transmit Pin
  #define   bluetooth_rxd_pin   10    // Receive Pins

//------------------------------------------------------------------
//  New Data Type
//------------------------------------------------------------------

  struct AnaInput {
    uint8_t       pin       = 0;      // This is The Pin Number For Analog Value
    unsigned int  position  = 0;      // This is Where We Store The Current Reading
    unsigned int  max       = 0;      // This is Where We Store The Maximum Value
    unsigned int  min       = 1024;   // This is Where We Store The Minimum Value
  };

//------------------------------------------------------------------
//  Libraries & Objects
//------------------------------------------------------------------

  // EEPROM Library
  #include        <EEPROM.h>

  // Button Library
  #include        <Button.h>

  // Button Object
  Button          calsave(button_calsave);  // Button For Calibrate & Save

  // LED Library
  #include        <LED.h>

  // Green LED Indication Calibration Is Done And Its In Running Mode
  LED             ready(led_run);

  // Red LED Indicating Calibration
  LED             calibration(led_calibration);

  // Software Serial Library
  #include        <SoftwareSerial.h>

  // Software Bluetooth
  SoftwareSerial  Bluetooth(bluetooth_txd_pin, bluetooth_rxd_pin);

  // Ghassan New Servo Library
  #include        <NewServo.h>

  // 5 Servo Objects
  NewServo        finger[5];

  // 5 Instances Of New Data Structure
  AnaInput        fingerValue[5];     // fingers control pin, movment, (min/max) boundaries.

  // Variables
  unsigned long   scanPeriod;         // Used For Count Time Between Operations
  unsigned int    positionValue[5];   // Previous Value Of The Servo Control
  unsigned int    BluetoothData[5];   // To Collect The Information To Be Sent Via Blue Tooth

  // [ pin : 14, min : 150, max : 298 ] - [ pin : 15, min : 111, max : 259 ] - [ pin : 16, min : 76, max : 179 ] - [ pin : 17, min : 85, max : 178 ] - [ pin : 18, min : 41, max : 80 ]

//==================================================================
//  END OF HEADER
//==================================================================