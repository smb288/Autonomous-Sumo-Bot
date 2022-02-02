/*
Author: Ringtarih Tamfu
Coauthors: Sean Baker
Project: Sumo Bot
Organization: Texas State University

Description: The purpose of this code to run all the processes of a Raspberry Pi Pico with other peripherals for the sumo bot that we
             are designing for our Senior Design Project specifically for the HALLWAY TRAVERSAL
             All statements are documented to show you the flow of the program,
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*LIBRARY BLOCK*/
/*DESCRIPTION: This block contains all the libraries we used in this program.*/
#include <Arduino.h>                //This library allows us to use the Arduino format of coding the Raspberry Pi Pico
#include "pico/multicore.h"         //This library will allow us both cores of the Raspberry Pi Pico
/*END OF LIBRARY BLOCK*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*INITIALIZE BLOCK*/
/*DESCRIPTION: This block contains all the pins used in the Pico for every peripheral we used in this project*/
//Below are the pins used for the new IR sensors for object detection
#define frontLeftSensor 0
#define frontMiddleSensor 1
#define frontRightSensor 2
//Below are the pins used for the IR sensors for boundary detection
#define IRFrontCenter 6
#define IRBackCenter 7
#define IRFrontLeft 8
#define IRFrontRight 9
//Below are the pins used to control the speeds of the TT motors
#define leftMotorFW 16
#define leftMotorBW 17
#define rightMotorBW 18
#define rightMotorFW 19
#define speedControlL 26
#define speedControlR 27
/*END OF INITIALIZE BLOCK*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*GLOBAL VARIABLE BLOCK*/
/*DESCRIPTION: This block contains all the variable primarily used for the speed control (RPM) values of the TT motors*/
int m1Speed = 250;
int m2Speed = 255;
int m1Slow = 100;
int m2Slow = 100;
/*END OF VARIABLE BLOCK*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*MOVEMENT BLOCK*/
/*DESCRIPTION: This block defines the controls sent to the motor driver to set the RPM of the wheels of the sumo bot. Each function is
               named to match exactly what the sumo bot should do.*/
void forward(){
  analogWrite(speedControlL,110);
  analogWrite(speedControlR,115);
  digitalWrite(leftMotorFW, 1);
  digitalWrite(leftMotorBW, 0);
  digitalWrite(rightMotorBW, 0);
  digitalWrite(rightMotorFW, 1);
}

void back(){
  analogWrite(speedControlL,200);
  analogWrite(speedControlR,200);
  digitalWrite(leftMotorFW, 0);
  digitalWrite(leftMotorBW, 1);
  digitalWrite(rightMotorBW, 1);
  digitalWrite(rightMotorFW, 0);
}

void right(){
  analogWrite(speedControlL,100);
  analogWrite(speedControlR,200);
  digitalWrite(leftMotorFW, 1);
  digitalWrite(leftMotorBW, 0);
  digitalWrite(rightMotorBW, 0);
  digitalWrite(rightMotorFW, 0);
}

void left(){
  analogWrite(speedControlL,0);
  analogWrite(speedControlR,100);
  digitalWrite(leftMotorFW, 0);
  digitalWrite(leftMotorBW, 0);
  digitalWrite(rightMotorBW, 0);
  digitalWrite(rightMotorFW, 1);
}

void backright(){
  analogWrite(speedControlL,200);
  analogWrite(speedControlR,200);
  digitalWrite(leftMotorFW, 0);
  digitalWrite(leftMotorBW, 1);
  digitalWrite(rightMotorBW, 1);
  digitalWrite(rightMotorFW, 0);
}

void backleft(){
  analogWrite(speedControlL,200);
  analogWrite(speedControlR,200);
  digitalWrite(leftMotorFW, 0);
  digitalWrite(leftMotorBW, 1);
  digitalWrite(rightMotorBW, 1);
  digitalWrite(rightMotorFW, 0);
  
}

void forwardright(){
  analogWrite(speedControlL,110);
  analogWrite(speedControlR,90);
  digitalWrite(leftMotorFW, 1);
  digitalWrite(leftMotorBW, 0);
  digitalWrite(rightMotorBW, 0);
  digitalWrite(rightMotorFW, 1);
}

void fasterright(){
  analogWrite(speedControlL,125);
  analogWrite(speedControlR,80);
  digitalWrite(leftMotorFW, 1);
  digitalWrite(leftMotorBW, 0);
  digitalWrite(rightMotorBW, 0);
  digitalWrite(rightMotorFW, 1);
}

void forwardleft(){
  analogWrite(speedControlL,90);
  analogWrite(speedControlR,150);
  digitalWrite(leftMotorFW, 1);
  digitalWrite(leftMotorBW, 0);
  digitalWrite(rightMotorBW, 0);
  digitalWrite(rightMotorFW, 1);
}

void stop() {
  back();
  delay(10);
  digitalWrite(leftMotorFW, 0);
  digitalWrite(leftMotorBW, 0);
  digitalWrite(rightMotorBW, 0);
  digitalWrite(rightMotorFW, 0);
}
/*END OF MOVEMENT BLOCK*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*SETUP BLOCK*/
/*DESCRIPTION: This block is used to set the modes of each pin that we initialize in the INITIALIZE BLOCK above*/
void setup() {
  //Below we set the input of the E18-D80NK IR sensors
  pinMode(frontLeftSensor, INPUT_PULLUP);
  pinMode(frontMiddleSensor, INPUT_PULLUP);
  pinMode(frontRightSensor, INPUT_PULLUP);
  //Below we set the input of the Arduino IR sensors 
  pinMode(IRFrontCenter, INPUT);
  pinMode(IRBackCenter, INPUT);
  pinMode(IRFrontLeft, INPUT);
  pinMode(IRFrontRight, INPUT);
  //Below we set the input of the TT motors through the L298N Motor driver
  pinMode(leftMotorFW, OUTPUT);
  pinMode(leftMotorBW, OUTPUT);
  pinMode(rightMotorBW, OUTPUT);
  pinMode(rightMotorFW, OUTPUT);
  pinMode(speedControlL, OUTPUT);
  pinMode(speedControlR, OUTPUT);
  
  Serial.begin(115200);
  /*END OF SETUP BLOCK*/
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*LOOP BLOCK*/
/*DESCRIPTION: This block is the main block where we run our main algorithm. In this block specifically, we will be running the 
               hallway traversal code. */
void loop() {
  //Reading from all the E18-D80NK IR sensors for object detection
  int frontLeft = digitalRead(frontLeftSensor);
  int frontMiddle = digitalRead(frontMiddleSensor);
  int frontRight = digitalRead(frontRightSensor);

  //Reading from all the Arduino IR sensors for boundary detection.
  bool frontLeftIR = digitalRead(IRFrontLeft);
  bool frontMiddleIR = digitalRead(IRFrontCenter);
  bool frontRightIR = digitalRead(IRFrontRight);
  bool backIR = digitalRead(IRBackCenter);

  //Main algorithm for the Hallway traversal. "Hug the wall" strategy from the middle of the hallway. 
  /*DESCRIPTION: This strategy works by using the boundary detection sensors to make sure we are on a white surface while going down 
                 the hallway and checking if the robot finds an object (wall) and goes away from the wall. 
                 When the robot reaches the black border at the end of the hallway, they will turn and make a U-turn.*/
  //For the boundary detection subsystem, 0 = white surface and 1 = black surface
  //For the object detection subsystem, 0 = object detected and 1 = clear
  if(frontLeftIR == 0 && frontMiddleIR == 0 && frontRightIR == 0 && backIR == 0){
    if(frontLeftSensor == 1 && frontMiddleSensor == 1 && frontRightSensor == 1){
      forward();
    } 
    else if(frontLeftSensor == 1 && frontMiddleSensor == 1 && frontRightSensor == 0){
      left();
    }
    else if(frontLeftSensor == 1 && frontMiddleSensor == 0 && frontRightSensor == 1){
      forwardleft();
    }
    else if(frontLeftSensor == 1 && frontMiddleSensor == 0 && frontRightSensor == 0){
      forwardleft();
    }
    else if(frontLeftSensor == 0 && frontMiddleSensor == 1 && frontRightSensor == 1){
      forwardright();
    }
    else if(frontLeftSensor == 0 && frontMiddleSensor == 1 && frontRightSensor == 0){
      forward();
    }
    else if(frontLeftSensor == 0 && frontMiddleSensor == 0 && frontRightSensor == 1){
      forwardright();
    }
    else if(frontLeftSensor == 0 && frontMiddleSensor == 0 && frontRightSensor == 0){
      back();
      delay(500);
    }
  }
  else {
    left();
    delay(500);
  }
  /*END OF LOOP BLOCK*/
  //////////////////////////////////////////////////////////////////////////////////////////////////////////

}