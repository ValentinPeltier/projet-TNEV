#include "motor.h"

void Motor::init(int speedPin, int control1Pin, int control2Pin) {
  // Set values
  this->speedPin = speedPin;
  this->control1Pin = control1Pin;
  this->control2Pin = control2Pin;
  
  // Set pin mode
  pinMode(speedPin, OUTPUT);
  pinMode(control1Pin, OUTPUT);
  pinMode(control2Pin, OUTPUT);

  // Set speed to 0
  setSpeed(0);

  // Set direction to forward
  setDirection(forward);
}

void Motor::set(Direction direction, int speed) {
  setDirection(direction);
  setSpeed(speed);
}

void Motor::setSpeed(int speed) {
  analogWrite(speedPin, speed);
}

void Motor::setDirection(Direction direction) {
  // Forward
  if(direction == forward) {
    digitalWrite(control1Pin, HIGH);
    digitalWrite(control2Pin, LOW);
  }
  // Backward
  else {
    digitalWrite(control1Pin, LOW);
    digitalWrite(control2Pin, HIGH);
  }
}