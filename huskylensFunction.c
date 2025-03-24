//Guide: https://www.the-diy-life.com/adding-ai-vision-to-a-robot-car-using-a-huskylens/


//Import all the needed libraries, including:
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "PIDLoop.h"
#include (The driver for the door motors)


//create the objects for motors
(
Motor1;
Motor2;
Motor3;
Motor4;
)

//define the properties of motors
(
    //sample code form the tracking robot example
    byte motorSpeed = 60;                             //The maximum motor speed
int motorOffset = 15;                             //Factor to account for one side being more powerful
int turnSpeed = 50;                               //Amount to add to motor speed when turning
int leftSpeed = 0;                                //Variables to keep current left and right motor speeds
int rightSpeed = 0;
)

PIDLoop headingLoop(120, 0, 0, false);            //Set up PID control for the heading, only P used for now
HUSKYLENS huskylens;                              //Create a Huskeylens object
int ID1 = 1(100 - 199);                                      //We're tracking recognised object 1
int ID2 = 2(200 - 299);
int ID3 = 3(300 - 399);
int ID4 = 0;                                      //Reject, because does not fit 

void setup() //Main loop, taken as example form the tracking robot, needs to be edited
{
    Serial.begin(115200);                                         //Start serial communication
    Wire.begin();                                                 //Begin communication with the Huskeylens
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
    huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING);            //Switch the algorithm to object tracking.
    glass.setSpeed(0);                                        //Set the motors to the motor speed, initially all 0
    plastic.setSpeed(0);
    cardboard.setSpeed(0);
    garbage.setSpeed(0);
   while (object detected)
       open();							//Algorithm to operate
       close();
}

void open() //function to open the gate
{
    if (ID == (100 - 199))
    {
        operate1();
    }
    else if (ID == (200 - 299))
    {
        operate2();
    }
    else if (ID == (300 - 399))
    {
        operate3();
    }
    else if (ID != (100-399))
    {
        operate4();
    }
}

void close()
{
    if (ID == (100 - 199))
    {
        complete1();
    }
    else if (ID == (200 - 299))
    {
        complete2();
    }
    else if (ID == (300 - 399))
    {
        complete3();
    }
    else if (ID != (100 - 399))
    {
        complete4();
    }
}

void stopMove()                                   //Set all motors to stop
{
    glass.run(RELEASE);
    plastic.run(RELEASE);
    cardboard.run(RELEASE);
    garbage.run(RELEASE);
}




