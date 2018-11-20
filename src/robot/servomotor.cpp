#include "servomotor.h"

void Servomotor::init(int pin) {
  servo.attach(pin);
}

void Servomotor::setValue(int v) {
  servo.write(90 + v);
}

int Servomotor::read() {
  return servo.read();
}