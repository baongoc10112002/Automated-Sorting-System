#include <HUSKYLENS.h>
#include <SoftwareSerial.h>
#include <Wire.h>

// Define HuskyLens communication pins
#define HUSKYLENS_TX 2 // Connect to HuskyLens RX
#define HUSKYLENS_RX 3 // Connect to HuskyLens TX

// Create a SoftwareSerial object for HuskyLens communication
SoftwareSerial huskyLens(HUSKYLENS_TX, HUSKYLENS_RX);

// Define category IDs
#define ID1 1
#define ID2 2
#define ID3 3
#define ID4 4

HUSKYLENS huskylens;

void setup() {
  Serial.begin(115200);
  huskyLens.begin(9600);

  // Initialize HuskyLens in object tracking mode
  while (!huskylens.begin(huskyLens)) {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please check the baud rate!"));
    Serial.println(F("3.Please check the sensor address!"));
    delay(100);
  }

 if (huskylens.writeAlgorithm(ALGORITHM_OBJECT_CLASSIFICATION)) {
    Serial.println("HuskyLens is set to Object Classification Mode.");
  } else {
    Serial.println("Failed to set mode. Please check connections.");
  }

  huskylens.writeAlgorithm(ALGORITHM_OBJECT_CLASSIFICATION); // Use object classification algorithm
  
}


void loop() {
  // Request HuskyLens data
  if (huskylens.request()) {
    if (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      
      // Check if object ID is 1
      if (result.ID == 1) {
        Serial.println("Object with ID 1 detected!");
      }
      else if (result.ID == 2)
      {
        Serial.println("Object with ID 2 detected!");
      }
      else if (result.ID == 3)
      {
        Serial.println("Object with ID 3 detected!");
      }
      else if (result.ID == 4)
      {
        Serial.println("Object with ID 4 detected!");
      }
    }
  } else {
    Serial.println("Failed to receive data from HuskyLens.");
  }
  
  delay(500); // Small delay to avoid spamming Serial output
}