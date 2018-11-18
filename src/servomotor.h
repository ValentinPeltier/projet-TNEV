#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <Servo.h>

class Servomotor {
public:
  void init(int);
  void setValue(int);
  int read();
  
private:
  Servo servo;
};

#endif
