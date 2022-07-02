#ifndef CAR
#define CAR
// add here 
#include<Arduino.h>
#include"motor.h"

unsigned long MOVETIME=3000; // this is 3000 milli seconds

// this car moves in one of 4 directions
class Car
{
private:
  Motor *leftMotor, *rightMotor;
  unsigned long start_time = 0;
  bool is_moving = false;
  void startMove(){
    start_time = millis();
    is_moving = true;
  }
  void endMove(){
    is_moving = false;

  }
public:
  Car(Motor& leftMotor, Motor& rightMotor){
    this->leftMotor = &leftMotor;
    this->rightMotor = &rightMotor;
  }
  
  void stop()
  {
    this->endMove();
    (*leftMotor).stop();
    (*rightMotor).stop();
  }
  void moveLeft()
  {
    this->startMove();
    (*leftMotor).stop();
    (*rightMotor).moveForward();
  }
  void moveRight()
  {
    this->startMove();
    (*leftMotor).moveForward();
    (*rightMotor).stop();
  }
  void moveForward()
  {
    this->startMove();
    (*rightMotor).moveForward();
    (*leftMotor).moveForward();
  }
  void moveBackward()
  {
    this->startMove();
    (*rightMotor).moveBackward();
    (*leftMotor).moveBackward();
  }
  void setup()
  {
  }
  void loop(){
    
    if(is_moving && millis() > MOVETIME + start_time ){
      this->stop();
    }  
  }
};
#endif
