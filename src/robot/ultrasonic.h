#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include "consts.h"

// Based on https://www.gotronic.fr/pj2-hc-sr04-utilisation-avec-picaxe-1343.pdf

class Ultrasonic {
public:
  void init(int, int);
  float getDistance();
  
private:
  int triggerPin, echoPin;

  long lastTime = 0L;
  float lastDistance = 0.0f;
};

#endif
