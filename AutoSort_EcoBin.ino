/*
  AutoSort EcoBin - Automated Sorting System
  University of Toledo Dept of EECS. 
  Fall 2024 - Senior Design II

  Group Members: 
  Tylor Reed
  Tran Bao Ngoc Ngyuen
  Rodion Pelykh
  Rafael Mercado 
  Joel Dumevi

  Huskylens AI Vision Sensor image based recognition of trash, 
  Resulting ID parsed and servos moved in accordance to the respective sorting sequence. 

  Desired bin will open, and opposing bin will over extend to push material into the desired bin


 */



#include <HUSKYLENS.h>
#include <Wire.h>
#include <Servo.h>

// Define object inserted detection
const int obstacleSensorPin = 6; // Orange Wire

// Define servo motors
Servo organicServo;
Servo metalServo;
Servo glassServo;
Servo plasticServo;

// Define servo pins
const int organicServoPin = 2;
const int metalServoPin = 3;
const int glassServoPin = 4;
const int plasticServoPin = 5;

// Define ultrasonic sensors trigger/echo pins
const int organicTrig=35, metalTrig=37, glassTrig=39, plasticTrig=41; // Yellow Wire Trig
const int organicEcho=34, metalEcho=36, glassEcho=38, plasticEcho=40; // Orange Wire Echo

// Define bin level distance variables        
float organicDistance, plasticDistance, glassDistance, metalDistance;

// Define bin level LED pins
const int organicBinEmpty = 52;   // Green LED for empty
const int organicBinHalf  = 50;   // Yellow LED for almost full
const int organicBinFull  = 48;   // Red LED for full

const int metalBinEmpty = 46;     // Green LED for empty
const int metalBinHalf  = 44;     // Yellow LED for almost full
const int metalBinFull  = 42;     // Red LED for full

const int glassBinEmpty = 53;     // Green LED for empty
const int glassBinHalf  = 51;     // Yellow LED for almost full
const int glassBinFull  = 49;     // Red LED for full

const int plasticBinEmpty = 47;   // Green LED for empty
const int plasticBinHalf  = 45;   // Yellow LED for almost full
const int plasticBinFull  = 43;   // Red LED for full

HUSKYLENS huskylens;
HUSKYLENSResult result;

// Function for opening/closing respective bin doors
void openBinDoor(Servo openServo, Servo pushServo);

// Function to read distance from an ultrasonic sensor
int readDistance(int trigPin, int echoPin);

// Function to update bin level LEDs
void updateBinLevel(int distance, int emptyPin, int halfPin, int fullPin);

// Function to train the model with more images
void trainModel();


void setup() 
{
  Serial.begin(115200);   
  
  Wire.begin();

  // Bind HuskyLens object to I2C conenction)
  while (!huskylens.begin(Wire))
  {
      Serial.println(F("Begin failed!"));
      delay(50);
  }
  
  // Set the sensor pin for object detection as an input
  pinMode(obstacleSensorPin, INPUT); 

  // Initialize servo motors
  organicServo.attach(organicServoPin, 500, 2500);      // Set PWM range
  organicServo.write(90);
  metalServo.attach(    metalServoPin, 500, 2500);      // Set PWM range
  metalServo.write(  90);
  glassServo.attach(    glassServoPin, 500, 2500);      // Set PWM range
  glassServo.write(  90);
  plasticServo.attach(plasticServoPin, 500, 2500);      // Set PWM range
  plasticServo.write(90);

  // Set trigger pins as outputs
  pinMode(organicTrig, OUTPUT);
  digitalWrite(organicTrig, LOW);
  delayMicroseconds(2);
  pinMode(metalTrig, OUTPUT);
  digitalWrite(metalTrig, LOW);
  delayMicroseconds(2);
  pinMode(glassTrig, OUTPUT);
  digitalWrite(glassTrig, LOW);
  delayMicroseconds(2);
  pinMode(plasticTrig, OUTPUT);
  digitalWrite(plasticTrig, LOW);
  delayMicroseconds(2);

  // Set echo pins as inputs
  pinMode(organicEcho, INPUT);
  pinMode(metalEcho, INPUT);
  pinMode(glassEcho, INPUT);
  pinMode(plasticEcho, INPUT);

  // Set LED pins as output
  pinMode(organicBinEmpty, OUTPUT);
  pinMode(organicBinHalf, OUTPUT);
  pinMode(organicBinFull, OUTPUT);

  pinMode(metalBinEmpty, OUTPUT);
  pinMode(metalBinHalf, OUTPUT);
  pinMode(metalBinFull, OUTPUT);

  pinMode(glassBinEmpty, OUTPUT);
  pinMode(glassBinHalf, OUTPUT);
  pinMode(glassBinFull, OUTPUT);

  pinMode(plasticBinEmpty, OUTPUT);
  pinMode(plasticBinHalf, OUTPUT);
  pinMode(plasticBinFull, OUTPUT);

  // Initialize buttons to be used to train new objects
  pinMode(A0, INPUT_PULLUP); // Press button to train new Do nothing object
  pinMode(A1, INPUT_PULLUP); // Press button to train new organic object
  pinMode(A2, INPUT_PULLUP); // Press button to train new metal object
  pinMode(A3, INPUT_PULLUP); // Press button to train new glass object
  pinMode(A4, INPUT_PULLUP); // Press button to train new plastic object
  pinMode(A5, INPUT_PULLUP); // Enter training mode
  pinMode(A6, INPUT_PULLUP); // Exit training mode

  delay(1000);
}

void loop() 
{
  // Training mode activiation
  if(digitalRead(A5) == LOW)
  {
    trainModel();
    Serial.println("EXITED TRAINING MODE!");
  }
  // Check that an object has been placed in the sorting mechanism
  if (digitalRead(obstacleSensorPin) == LOW)
  {
    if (!huskylens.request()) 
    {
      Serial.println(F("Fail to request data from HUSKYLENS."));
      return;
    }
    result = huskylens.read();
    Serial.print("Command: 0x");
    Serial.println(result.command, HEX);
    Serial.print("result.ID: ");
    Serial.println(result.ID);
    delay(100);
    
    // Check if an object was detected
    if (result.command == COMMAND_RETURN_BLOCK) 
    {
      switch (result.ID) 
      {
        case 1: // Nothing (ID1)
          // Continue
          Serial.println("Nothing:ID1 detected. Continuing...");
          break;
        case 2: // Organic (ID2)
          // Command servos for Organic
          Serial.println("Organic:ID2 detected. Commanding servos for Organic...");
          openBinDoor(organicServo, glassServo); 
          break;
        case 3: // Metal (ID3)
          // Command servos for Metal
          Serial.println("Metal:ID3 detected. Commanding servos for Metal...");
          openBinDoor(metalServo, plasticServo);
          break;
        case 4: // Glass (ID4)
          // Command servos for Glass
          Serial.println("Glass:ID4 detected. Commanding servos for Glass...");
          openBinDoor(glassServo, organicServo); 
          break;
        case 5: // Plastic (ID5)
          // Command servos for Plastic
          Serial.println("Plastic:ID5 detected. Commanding servos for Plastic...");
          openBinDoor(plasticServo, metalServo); 
          break;
        default:
          // Handle unexpected IDs (optional)
          Serial.println("Unknown ID detected.");
          break;
      }
    }
  }
  // Read distances for each bin level from ultrasonic sensors
  organicDistance = readDistance(organicTrig, organicEcho);
  metalDistance = readDistance(metalTrig, metalEcho);
  glassDistance = readDistance(glassTrig, glassEcho);
  plasticDistance = readDistance(plasticTrig, plasticEcho);
  delay(5000);

  if (organicDistance == -1 || metalDistance == -1 || glassDistance == -1 || plasticDistance == -1) 
  {
    Serial.println("Out of range");
  } 
  else 
  {    
    // Print distances for each bin level
    Serial.print("Organic Level: ");
    Serial.print(organicDistance);
    Serial.println(" inches");

    Serial.print("Metal Level: ");
    Serial.print(metalDistance);
    Serial.println(" inches");

    Serial.print("Glass Level: ");
    Serial.print(glassDistance);
    Serial.println(" inches");

    Serial.print("Plastic Level: ");
    Serial.print(plasticDistance);
    Serial.println(" inches");
  }

  // Update bin level LEDs
  updateBinLevel(organicDistance, organicBinEmpty, organicBinHalf, organicBinFull);
  updateBinLevel(metalDistance, metalBinEmpty, metalBinHalf, metalBinFull);
  updateBinLevel(glassDistance, glassBinEmpty, glassBinHalf, glassBinFull);
  updateBinLevel(plasticDistance, plasticBinEmpty, plasticBinHalf, plasticBinFull);
}

// Function to open bin door (rotate servo 90 degrees)
void openBinDoor(Servo openServo, Servo pushServo) 
{
  // Open the door slowly
  for (int pos = openServo.read(); pos >= 45; pos -= 1) { 
    openServo.write(pos);              
    delay(15);                       // Adjust this delay for speed control
  }

  // Move the pushing servo slowly
  for (int pos = pushServo.read(); pos <= 135; pos += 1) {
    pushServo.write(pos);
    delay(15);                       // Adjust this delay for speed control
  }

  delay(4000);                         // Keep door open for 4 seconds

  // Close the opening door slowly
  for (int pos = openServo.read(); pos <= 90; pos += 1) { 
    openServo.write(pos);              
    delay(15);                       // Adjust this delay for speed control
  } 

  // Close the pushing door slowly
  for (int pos = pushServo.read(); pos >= 90; pos -= 1) {
    pushServo.write(pos);
    delay(15);                       // Adjust this delay for speed control
  }
}

// Function to read distance from an ultrasonic sensor
int readDistance(int trigPin, int echoPin) 
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(50);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  delayMicroseconds(50);
  if (duration >= 38000) 
  {
    return -1; // Indicate out of range
  } 
  else 
  {
    return duration / 148; // Calculate distance in inches
  }
}

// Function to update bin level LEDs
void updateBinLevel(int distance, int emptyPin, int halfPin, int fullPin)
{
  if (distance >= 20) {
    digitalWrite(emptyPin, HIGH);
    digitalWrite(halfPin, LOW);
    digitalWrite(fullPin, LOW);
  } else if (distance > 10) {
    digitalWrite(emptyPin, LOW);
    digitalWrite(halfPin, HIGH);
    digitalWrite(fullPin, LOW);
  } else {
    digitalWrite(emptyPin, LOW);
    digitalWrite(halfPin, LOW);
    digitalWrite(fullPin, HIGH);
  }
}

// Function for training the model with more images
void trainModel()
{
  Serial.println("TRAINING MODE!");
  do
  {
    if(digitalRead(A0) == LOW)
    {
      delay(100);
      while (!huskylens.writeLearn(1))  // Learn(Nothing ID1)
      {
        //Serial.println(F("Learn object Nothing:ID1 failed!")); 
        //delay(500);
      }
      Serial.println(F("Learn object Nothing:ID1 success")); 
      delay(300);
    }
    else if(digitalRead(A1) == LOW)
    {
      delay(100);
      while (!huskylens.writeLearn(2))  // Learn(Organic ID2)
      {
        //Serial.println(F("Learn object Organic:ID2 failed!"));
      }
      Serial.println(F("Learn object Organic:ID2 success"));
      delay(300); 
    }
    else if(digitalRead(A2) == LOW)
    {
      delay(100);
      while (!huskylens.writeLearn(3))  // Learn(Metal ID3)
      {
        //Serial.println(F("Learn object Metal:ID3 failed!"));
      }
      Serial.println(F("Learn object Metal:ID3 success"));
      delay(300); 
    }
    else if(digitalRead(A3) == LOW)
    {
      delay(100);
      while (!huskylens.writeLearn(4))  // Learn(Glass ID4)
      {
        //Serial.println(F("Learn object Glass:ID4 failed!"));
      }
      Serial.println(F("Learn object Glass:ID4 success")); 
      delay(300);
    }
    else if(digitalRead(A4) == LOW)
    {
      delay(100);
      while (!huskylens.writeLearn(5))  // Learn(Plastic ID5)
      {
        //Serial.println(F("Learn object Plastic:ID5 failed!"));
      }
      Serial.println(F("Learn object Plastic:ID5 success")); 
      delay(300);
    }
  }while(digitalRead(A6) == HIGH);
}
