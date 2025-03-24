#include <HUSKYLENS.h>
#include <SoftwareSerial.h>

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
  huskylens.writeAlgorithm(ALGORITHM_OBJECT_CLASSIFICATION); // Use object classification algorithm
}

void loop() {
  // Train the model
  if (!trainModel()) {
    Serial.println(F("Training failed!"));
    return;
  }

  // Save the model to the SD card (specify the file number)
  if (!huskylens.saveModelToSDCard(1)) { // Save to file number 1
    Serial.println(F("Saving model failed!"));
    return;
  }

  Serial.println(F("Training and saving complete!"));
  while (true) {
    // Run object recognition in the loop
    runObjectRecognition();
  }
}

bool trainModel() {
  int categoryCount = 4;
  int objectsPerCategory = 20; // Train with up to 20 objects per category

  for (int category = 1; category <= categoryCount; category++) {
    Serial.print(F("Training category "));
    Serial.println(category);

    // Learn objects for the current category
    for (int i = 1; i <= objectsPerCategory; i++) {
      Serial.print(F("Learning object "));
      Serial.println(i);

      // Wait for the button to be pressed and released
      while (!huskylens.isLearned()) {
        huskylens.request();
        if (huskylens.available()) {
          HUSKYLENSResult result = huskylens.read();
          if (result.command == COMMAND_RETURN_OK) { // Check for COMMAND_RETURN_OK
            break;
          }
        }
      }

      // Assign a custom name to the learned object using writeLearn()
      switch (category) {
        case ID1:
          huskylens.writeLearn(1);
          break;
        case ID2:
          huskylens.writeLearn(2);
          break;
        case ID3:
          huskylens.writeLearn(3);
          break;
        case ID4:
          huskylens.writeLearn(4);
          break;
      }

      // Wait for the button to be released
      while (huskylens.isLearned()) {
        huskylens.request();
      }

      delay(100); // Short delay before learning the next object
    }
  }

  return true;
}

void runObjectRecognition() {
  huskylens.request();
  if (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();
    // Access block information directly from result
    if (result.command == COMMAND_RETURN_BLOCK) { 
      for (int i = 0; i < result.command; i++) { 
        Serial.print(F("Block "));
        Serial.print(i);
        Serial.print(F(": ID = "));
        Serial.print(result.ID); // Access ID using result.id[i]
        Serial.print(F(", X = "));
        Serial.print(result.xCenter);
        Serial.print(F(", Y = "));
        Serial.print(result.yCenter);
        Serial.print(F(", Width = "));
        Serial.print(result.width);
        Serial.print(F(", Height = "));
        Serial.print(result.height);
        Serial.println(); // Just print a newline here
      }
    }
  }
}