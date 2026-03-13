#include "helpers.h"
#include <Arduino.h>
#include <Servo.h>

const int red = 10;
const int grn = 9;
const int ylw = 5;
static const int stopPulse = 1530;
extern Servo leftWheel;
extern Servo rightWheel;

static void turn_on_led(int colour) {
  digitalWrite(grn, LOW);
  digitalWrite(ylw, LOW);
  digitalWrite(red, LOW);
  digitalWrite(colour, HIGH);
}

void runMotors(int deltaL, int deltaR) {
  leftWheel.writeMicroseconds(stopPulse + deltaL);
  rightWheel.writeMicroseconds(stopPulse + deltaR);
}

void led_direction(int error) {
  if (error > 200) {
    turn_on_led(red);
  } else if (error < -200) {
    turn_on_led(ylw);
  } else {
    turn_on_led(grn);
  }
}
