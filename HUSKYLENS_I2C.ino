/***************************************************
 HUSKYLENS An Easy-to-use AI Machine Vision Sensor
 <https://www.dfrobot.com/product-1922.html>
 
 ***************************************************
 This example shows the basic function of library for HUSKYLENS via I2c.
 
 Created 2020-03-13
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
 <https://wiki.dfrobot.com/HUSKYLENS_V1.0_SKU_SEN0305_SEN0336#target_23>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "HUSKYLENS.h"
#include "Wire.h"
HUSKYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL
void printResult(HUSKYLENSResult result);

int currentID = 1;  // Initialize current ID
int maxObjectsPerID = 5;  // Maximum objects to learn per ID (you can adjust this)\

// Set new name function: this method aim for assigning ID 1 for Trash, ID 2 for Plastic, ID 3 for Metal, and ID 4 for Glass.
void setNewName(String newname, uint8_t ID) {
  while (!huskylens.setCustomName(newname, ID)) {
    Serial.println(F("Custom name failed."));
    delay(100);
  }
}

void setup() {
    Serial.begin(115200);
    pinMode(A0, INPUT_PULLUP);
    pinMode(A1, INPUT_PULLUP); // Button to change ID after learning multiple objects
    Wire.begin();   // Connect to I2C bus as master
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }

    delay(100);   // Short delay for the HuskyLens stabilize

    // Change detected objects' namea. Logically, when objects are learned by arduino
    // objects of the same type may have the same ID; Hence, we can assign the type for each ID.
    setNewName("Trash", 1);
    setNewName("Plastic", 2);
    setNewName("Metal", 3);
    setNewName("Glass", 4);

    // Below are codes for assign IDs for 4 different trash types: Plastic, Metal, Glass, and other.
    while (!huskylens.setCustomName("Trash",1))  // bool setCustomName(String name,uint8_t id)
    {
      Serial.println(F("ID1 customname failed!")); 
      delay(100);
    }
    while (!huskylens.setCustomName("Plastic",2))  // bool setCustomName(String name,uint8_t id)
    {
      Serial.println(F("ID2 customname failed!")); 
      delay(100);
    }
    while (!huskylens.setCustomName("Metal",3))  // bool setCustomName(String name,uint8_t id)
    {
      Serial.println(F("ID3 customname failed!")); 
      delay(100);
    }
    while (!huskylens.setCustomName("Glass",4))  // bool setCustomName(String name,uint8_t id)
    {
      Serial.println(F("ID4 customname failed!")); 
      delay(100);
    }
}

void learnObjects(int id) 
{
    int learnedCount = 0;
    while (learnedCount < maxObjectsPerID)  // Learn multiple objects under current ID
    {
        if (digitalRead(A0) == LOW)  // When A0 is pressed (button to learn)
        {
            // Learn object for the current ID
            while (!huskylens.writeLearn(id)) 
            {
                Serial.println(F("Learning object failed!"));
                delay(100);
            }
            Serial.println(F("Object learned successfully!"));
            learnedCount++;
            delay(1000);  // Wait a bit before learning next object (avoid accidental multiple presses)
        }
    }
}

void loop() {
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
        }    
    }
    learnObjects(currentID);  // Learn objects for the current ID

    // After learning multiple objects, check if the button to change ID is pressed (A1)
    if (digitalRead(A1) == LOW)  // When A1 is pressed (button to change ID)
    {
        currentID++;  // Increment to the next ID
        Serial.print(F("Switching to ID "));
        Serial.println(currentID);
        delay(1000);  // Wait to avoid bouncing issues or accidental multiple presses
    }
    // The following code is for learning multiple objects with same ID.
    // if(digitalRead(A0) == 0)
    // {
    //   while (!huskylens.writeLearn(1))  // bool writeLearn(int ID)
    //   {
    //     Serial.println(F("learn object ID1 failed!")); 
    //     delay(100);
    //   }
    //   Serial.println(F("learn object ID1 success")); 
    // }
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}
