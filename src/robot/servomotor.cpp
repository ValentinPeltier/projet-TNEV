#include "servomotor.h"

void Servomotor::init(int pin) {
  servo.attach(pin);
  setValue(0);
}

void Servomotor::setValue(int v) {
  servo.write(90 + SERVOMOTOR_ANGLE_SHIFT + v);
}

int Servomotor::getValue() {
  return servo.read() - 90 - SERVOMOTOR_ANGLE_SHIFT;
}