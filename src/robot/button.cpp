#include "button.h"

void Button::init(int pin) {
  this->pin = pin;

  pinMode(pin, INPUT);
}

bool Button::click() {
  lastState = state;
  state = digitalRead(pin);

  if(lastState != state && state == true) {
    return true;
  }

  return false;
}