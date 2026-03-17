 /********************************************************
   M103Lab3MiniBotFwdBack 
   B.W. Surgenor, 04/03/2025

  This program is used to calibrate the speed of a MiniBot.
  Robot goes forward, pauses, then reverses.
  Change delta to change the speed of your robot. 
*********************************************************/

#include <Servo.h>
#include <Arduino.h>
#include "helpers.h"

Servo leftWheel;
Servo rightWheel;

// Pin Assignments  
#define RED 10           // red LED Pin
#define GRN 9            // green LED Pin
#define YLW 5            // yellow LED Pin
int BUTTON = 7;         // pushbutton Pin
int MOTOR_R = 3;        // right motor signal pin
int MOTOR_L = 4;        // left motor signal pin

// Delta = speed above (+) or below (-) stop speed (must be positive)
const int delta = 7;       // pulse differential (default = 15)
const int offset = -2;       // offset, slows left wheel (default = 0)

//added variables
#define MAX_DELTA 15

int error;
int deltaL = 0;
int deltaR = 0;                                  

const int LSENSOR = A1;
const int RSENSOR = A2;
const int SHARP = A3;

int lastError = 0;

int lvalue = 0;  //left sensor value
int rvalue = 0;  //right sensor value
int distance = 0; //SHARP sensor value

const float Kp = 0.0035;
const float Kd = 0.0000045;

// Set-up Routine
void setup() {
  // Initialize LED pins as outputs.
    pinMode(RED, OUTPUT);
    pinMode(GRN, OUTPUT);
    pinMode(YLW, OUTPUT);

  // Initialize button pins as inputs
    pinMode(BUTTON, INPUT);

  //initialize motor control pins as servo pins
    leftWheel.attach(MOTOR_L);
    rightWheel.attach(MOTOR_R);

  while(digitalRead(BUTTON) == LOW){
    delay(125);
    runMotors(deltaL, deltaR);
  }
}

// Main Routine
void loop() {

  lvalue = map(analogRead(LSENSOR), 0, 1023, 0, 3000);
  rvalue = map(analogRead(RSENSOR), 0, 1023, 0, 3000);
  distance = map(analogRead(SHARP), 0, 1023, 0, 3300);

  error = lvalue - rvalue;
  
  float derivative = error - lastError;
  float correction = (Kp * error) + (Kd * derivative);

  lastError = error;

  deltaL = delta + (int)correction;
  deltaR = delta - (int)correction;

  //Motor Speed Cap
  deltaL = constrain(deltaL, 0, MAX_DELTA);
  deltaR = constrain(deltaR, 0, MAX_DELTA);

  runMotors(deltaL, deltaR);
}

