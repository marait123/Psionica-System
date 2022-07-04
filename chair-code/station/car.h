#ifndef CAR
#define CAR
// add here 
#include<Arduino.h>
#include"motor.h"
// X means forward and backward ,
unsigned long MOVETIME_X=250; // this is MOVETIME milli seconds
// Y means left and right
unsigned long MOVETIME_Y=125; // this is MOVETIME milli seconds

unsigned long MOVETIME_LEFT=MOVETIME_Y; // this is MOVETIME milli seconds
unsigned long MOVETIME_RIGHT=MOVETIME_Y; // this is MOVETIME milli seconds
unsigned long MOVETIME_FORWARD=MOVETIME_X; // this is MOVETIME milli seconds
unsigned long MOVETIME_BACKWARD=MOVETIME_X; // this is MOVETIME milli seconds

// this car moves in one of 4 directions
class Car
{
private:
  Motor *leftMotor, *rightMotor;
  unsigned long start_time = 0;
  bool is_moving_left = false;
  bool is_moving_right = false;
  
  bool is_moving_forward = false;
  
  bool is_moving_backward = false;
  
  void startMove(){
    start_time = millis();
//    is_moving = true;
  }
  void endMove(){
  //    is_moving = false;
  is_moving_left = false;
  is_moving_right = false;
  is_moving_forward = false; 
  is_moving_backward = false;
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
    is_moving_left = true;
    (*leftMotor).moveBackward();
    (*rightMotor).moveForward();
  }
  void moveRight()
  {
    this->startMove();
    is_moving_right = true;
    (*leftMotor).moveForward();
    (*rightMotor).moveBackward();
  }
  void moveForward()
  {
    this->startMove();
    is_moving_forward = true;
    (*leftMotor).moveForward();
    (*rightMotor).moveForward();

  }
  void moveBackward()
  {
    this->startMove(); 
    is_moving_backward = true;
    (*rightMotor).moveBackward();
    (*leftMotor).moveBackward();
  }
  void setup()
  {
  }
  void loop(){
   
    if(is_moving_left && millis() > MOVETIME_LEFT + start_time ){
      this->stop();
    }
    else if(is_moving_right && millis() > MOVETIME_RIGHT + start_time ){
      this->stop();
    }
    else if(is_moving_forward && millis() > MOVETIME_FORWARD + start_time ){
      this->stop();
    }
    else if(is_moving_backward && millis() > MOVETIME_BACKWARD + start_time ){
      this->stop();
    }
    else{
      Serial.println("this is an unhandled case please review the loop of the car");
    }

  // the car issue 
    if(is_moving_forward){
      //  do this if moving forward
      ledcWrite(0, 180); // set the brightness of the LED
      ledcWrite(1, 230); // set the brightness of the LED 
    }else if (is_moving_backward){
      // do this if moving backward
      ledcWrite(0, 200); // set the brightness of the LED
      ledcWrite(1, 200); // set the brightness of the LED 
    }
  }
};
#endif
