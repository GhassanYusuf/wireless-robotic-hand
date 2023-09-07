#include <EEPROM.h>

// Define the addresses where you want to store the data
const int addressStart = 1;  // Starting address in EEPROM

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Perform calibration and store the data in EEPROM
  calibrateAndStoreData();
  
  // Read and print the stored data from EEPROM
  printData();
}

void loop() {
  // Your main code goes here
}

void calibrateAndStoreData() {
  // Perform your calibration process and obtain the data values
  uint16_t data[5] = {123, 456, 789, 101, 202};

  // Store the data in EEPROM
  for (int i = 0; i < 5; i++) {
    EEPROM.put(addressStart + (i * 2), data[i]);
  }

  Serial.println("Data stored in EEPROM.");
}

void printData() {
  // Read and print the stored data from EEPROM
  for (int i = 0; i < 5; i++) {
    uint16_t value;
    EEPROM.get(addressStart + (i * 2), value);
    Serial.print("Data ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(value);
  }
}