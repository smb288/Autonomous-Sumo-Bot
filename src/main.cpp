// Autonomous Sumo Robot
// EE Senior Design
// Designed for Raspberry Pi Pico

#include <Arduino.h>                
#include "pico/multicore.h"         

#define objLeft 0
#define objMid 2
#define objRight 1

#define boundLeft 26
#define boundRight 28

#define lMotorFW 16
#define lMotorBW 17
#define rMotorFW 18
#define rMotorBW 19

#define speedControlL 20
#define speedControlR 21

// Determines max speed of motors
int m1Speed = 200*0.8;
int m2Speed = 200*0.8;

// Keeps track of the last seen object
// Default to 3 so the sumo-bot defaults to forward
int lastState = 3;

// Movement functions. Will make these so you can pass
// in different speeds at a later time. 
void fwDrive() {
  analogWrite(speedControlL, m1Speed);
  analogWrite(speedControlR, m2Speed);
  digitalWrite(lMotorFW,1);
  digitalWrite(rMotorFW,1);
  digitalWrite(lMotorBW,0);
  digitalWrite(rMotorBW,0);
}

void bwDrive() {
  analogWrite(speedControlL, m1Speed);
  analogWrite(speedControlR, m2Speed);
  digitalWrite(lMotorFW,0);
  digitalWrite(rMotorFW,0);
  digitalWrite(lMotorBW,1);
  digitalWrite(rMotorBW,1);
}

void lTurn() {
  analogWrite(speedControlL, 0);
  analogWrite(speedControlR, m2Speed);
  digitalWrite(lMotorFW,0);
  digitalWrite(rMotorFW,1);
  digitalWrite(lMotorBW,0);
  digitalWrite(rMotorBW,0);
}

void rTurn() {
  analogWrite(speedControlL, m1Speed);
  analogWrite(speedControlR, 0);
  digitalWrite(lMotorFW,1);
  digitalWrite(rMotorFW,0);
  digitalWrite(lMotorBW,0);
  digitalWrite(rMotorBW,0);
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
  int lBoundVal = analogRead(boundLeft);
  int rBoundVal = analogRead(boundRight);

  // Threshold for boundary detection sensors
  int boundThresh = 190;

  // Check if values are less than 200
  // if not, search for enemy
  if(lBoundVal < boundThresh) {
    bwDrive();
    delay(500);
    rTurn();
    delay(500);
  }
  else if(rBoundVal < boundThresh) {
    bwDrive();
    delay(500);
    lTurn();
    delay(500);
  }
  // Determines the direction the car
  // should move based on what sensors are high
  else {
    int dir = getDir();
    switch(dir) {
      case 1:
        while(digitalRead(objMid)) {
          lBoundVal = analogRead(boundLeft);
          rBoundVal = analogRead(boundRight);
          lastState = 1;
          if(lBoundVal < boundThresh || rBoundVal < boundThresh)
            break;
          lTurn();
        }
      break;
      case 2:
        lTurn();
        lastState = 2;
      break;
      case 3:
        fwDrive();
        lastState = 3;
      break;
      case 4:
        rTurn();
        lastState = 4;
      break;
      case 5:
        while(digitalRead(objMid)) {
          lBoundVal = analogRead(boundLeft);
          rBoundVal = analogRead(boundRight);
          lastState = 5;
          if(lBoundVal < boundThresh || rBoundVal < boundThresh)
            break;
          rTurn();
        }
      break;
      // If sensors aren't seeing anything,
      // turn in the direction the object was
      // last seen.
      default:
        if(lastState == 1)
          lTurn();
        else if(lastState == 5)
          rTurn();
        else if(lastState == 3)
          fwDrive();
        else
          fwDrive();
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
  
  Serial.begin(115200);
}


// Loops the boundary check function. Will expand on
// this in the future.
void loop() {
  boundCheck();
}