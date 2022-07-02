#ifndef MOTOR
#define MOTOR
#include <Arduino.h>
class Arduino
{
public:
  virtual void setup() = 0;
};

// motor
/**
 * @brief https://howtomechatronics.com/tutorials/arduino/arduino-dc-motor-control-tutorial-l298n-pwm-h-bridge/
 *
 */
class Motor : Arduino
{
public:
  int pin0, pin1;
  Motor(int pin0, int pin1)
  {
    this->pin0 = pin0;
    this->pin1 = pin1;
  }
  void setup()
  {
    pinMode(pin0, OUTPUT);
    pinMode(pin1, OUTPUT);
  }
  void moveForward()
  {
    digitalWrite(pin0, LOW);
    digitalWrite(pin1, HIGH);
  }
  void moveBackward()
  {
    digitalWrite(pin0, HIGH);
    digitalWrite(pin1, LOW);
  }
  void stop()
  {
    digitalWrite(pin0, LOW);
    digitalWrite(pin1, LOW);
  }
};
#endif
