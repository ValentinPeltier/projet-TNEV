#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <Servo.h>
#include "consts.h"

class Servomotor {
public:
  void init(int);
  void setValue(int);
  int getValue();
  
private:
  Servo servo;
};

#endif
