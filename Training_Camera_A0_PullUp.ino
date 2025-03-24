#include "HUSKYLENS.h"

HUSKYLENS huskylens;

void setup() 
{
    Serial.begin(115200);
    pinMode(A0,INPUT_PULLUP);
    pinMode(A1,INPUT_PULLUP);
    pinMode(A2,INPUT_PULLUP);
    pinMode(A3,INPUT_PULLUP);
    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        delay(50);
    }
}

void loop() 
{
  if(digitalRead(A0) == 0)
  {
     while (!huskylens.writeLearn(1))  // bool writeLearn(int ID)
    {
      Serial.println(F("Learn object Trash:ID1 failed!")); 
      //delay(500);
    }
    Serial.println(F("Learn object Trash:ID1 success")); 
    //delay(500);
  }
  else if(digitalRead(A1) == 0)
  {
     while (!huskylens.writeLearn(2))  // bool writeLearn(int ID)
    {
      Serial.println(F("Learn object Plastic:ID2 failed!")); 
      //delay(500);
    }
    Serial.println(F("Learn object Plastic:ID2 success"));
    //delay(500); 
  }
  else if(digitalRead(A2) == 0)
  {
     while (!huskylens.writeLearn(3))  // bool writeLearn(int ID)
    {
      Serial.println(F("Learn object Metal:ID3 failed!")); 
      //delay(500);
    }
    Serial.println(F("Learn object Metal:ID3 success"));
    //delay(500); 
  }
  else if(digitalRead(A3) == 0)
  {
     while (!huskylens.writeLearn(4))  // bool writeLearn(int ID)
    {
      Serial.println(F("Learn object Glass:ID4 failed!")); 
      //delay(500);
    }
    Serial.println(F("Learn object Glass:ID4 success")); 
    //delay(500);
  }
}