// Autonomous Sumo Robot
// EE Senior Design
// Designed for Raspberry Pi Pico

#include <Arduino.h>                
#include "pico/multicore.h"         

// Front facing IR sensors
// for object detection
#define objLeft 2
#define objMid 1
#define objRight 0

// Bottom mounted IR sensors
// for boundary detection
#define boundLeft 27
#define boundRight 28


// Pins for motor driver
#define lMotorFW 16
#define lMotorBW 17
#define rMotorFW 18
#define rMotorBW 19
#define speedControlL 20
#define speedControlR 21


// Keeps track of the last seen object
// Default to 3 so the sumo-bot defaults to forward
int lastState = 3;

// Bool value that changes to true after 5 seconds
bool start = false;

// Boundary detection sensors sample to these arrays
int lBoundSamples[100];
int rBoundSamples[100];
byte arrayIndex = 0;


// Average and sum of each boundary array
int lAvg, rAvg;
int lSum, rSum;
int lThresh = 0;
int rThresh = 0;


// Keeps track of time in start sequence
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long startPeriod = 5000;


int lBoundVal;
int rBoundVal;

// Movement functions. Will make these so you can pass
// in different speeds at a later time. 
void fwDrive(int m1Speed, int m2Speed) {
  analogWrite(speedControlL, m1Speed);
  analogWrite(speedControlR, m2Speed);
  digitalWrite(lMotorFW,1);
  digitalWrite(rMotorFW,1);
  digitalWrite(lMotorBW,0);
  digitalWrite(rMotorBW,0);
}

void bwDrive(int m1Speed, int m2Speed) {
  analogWrite(speedControlL, m1Speed);
  analogWrite(speedControlR, m2Speed);
  digitalWrite(lMotorFW,0);
  digitalWrite(rMotorFW,0);
  digitalWrite(lMotorBW,1);
  digitalWrite(rMotorBW,1);
}

void lTurn(int m2Speed) {
  analogWrite(speedControlL, 0);
  analogWrite(speedControlR, m2Speed);
  digitalWrite(lMotorFW,0);
  digitalWrite(rMotorFW,1);
  digitalWrite(lMotorBW,0);
  digitalWrite(rMotorBW,0);
}

void rTurn(int m1Speed) {
  analogWrite(speedControlL, m1Speed);
  analogWrite(speedControlR, 0);
  digitalWrite(lMotorFW,1);
  digitalWrite(rMotorFW,0);
  digitalWrite(lMotorBW,0);
  digitalWrite(rMotorBW,0);
}

void mStop() {
  analogWrite(speedControlL, 0);
  analogWrite(speedControlR, 0);
  digitalWrite(lMotorFW,0);
  digitalWrite(rMotorFW,0);
  digitalWrite(lMotorBW,0);
  digitalWrite(rMotorBW,0);
}


// Reads from boundary sensors
int readQD(int boundPin){
  //Returns value from the QRE1113 
  //Lower numbers mean more refleacive
  //More than 3000 means nothing was reflected.
  pinMode( boundPin, OUTPUT );
  digitalWrite( boundPin, HIGH );  
  delayMicroseconds(10);
  pinMode( boundPin, INPUT );

  long time = micros();

  //time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(boundPin) == HIGH && micros() - time < 3000); 
  int diff = micros() - time;

  return diff;
}


/*******************************************************
* Determines the direction the sumo-bot should turn
*
*  1 -> Left
*  2 -> Forward Left
*  3 -> Forward
*  4 -> Forward Right
*  5 -> Right
*  Anything else -> 6
*
*  May add additional states in the future, but these
*  5 are the most important right now.
*******************************************************/
int getDir() {
  int dir;
  bool l = !digitalRead(objLeft);
  bool m = !digitalRead(objMid);
  bool r = !digitalRead(objRight);
  
       if(l && !m && !r) dir = 1;
  else if(l && m && !r)  dir = 2;
  else if(!l && m && !r) dir = 3;
  else if(!l && m && r)  dir = 4;
  else if(!l && !m && r) dir = 5;
  else dir = 6;

  return dir;
}


void boundCheck() {
  // Read in values from boundary sensors
  lBoundVal = readQD(boundLeft);
  rBoundVal = readQD(boundRight);

  // Threshold for boundary detection sensors
  int lBoundThresh = 3000;
  int rBoundThresh = 3000;
  int low = 95;
  int high = 125;

  // Check if values are less than 200
  // if not, search for enemy
  if(rBoundVal < rBoundThresh) {
    bwDrive(170,170);
    delay(500);
    lTurn(170);
    delay(500);
    lastState = 3;
  }
  else if(lBoundVal < lBoundThresh) {

    bwDrive(170,170);
    delay(500);
    rTurn(170);
    delay(500);
    lastState = 3;
  }
  // Determines the direction the car
  // should move based on what sensors are high
  //Original low 130 and high 150
  
  else {
    int dir = getDir();
    switch(dir) {
      case 1:
        while(digitalRead(objMid)) {
          lBoundVal = readQD(boundLeft);
          rBoundVal = readQD(boundRight);
          lastState = 1;
          if(lBoundVal < lBoundThresh || rBoundVal < rBoundThresh)
            break;
          lTurn(high);
        }
      break;
      case 2:
        fwDrive(low,high);
        lastState = 2;
      break;
      case 3:
        fwDrive(high,high);
        lastState = 3;
      break;
      case 4:
        fwDrive(high,low);
        lastState = 4;
      break;
      case 5:
        while(digitalRead(objMid)) {
          lBoundVal = readQD(boundLeft);
          rBoundVal = readQD(boundRight);
          lastState = 5;
          if(lBoundVal < lBoundThresh || rBoundVal < rBoundThresh)
            break;
          rTurn(high);
        }
      break;
      // If sensors aren't seeing anything,
      // turn in the direction the object was
      // last seen.
      default:
        if(lastState == 1)
          lTurn(high);
        else if(lastState == 5)
          rTurn(high);
        else if(lastState == 3)
          fwDrive(high,high);
        else
          fwDrive(high,high);
      break;
    }
  }
}

// Initializes all of the pins
void setup() {
  pinMode(objLeft, INPUT);
  pinMode(objMid, INPUT);
  pinMode(objRight, INPUT);

  pinMode(boundLeft, INPUT);
  pinMode(boundRight, INPUT);

  pinMode(lMotorBW, OUTPUT);
  pinMode(lMotorFW, OUTPUT);
  pinMode(rMotorBW, OUTPUT);
  pinMode(rMotorFW, OUTPUT);
  pinMode(speedControlL, OUTPUT);
  pinMode(speedControlR, OUTPUT);

  // Start at a high value so
  // car doesn't start backing up
  // at start
  lBoundVal = 3010;
  rBoundVal = 3010;
  
  Serial.begin(115200);

  // Get time from power on
  startMillis = millis();
}


// Stops the car from moving for 5 seconds
// During this time, read from the boundary
// sensors to get a value for the field
void startSeq() {
  if(!start) {
    mStop();
    if(arrayIndex < 100) {
      int lSample = analogRead(boundLeft);
      int rSample = analogRead(boundRight);
      lBoundSamples[arrayIndex] = lSample;
      rBoundSamples[arrayIndex] = rSample;
      arrayIndex++;
    }
    
    if(arrayIndex == 99) {
      for(int i = 0; i < 100; i++) {
        lSum += lBoundSamples[i];
        rSum += rBoundSamples[i];
      }
      lAvg = lSum / 100;
      rAvg = rSum / 100;
    } 

    currentMillis = millis();
    if (currentMillis - startMillis >= startPeriod)
    {
      startMillis = currentMillis;
      Serial.print("L Avg: ");
      Serial.print(lAvg);
      Serial.print("R Avg: ");
      Serial.print(rAvg);
      start = true;
    }
  }
}

// Loops the boundary check function. Will expand on
// this in the future.
void loop() {
  //startSeq();
  boundCheck();
}