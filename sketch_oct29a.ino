#include <Servo.h>
#include <Wire.h>
#include "HUSKYLENS.h"      // Include the HuskyLens library
HUSKYLENS huskylens;        // Initialize the HuskyLens object

// Define servo motors
Servo organicServo;
Servo metalServo;
Servo glassServo;
Servo plasticServo;

// Define servo pins
const int organicServoPin = 3;
const int metalServoPin = 5;
const int glassServoPin = 6;
const int plasticServoPin = 9;

const int metalEcho=13, glassEcho=12, plasticEcho=11, organicEcho=8;
const int trig=7;

int organicDuration, plasticDuration, glassDuration, metalDuration;         
float organicDistance, plasticDistance, glassDistance, metalDistance;

// Define LED pins
const int empty = A0;  // Moved to A0
const int half = A1;   // Moved to A1
const int full = A2;   // Moved to A2 

// Mode toggle
int mode = 0;

void setup() 
{   
  Serial.begin(9600);   
  Wire.begin();                     // Start I2C
  huskylens.begin(Wire);            // Start HuskyLens on I2C with Wire

  // Initialize servo motors
  organicServo.attach(organicServoPin, 500, 2500);  
  metalServo.attach(metalServoPin, 500, 2500);      
  glassServo.attach(glassServoPin, 500, 2500);      
  plasticServo.attach(plasticServoPin, 500, 2500);  

  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  pinMode(organicEcho, INPUT);
  pinMode(plasticEcho, INPUT);
  pinMode(glassEcho, INPUT);
  pinMode(metalEcho, INPUT);

  pinMode(empty, OUTPUT);
  pinMode(half, OUTPUT);
  pinMode(full, OUTPUT);

  delay(6000);
  Serial.println("Distance:"); 
}  

void loop() 
{   
  // Check for user input from serial monitor
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == 'u') {
      mode = 0;
      Serial.println("Switched to User Input Mode");
    } else if (input == 'o') {
      mode = 1;
      Serial.println("Switched to Object Recognition Mode");
    }
  }

  if (mode == 0) {
    // User Input Mode
    if (Serial.available()) {
      String objectType = Serial.readStringUntil('\n');
      objectType.trim();
      if (objectType == "organic") {
        openBinDoor(organicServo);
      } else if (objectType == "plastic") {
        openBinDoor(plasticServo);
      } else if (objectType == "metal") {
        openBinDoor(metalServo);
      } else if (objectType == "glass") {
        openBinDoor(glassServo);
      } else {
        Serial.println("Invalid object type.");
      }
    }
  } else if (mode == 1) {
    // Object Recognition Mode
    if (huskylens.request()) {
      if (huskylens.isLearned()) {
        HUSKYLENSResult result = huskylens.read();  // Use read() without arguments
        if (result.ID == 1) {
          openBinDoor(organicServo); //set to operate when detecting an orange monster can bottle:ID1
        } else if (result.ID == 2) {
          openBinDoor(plasticServo);
        } else if (result.ID == 3) {
          openBinDoor(metalServo);
        } else if (result.ID == 4) {
          openBinDoor(glassServo);
        }
      }
    }
  }

  // Distance and LED logic (unchanged)
  digitalWrite(trig, HIGH);       
  delayMicroseconds(10);
  digitalWrite(trig, LOW);       
  organicDuration = pulseIn(organicEcho, HIGH);
  plasticDuration = pulseIn(plasticEcho, HIGH);
  glassDuration   = pulseIn(glassEcho, HIGH);
  metalDuration   = pulseIn(metalEcho, HIGH);

  organicDistance = organicDuration / 58;
  plasticDistance = plasticDuration / 58;
  glassDistance = glassDuration / 58;
  metalDistance = metalDuration / 58;

  if (organicDistance >= 50.00 || plasticDistance >= 50.00 || glassDistance >= 50.00 || metalDistance >= 50.00) {
    digitalWrite(empty, HIGH); 
    digitalWrite(half, LOW);   
    digitalWrite(full, LOW);
  } else if (organicDistance > 10.00 || plasticDistance > 10.00 || glassDistance > 10.00 || metalDistance > 10.00) {
    digitalWrite(empty, LOW);
    digitalWrite(half, HIGH);  
    digitalWrite(full, LOW);
  } else {
    digitalWrite(empty, LOW);
    digitalWrite(half, LOW);
    digitalWrite(full, HIGH);  
  }

  delay(5000);    
}  

void openBinDoor(Servo servo) 
{
  servo.write(90);
  delay(1000);
  servo.write(0);  
}
