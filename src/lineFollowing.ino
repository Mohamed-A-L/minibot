#include <Servo.h>

#define MAX_DELTA 15
#define BLACK 2200
#define WHITE 600
#define TURN_SPEED 8

#define RED 10
#define GRN 9
#define YLW 5

Servo leftWheel;
Servo rightWheel;
int error;
int deltaL;
int deltaR;

const int BUTTON = 7;

const int MOTOR_R = 3;
const int MOTOR_L = 4;

const int LSENSOR = A1;
const int RSENSOR = A2;
const int SHARP = A3;

const int stopPulse = 145;

int lvalue = 0;  //left sensor value
int rvalue = 0;  //right sensor value
int distance = 0; //SHARP sensor value

int Delta = 8;
const int offset = 1;

//plugged in values
const float Kp = 0.0035;
const float Kd = 0.0000045;

int lastError = 0;
int turnType = 0;

// run robot wheels
void runMotors(int deltaL, int deltaR)
{
  int pulseL = (stopPulse + deltaL)*10;    //length of pulse in microseconds
  int pulseR = (stopPulse + deltaR)*10;
  leftWheel.writeMicroseconds(pulseL);
  rightWheel.writeMicroseconds(pulseR);
}

void turn(){
  if (lvalue > BLACK && rvalue > BLACK) {
    runMotors(TURN_SPEED, TURN_SPEED);
    delay(200);

    runMotors(TURN_SPEED, -TURN_SPEED);
    delay(1250);

    runMotors(TURN_SPEED, TURN_SPEED);
    delay(750);

    runMotors(0, 0);
    delay(400);
  }
  if (distance > 1300) {
    runMotors(TURN_SPEED, -TURN_SPEED + offset);
    delay(2750);

    runMotors(TURN_SPEED, TURN_SPEED);
    delay(650);

    runMotors(0, 0);
    delay(400);
  }
  lastError = 0;
}

// Turn on a single LED and turn others off
void turnOnLED(int colour)
{
  digitalWrite(GRN, LOW);
  digitalWrite(YLW, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(colour, HIGH);
}

// Light LED based on error direction
void led_direction(int error) {
  if (error > 0) {
    turnOnLED(RED);
  } else if (error < 0) {
    turnOnLED(GRN);
  } else {
    turnOnLED(YLW);
  }
}

//Toggle an LED on/off
void toggleLED(int colour){
  digitalWrite(GRN, LOW);
  digitalWrite(YLW, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(colour, HIGH);
  delay(250);
  digitalWrite(colour, LOW);
  delay(250);
}

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
    runMotors(0,0);
  }
}

void loop() {

  lvalue = map(analogRead(LSENSOR), 0, 1023, 0, 3000);
  rvalue = map(analogRead(RSENSOR), 0, 1023, 0, 3000);
  distance = map(analogRead(SHARP), 0, 1023, 0, 3300);

  error = lvalue - rvalue;

  float derivative = error - lastError;
  float correction = (Kp * error) + (Kd * derivative);

  lastError = error;

  deltaL = Delta + (int)correction;
  deltaR = Delta - (int)correction;

  //Motor Speed Cap
  deltaL = constrain(deltaL, 0, MAX_DELTA);
  deltaR = constrain(deltaR, 0, MAX_DELTA);

  led_direction(error);
  turn();

  runMotors(deltaL, deltaR);
}
