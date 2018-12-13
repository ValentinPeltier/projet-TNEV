#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

enum Direction {
	forward,
	backward
};

class Motor {
public:	
  void init(int, int, int);
  void set(Direction, int);
  void setSpeed(int);
  void setDirection(Direction);
  
private:
  int speedPin;
  int control1Pin;
  int control2Pin;
};

#endif
