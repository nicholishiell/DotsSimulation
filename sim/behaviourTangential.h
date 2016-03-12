#ifndef TANGENTIAL_H
#define TANGENTIAL_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "behaviour.h"
#include "sensorContact.h"

class BehaviourTangential: public Behaviour{

 public:
  BehaviourTangential(float constraint, int id){

    activationLevel = 0.;
    staticGain = 1.;

    targetID = id;
    bearingConstraint = constraint;

    leftHanded = false;
    if(Random() > 0.5)
      leftHanded = true;
    //printf("leftHanded = %d\n", leftHanded);
  };

  ~BehaviourTangential(){};
 
  // Returns a velocity vector
  Vector2d * GetResponse(){
    
    Vector2d * r = new Vector2d(targetBearing);
    Vector2d * rPerp = r->GetPerp();
    Vector2d * f = new Vector2d(bearingConstraint);
    Vector2d * fPerp = f->GetPerp();

    float mag = DotProduct(r, fPerp);

    response = ScalarMultipleVector(mag, rPerp);

    return response;
  };

  // Takes in a list of vectors giving the local position of neighbours
  void UpdateStimulus( std::vector<SensorContact*> contacts){

    targetBearing = contacts[targetID]->GetVector()->GetAngle();
    
    Vector2d * r = new Vector2d(targetBearing);
    Vector2d * rPerp = r->GetPerp();
    Vector2d * f = new Vector2d(bearingConstraint);
    Vector2d * fPerp = f->GetPerp();
    float mag = DotProduct(r, fPerp);

    // Vector2d * u = ScalarMultipleVector(mag, rPerp);
    activationLevel = fabs(mag);
  };
  
  // Display information to the screen.
  void Print(){
    printf("Tangential\t%f\n",activationLevel);
  };

 private:
  float bearingConstraint;
  float targetBearing;
  int targetID;

  bool leftHanded;
 
  float Random(){
    return rand()/(float)(RAND_MAX);
  }

};
#endif
