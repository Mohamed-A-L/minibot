#include <Arduino.h>
#include <Servo.h>
#include "helpers.h"

#define MAX_DELTA 15
#define PID_INTERVAL 10

#define INTERSECT_THRESHOLD 2200
#define TURN_SPEED     8
#define TURN_DURATION  700

Servo leftWheel;
Servo rightWheel;

int deltaL;
int deltaR;
int error;

const int button = 7;

const int motorR = 3;
const int motorL = 4;

const int lSensor = A1;
const int rSensor = A2;
const int sharpSensor = A3;

int lvalue = 0;
int rvalue = 0;
int distance = 0;

int delta = 3;

const float kp = 0.0025;
const float kd = 0.000008;
const float dFilter = 0.9;   // 0=max filtering, 1=no filtering

unsigned long lastTime = 0;
float lastError = 0;
float filteredDerivative = 0;

void setup() {

  pinMode(grn, OUTPUT);
  pinMode(ylw, OUTPUT);
  pinMode(red, OUTPUT);

  pinMode(button, INPUT_PULLUP);

  pinMode(lSensor, INPUT);
  pinMode(rSensor, INPUT);
  pinMode(sharpSensor, INPUT);

  leftWheel.attach(motorL);
  rightWheel.attach(motorR);

  while (digitalRead(button) == LOW) {
    digitalWrite(grn, HIGH);
    delay(125);
    digitalWrite(grn, LOW);
    delay(125);
  }

  lastTime = millis();
}

void loop() {
  lvalue = map(analogRead(lSensor), 0, 1023, 0, 3000);
  rvalue = map(analogRead(rSensor), 0, 1023, 0, 3000);
  distance = map(analogRead(sharpSensor), 0, 1023, 0, 3300);

  // Intersection: turn 90 degrees right
  // if (lvalue > INTERSECT_THRESHOLD && rvalue > INTERSECT_THRESHOLD) {
  //   runMotors(TURN_SPEED, -TURN_SPEED);
  //   delay(TURN_DURATION);
  //   runMotors(0, 0);
  //   lastTime = millis();
  // }

  unsigned long currentTime = millis();
  if (currentTime - lastTime >= PID_INTERVAL) {
    error = lvalue - rvalue;
    float dt = (currentTime - lastTime) / 1000.0;
    float Derivative = (error - lastError) / dt;
    filteredDerivative = dFilter * Derivative + (1.0 - dFilter) * filteredDerivative;
    float correction = (kp * error) + (kd * filteredDerivative);

    lastError = error;
    lastTime = currentTime;

    deltaL = constrain(delta + (int)correction, -MAX_DELTA, MAX_DELTA);
    deltaR = constrain(delta - (int)correction, -MAX_DELTA, MAX_DELTA);
  }

  led_direction(error);

  if (distance >= 1300) {
    deltaL = 0;
    deltaR = 0;
  }

  runMotors(deltaL, deltaR);
}
