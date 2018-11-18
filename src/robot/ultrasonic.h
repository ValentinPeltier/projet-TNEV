#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include "consts.h"

class Ultrasonic {
public:
  void init(int, int);
  float getDistance();
  
private:
  int triggerPin, echoPin;
};

#endif
