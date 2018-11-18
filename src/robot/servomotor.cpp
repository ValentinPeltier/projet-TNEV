#include "servomotor.h"

void Servomotor::init(int pin) {
  servo.attach(pin);
}

void Servomotor::setValue(int v) {
  servo.write(v);
}

int Servomotor::read() {
  return servo.read();
}