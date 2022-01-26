#include <Arduino.h>

#define echoPin1 0 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin1 1 //attach pin D3 Arduino to pin Trig of HC-SR04
#define echoPin2 2
#define trigPin2 3
#define echoPin3 4
#define trigPin3 5
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
void setup() {
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin1, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin3, INPUT);
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

void trigAll() {
    distanceM = pulseUltra(echoPin1, trigPin1);
    distanceL = pulseUltra(echoPin2, trigPin2);
    distanceR = pulseUltra(echoPin3, trigPin3);
}

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
  trigAll();

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
        /*
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
      }*/
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
}

//This is a comment to test my code push and pull requests
