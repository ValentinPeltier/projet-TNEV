#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
public:
  void init(int);
  bool click();

private:
  int pin;
  bool state, lastState;
};

#endif