#ifndef SONAR
#define SONAR
#include<Arduino.h>

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
class Sonar{
private:

  //trig 13  echo  12 for front sonar
  int trigPin ;
  int echoPin;
  long duration;
  float distanceCm;


public:

  Sonar(int trigPin, int echoPin){
    this->trigPin =  trigPin;
    this->echoPin = echoPin;
   }
  // sonar setup
  void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  }

  int get_distance(){
    
      // Clears the trigPin
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin, HIGH);
      
      // Calculate the distance
      distanceCm = duration * SOUND_SPEED/2;
      
      return distanceCm;
    }

  void loop(){
    
  }
  
};




#endif
