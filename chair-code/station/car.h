#ifndef CAR
#define CAR
// add here 
#include<Arduino.h>
#include"motor.h"
#include"sonar.h"
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
  
  Sonar backSonar;

  
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
  Car(Motor& leftMotor, Motor& rightMotor):backSonar(13,12){ //:backSonar(13,12)
    this->leftMotor = &leftMotor;
    this->rightMotor = &rightMotor;
  }
  void setup()
  {
    backSonar.setup();
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
  int get_back_distance(){
  
    return backSonar.get_distance();
  }
  void loop(){
   


    //  sonar_get_distance();
    int backDistance = backSonar.get_distance();
    // Prints the distance in the Serial Monitor
      Serial.print("backDistance (cm): ");
      Serial.println(backDistance);


    if(is_moving_left && millis() > MOVETIME_LEFT + start_time ){
      this->stop();
    }
    else if(is_moving_right && millis() > MOVETIME_RIGHT + start_time ){
      this->stop();
    }
    else if(is_moving_forward && (millis() > MOVETIME_FORWARD + start_time )){
      this->stop();
    }
    else if(is_moving_backward &&( millis() > MOVETIME_BACKWARD + start_time || backDistance < 20)){
      this->stop();
    }
    else{
//      Serial.println("this is an unhandled case please review the loop of the car");
    }

  // the car issue the car when moving forward it inclines to the left since the right motor 
  // is more powerfull compared to the left but this doesn't happen in the backward direction
    if(is_moving_forward ||  is_moving_right){
      //  do this if moving forward
      ledcWrite(1, 200); // set the brightness of the LED 
      ledcWrite(0, 200); // set the brightness of the LED
    }else{ //else if (is_moving_backward || is_moving_left)
      // do this if moving backward
      ledcWrite(0, 200); // set the brightness of the LED
      ledcWrite(1, 200); // set the brightness of the LED 
    }
  }
};
#endif
