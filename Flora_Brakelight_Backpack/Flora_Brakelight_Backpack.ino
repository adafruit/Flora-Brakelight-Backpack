#include <Wire.h>
#include <Adafruit_LSM303.h>

Adafruit_LSM303 lsm;

#define BRAKETHRESHOLD        350
#define BRAKETIMETHRESHOLD    200

int start = 0;

int prevX = 0;
int currentX = 0;

int speakerPin = 11;
long brakeTime = 0;

void setup() 
{
  Serial.begin(9600);
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while (1);
  }
  pinMode(speakerPin, OUTPUT);
}

void loop() 
{
  lsm.read();
  currentX = abs(lsm.accelData.x);
  
  if (start == 0){
    Serial.println("In start");
    prevX = currentX;
    start = 1;
  }
  
  int i = currentX - prevX;
  Serial.print("Outer Loop: ");
  Serial.println(i);
        //Serial.print("currentx: ");
      //Serial.println(currentX);
      //Serial.print("prevX: ");
       //Serial.println(prevX);
  if (abs(i) > BRAKETHRESHOLD) {
    Serial.print("In outside IF: ");
    Serial.println(abs(i));
    brakeTime = millis();
    int strikes = 0;
    while ((abs(i) > BRAKETHRESHOLD) && (strikes < 3)) {
      //Serial.println("In While");
      if (abs(i) < BRAKETHRESHOLD) {
        strikes = strikes + 1; 
      }
      //Serial.print("strikes: ");
      //Serial.println(strikes);
      //Serial.println(millis() - brakeTime);
      //Serial.println("After Break IF");
      lsm.read();
      currentX = abs(lsm.accelData.z);

     
      i = currentX - prevX;
      //prevX = currentX;
      //Serial.println(abs(i));
      
      if ((millis() - brakeTime) > BRAKETIMETHRESHOLD) {
        //Serial.print("Calculated time: ");
        //Serial.println(millis() - brakeTime);
        analogWrite(speakerPin, 100);
        while (abs(i) > BRAKETHRESHOLD) {
          lsm.read();
          currentX = abs(lsm.accelData.x);
          i = currentX - prevX;
          Serial.print("In Inner While: ");
          Serial.println(i);
          delay(100);
        }
        delay(2000);
        analogWrite(speakerPin, 0);
        brakeTime = millis();
        i = 0;
                  lsm.read();
          currentX = abs(lsm.accelData.x);
      }
    }
  }

  //brakeTime = millis();
  prevX = currentX;
  delay(200);
}
