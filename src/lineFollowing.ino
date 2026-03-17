#include <Servo.h>
#include "helpers.h"

#define MAX_DELTA 15
#define PID_INTERVAL 10

Servo leftWheel;
Servo rightWheel;

int error;
int deltaL;
int deltaR;

extern const int RED = 10;
extern const int GRN = 9;
extern const int YLW = 5;
const int BUTTON = 7;

const int MOTOR_R = 3;
const int MOTOR_L = 4;

const int LSENSOR = A1;
const int RSENSOR = A2;
const int SHARP = A3;

int lvalue = 0;  //left sensor value
int rvalue = 0;  //right sensor value
int distance = 0; //SHARP sensor value

extern const int stopPulse = 153;  // default = 155
int Delta = 8;
const int offset = 1;

//unplugged values (wip)
// const float Kp = 0.00263;
// const float Ki = 0.0;
// const float Kd = 0.0000008;

//plugged in values
const float Kp = 0.0035;
const float Ki = 0.0;
const float Kd = 0.0000045;

const int setpoint = 0; //target error
unsigned long lastTime = 0;
int lastError = 0;
float integral = 0;

void setup(){  
  pinMode(GRN, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(RED, OUTPUT);
  
  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(LSENSOR, INPUT);
  pinMode(RSENSOR, INPUT);
  pinMode(SHARP, INPUT);

  leftWheel.attach(MOTOR_L);
  rightWheel.attach(MOTOR_R);
  Serial.begin(9600);     
  
  while(digitalRead(BUTTON) == LOW){
    digitalWrite(GRN, HIGH);
    delay(125);
    digitalWrite(GRN, LOW);
    delay(125);
  }
}

void loop() {

  lvalue = map(analogRead(LSENSOR), 0, 1023, 0, 3000);
  rvalue = map(analogRead(RSENSOR), 0, 1023, 0, 3000);
  distance = map(analogRead(SHARP), 0, 1023, 0, 3300);

  unsigned long currentTime = millis();
  if(currentTime - lastTime > PID_INTERVAL) {

  error = lvalue - rvalue;
  const float dt = PID_INTERVAL / 1000.0;
  float derivative = (error - lastError) / dt;
  integral += error * dt;

  float correction = (Kp * error) + (Ki * integral) + (Kd * derivative);

  lastError = error;
  lastTime += PID_INTERVAL;

  deltaL = Delta + (int)correction;
  deltaR = Delta - (int)correction;

  //Motor Speed Cap
  deltaL = constrain(deltaL, 0, MAX_DELTA);
  deltaR = constrain(deltaR, 0, MAX_DELTA);
  integral = constrain(integral, -500, 500);
  }

  led_direction(error);

  Serial.println(distance);
  if(distance >= 1300){
    deltaL = 0;
    deltaR = 0;
  }

  runMotors(deltaL, deltaR);
}

