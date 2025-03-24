#include "HUSKYLENS.h"
#include "Wire.h"
HUSKYLENS huskylens;

int shortDelay = 400; // delay 4s 
int longDelay = 4000; // delay 40s
const int trashID = 1;
const int plasticID = 2;
const int metalID = 3;
const int glassID = 4;

void setup() {
    Serial.begin(115200);
    Wire.begin();   // Connect to I2C bus as master
  // Based on the circuit set up pins
    while (!huskylens.begin(Wire))
      {
          Serial.println(F("Begin failed!"));
          Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
          Serial.println(F("2.Please recheck the connection."));
          delay(100);
      }
}

void loop() {
    // put your main code here, to run repeatedly:
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            sorting(result);
        }    
    }
    // The following code is for learning multiple objects with same ID.
    if(digitalRead(A0) == 0)
    {
      while (!huskylens.writeLearn(1))  // bool writeLearn(int ID)
      {
        Serial.println(F("learn object ID1 failed!")); 
        delay(100);
      }
      Serial.println(F("learn object ID1 success")); 
    }
}

void sorting(HUSKYLENSResult result) {
    if (result.ID == trashID) {
      // Open the trash door
    }
    else if(result.ID == plasticID) {
      // Open the plastic door
    }
    else if(result.ID == metalID) {
      // Open the plastic door
    }
    else {
      // Open the glass door
    }
}