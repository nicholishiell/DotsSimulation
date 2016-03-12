#ifndef FOLLOW_HEADING_H
#define FOLLOW_HEADING_H

#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "sensorContact.h"
#include "behaviour.h"


class BehaviourFollowHeading: public Behaviour{

 public:
  BehaviourFollowHeading(float ang, float val){
    heading = ang;

    if(val > 1.) val = 1.;
    if(val < 0.) val = 0.;

    activationLevel = val;
    staticGain = 0.8;

  };
  ~BehaviourFollowHeading(){};
 
  // Returns a velocity vector
  Vector2d * GetResponse(){
    return new Vector2d(heading);
  }

  // Takes in a list of vectors giving the local position of neighbours
  void UpdateStimulus( std::vector<SensorContact*> contacts){
    
  }
  
  // Display information to the screen.
  void Print(){ 
     printf("Follow\t%f\n",activationLevel);
  };


 private:
  float heading;

};
#endif
