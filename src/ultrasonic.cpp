#include "ultrasonic.h"

void Ultrasonic::init(int triggerPin, int echoPin) {
  this->triggerPin = triggerPin;
  this->echoPin = echoPin;

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

float Ultrasonic::getDistance() {
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);

  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);

  float distance = duration / 2.0f / 29.1f;

  if(distance <= 0.0f || distance >= 400.0f) {
    return 0;
  }

  return distance;
}