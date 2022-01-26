#include <Arduino.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setting up the pins for the ultrasonic sensors
/*
#define echoPin1 0 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin1 1 //attach pin D3 Arduino to pin Trig of HC-SR04
#define echoPin2 2
#define trigPin2 3
#define echoPin3 4
#define trigPin3 5
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Below are the pins for the new IR sensors object detection
#define frontLeftSensor 0
#define frontMiddleSensor 1
#define frontRightSensor 2
/////////////////////////////////////////////////////////
#define IRFrontCenter 6
#define IRBackCenter 7
#define IRFrontLeft 8
#define IRFrontRight 9
#define leftMotorFW 16
#define leftMotorBW 17
#define rightMotorBW 18
#define rightMotorFW 19
#define LED 25
#define speedControlL 26
#define speedControlR 27

long duration; // variable for the duration of sound wave travel
int distanceM, distanceL, distanceR; // variable for the distance measurement
int m1Speed = 250;
int m2Speed = 255;
int m1Slow = 100;
int m2Slow = 100;
bool FOUNDMID = false;
bool FOUNDLEFT = false;
bool FOUNDRIGHT = false;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP LOOP
void setup() {
  //We do not need this block to set the different pins for the ultrasonic sensors
  /*
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin1, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin3, INPUT);
  */
 //////////////////////////////////////////////////////////////
 
  //Below is the code for the new IR Sensors to set them up
  pinMode(frontLeftSensor, INPUT_PULLUP);
  pinMode(frontMiddleSensor, INPUT_PULLUP);
  pinMode(frontRightSensor, INPUT_PULLUP);
  ///////////////////////////////////////////////////////////
  pinMode(IRFrontCenter, INPUT);
  pinMode(IRBackCenter, INPUT);
  pinMode(IRFrontLeft, INPUT);
  pinMode(IRFrontRight, INPUT);
  pinMode(leftMotorFW, OUTPUT);
  pinMode(leftMotorBW, OUTPUT);
  pinMode(rightMotorBW, OUTPUT);
  pinMode(rightMotorFW, OUTPUT);
  pinMode(speedControlL, OUTPUT);
  pinMode(speedControlR, OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//this is to initialize the ultrasonic sensors which we are phasing out
/*
int pulseUltra(int echo, int trig) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  // Calculating the distance in cm
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  return distance;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//movement functions to move the car
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This portion was used to trigger the ultrasonic sensors
/*
void trigAll() {
    distanceM = pulseUltra(echoPin1, trigPin1);
    distanceL = pulseUltra(echoPin2, trigPin2);
    distanceR = pulseUltra(echoPin3, trigPin3);
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void blinkLED(){
  digitalWrite(25, 1);
  delay(1000);
  digitalWrite(25, 0);
  delay(1000);
  digitalWrite(25, 1);
  delay(1000);
  digitalWrite(25, 0);
  delay(1000);
  digitalWrite(25, 1);
  delay(1000);
}
 
int tooClose = 20;

void loop() {
  //robot car is delaying for 5 seconds with builtin LED blinking for 5s
  //blinkLED();
  //Do not need to trigger all ultrasonic sensors
  //trigAll();
  //////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //Read all the front sensors
  int frontLeft = digitalRead(frontLeftSensor);
  int frontMiddle = digitalRead(frontMiddleSensor);
  int frontRight = digitalRead(frontRightSensor);
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  //Getting the boolean Values from the IR sensors for boundary detection.
  bool frontLeftIR = digitalRead(IRFrontLeft);
  bool frontMiddleIR = digitalRead(IRFrontCenter);
  bool frontRightIR = digitalRead(IRFrontRight);
  bool backIR = digitalRead(IRBackCenter);

  /*
  //Serial.println(distanceR);
  bool mStatus, rStatus, lStatus;
  if(distanceM < 25) mStatus = true;
  else mStatus = false;
  if(distanceR < 15) rStatus = true;
  else rStatus = false;
  if(distanceL < 15) lStatus = true;
  else lStatus = false;
  
  if(!mStatus && !rStatus) {
  forwardright();
  //Serial.println("Forward right");
  }
  else if(mStatus && !rStatus) {
    left();
    delay(500);
  }
  else if((!mStatus && rStatus)  || (distanceR < 10))  {
    forwardleft();
    //Serial.println("Forward left");
  }
  
  else if(mStatus && rStatus) {
    left();
    delay(600);
  }
  if(digitalRead(IRFrontCenter) && digitalRead(IRFrontLeft) && digitalRead(IRFrontRight)) {
    left();
    delay(1500);
    while(true) {
      trigAll();
      
      if(distanceM < 20) {
        left();
        delay(500);
      }
      else if(distanceL < 20) {
        forwardl();
      }
      else if(distanceL < 20) {
        forwardright();
      }
      else forward();
    }
    if(distanceM < 25) mStatus = true;
    else mStatus = false;
    if(distanceR < 15) rStatus = true;
    else rStatus = false;
    if(distanceL < 15) lStatus = true;
    else lStatus = false;
    
    if(!mStatus && !lStatus) {
    forwardleft();
    }
    else if(mStatus && !lStatus) {
      right();
      delay(400);
    }
    else if((!mStatus && lStatus)  || (distanceL < 10))  {
      fasterright();
    }
    
    else if(mStatus && lStatus) {
      right();
      delay(400);
    }
  }
  }
  */

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //This is the algorithm for the Hallway traversal. "Hug the wall" Strat
  //For the boundary detection subsystem, 0 = white and 1 = black
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
      right();
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
  //////////////////////////////////////////////////////////////////////////////////////////////////////////

}