#include "helpers.h"
#include <Arduino.h>
#include <Servo.h>

extern const int GRN;
extern const int YLW;
extern const int RED;
extern const int stopPulse;
extern Servo leftWheel;
extern Servo rightWheel;

void turnOnLED(int COLOUR) {
  digitalWrite(GRN, LOW);
  digitalWrite(YLW, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(COLOUR, HIGH);
}

void runMotors(int deltaL, int deltaR) {
  int pulseL = (stopPulse + deltaL) * 10;
  int pulseR = (stopPulse + deltaR) * 10;
  leftWheel.writeMicroseconds(pulseL);
  rightWheel.writeMicroseconds(pulseR);
}

void led_direction(int error) {
  if(error > 200){  // correct to left
    turnOnLED(RED);
  }
  else if(error < -200){ // correct to right
    turnOnLED(YLW);
  }
  else{
    turnOnLED(GRN);
  }
}
