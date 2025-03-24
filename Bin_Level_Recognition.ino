#include <Servo.h>

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
const int organicBinEmpty = 22;   // Green LED for empty
const int organicBinHalf  = 24;   // Yellow LED for almost full
const int organicBinFull  = 26;   // Red LED for full

const int plasticBinEmpty = 28;   // Green LED for empty
const int plasticBinHalf  = 30;   // Yellow LED for almost full
const int plasticBinFull  = 32;   // Red LED for full

const int metalBinEmpty = 23;     // Green LED for empty
const int metalBinHalf  = 25;     // Yellow LED for almost full
const int metalBinFull  = 27;     // Red LED for full

const int glassBinEmpty = 29;     // Green LED for empty
const int glassBinHalf  = 31;     // Yellow LED for almost full
const int glassBinFull  = 33;     // Red LED for full

void setup() 
{   
  Serial.begin(9600);   
  
  // Initialize servo motors
  organicServo.attach(organicServoPin, 500, 2500);  // Set PWM range
  metalServo.attach(metalServoPin, 500, 2500);      // Set PWM range
  glassServo.attach(glassServoPin, 500, 2500);      // Set PWM range
  plasticServo.attach(plasticServoPin, 500, 2500);  // Set PWM range


  // Set trigger pin as output
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  // Set echo pins as input
  pinMode(organicEcho, INPUT);
  pinMode(plasticEcho, INPUT);
  pinMode(glassEcho, INPUT);
  pinMode(metalEcho, INPUT);

  // Set LED pins as output
  pinMode(organicBinEmpty, OUTPUT);
  pinMode(organicBinHalf, OUTPUT);
  pinMode(organicBinFull, OUTPUT);

  pinMode(plasticBinEmpty, OUTPUT);
  pinMode(plasticBinHalf, OUTPUT);
  pinMode(plasticBinFull, OUTPUT);

  pinMode(metalBinEmpty, OUTPUT);
  pinMode(metalBinHalf, OUTPUT);
  pinMode(metalBinFull, OUTPUT);

  pinMode(glassBinEmpty, OUTPUT);
  pinMode(glassBinHalf, OUTPUT);
  pinMode(glassBinFull, OUTPUT);

  delay(6000);
  Serial.println("Distance:"); 
}  
void loop() 
{   
  // Check for user input from serial monitor
  if (Serial.available() > 0) {
    String objectType = Serial.readStringUntil('\n');
    objectType.trim(); // Remove leading/trailing whitespace

    // Control servos based on user input
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
  // Collect the echo time for each bin for future distance/level calculation
  digitalWrite(trig, HIGH);       
  delayMicroseconds(50);
  digitalWrite(trig, LOW);       
  organicDuration = pulseIn(organicEcho, HIGH);
  delayMicroseconds(50);
  digitalWrite(trig, HIGH);       
  delayMicroseconds(50);
  digitalWrite(trig, LOW); 
  plasticDuration = pulseIn(plasticEcho, HIGH);
  delayMicroseconds(50);
  digitalWrite(trig, HIGH);       
  delayMicroseconds(50);
  digitalWrite(trig, LOW);  
  glassDuration   = pulseIn(glassEcho, HIGH);
  delayMicroseconds(50);
  digitalWrite(trig, HIGH);       
  delayMicroseconds(50);
  digitalWrite(trig, LOW);  
  metalDuration   = pulseIn(metalEcho, HIGH);


  if((organicDistance || plasticDistance || glassDistance || metalDistance) >= 38000)
  {       
    Serial.print("Out range");         
  }      
  else
  {       
    //Calculate distance for each bin level in centimeters
    organicDistance = organicDuration/58;          
    Serial.print(organicDistance);           
    Serial.print("cm ");
    plasticDistance = plasticDuration/58;      
    //Serial.print("\t");       
    Serial.print(plasticDistance);        
    Serial.println("cm ");
    glassDistance = glassDuration/58;
    //Serial.print("\t");       
    Serial.print(glassDistance);        
    Serial.println("cm ");
    metalDistance = metalDuration/58;
    //Serial.print("\t");       
    Serial.print(metalDistance);        
    Serial.println("cm ");
    Serial.print("\t");

    if(organicDistance >= 50.00)
    {
      digitalWrite(organicBinEmpty, HIGH);  // Turn on empty LED
      digitalWrite(organicBinHalf, LOW);    // Make sure others are off
      digitalWrite(organicBinFull, LOW);
    }
    else if(organicDistance > 10.00)
    {
      digitalWrite(organicBinEmpty, LOW);
      digitalWrite(organicBinHalf, HIGH);   // Turn on half LED
      digitalWrite(organicBinFull, LOW);
    } 
    else 
    {
      digitalWrite(organicBinEmpty, LOW);
      digitalWrite(organicBinHalf, LOW);
      digitalWrite(organicBinFull, HIGH);   // Turn on full LED
    }

    if(plasticDistance >= 50.00)
    {
      digitalWrite(plasticBinEmpty, HIGH);  // Turn on empty LED
      digitalWrite(plasticBinHalf, LOW);    // Make sure others are off
      digitalWrite(plasticBinFull, LOW);
    }
    else if(plasticDistance > 10.00)
    {
      digitalWrite(plasticBinEmpty, LOW);   // Turn on empty LED
      digitalWrite(plasticBinHalf, HIGH);   // Make sure others are off
      digitalWrite(plasticBinFull, LOW);
    }
    else
    {
      digitalWrite(organicBinEmpty, LOW);
      digitalWrite(organicBinHalf, LOW);
      digitalWrite(organicBinFull, HIGH);   // Turn on full LED
    }
    if(glassDistance >= 50.00)
    {
      digitalWrite(glassBinEmpty, HIGH);  // Turn on empty LED
      digitalWrite(glassBinHalf, LOW);
      digitalWrite(glassBinFull, LOW); 
    }
    else if(glassDistance > 10.00)
    {
      digitalWrite(glassBinEmpty, LOW);
      digitalWrite(glassBinHalf, HIGH);   // Turn on half LED
      digitalWrite(glassBinFull, LOW); 
    }
    else
    {
      digitalWrite(glassBinEmpty, LOW);
      digitalWrite(glassBinHalf, LOW);
      digitalWrite(glassBinFull, HIGH);   // Turn on full LED
    }
    if(metalDistance >= 50.00)
    {
      digitalWrite(metalBinEmpty, HIGH);  // Turn on empty LED
      digitalWrite(metalBinHalf, LOW);   
      digitalWrite(metalBinFull, LOW);   
    }
    else if(metalDistance > 10.00)
    {
      digitalWrite(metalBinEmpty, LOW);
      digitalWrite(metalBinHalf, HIGH);   // Turn on half LED
      digitalWrite(metalBinFull, LOW);   
    }
    else
    {
      digitalWrite(metalBinEmpty, LOW);
      digitalWrite(metalBinHalf, LOW);
      digitalWrite(metalBinFull, HIGH);   // Turn on full LED
    }
  }
  
  delay(1500);    
}  

// Function to open bin door (rotate servo 90 degrees)
void openBinDoor(Servo servo) 
{
  servo.write(90);
  delay(1000); // Keep door open for 1 second (adjust as needed)
  servo.write(0);  // Close the door
}