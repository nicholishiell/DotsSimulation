#ifndef SMART_AVOID_H
#define SMART_AVOID_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "behaviour.h"
#include "sensorContact.h"

class BehaviourSmartAvoid: public Behaviour{

 public:
  BehaviourSmartAvoid(float val){

    activationLevel = 0.;
    staticGain = 1.5;
    minDist = val;

    if(Random() < 0.5)
      leftHanded = false;
    else
      leftHanded = false;
      
  };

  ~BehaviourSmartAvoid(){};
 
  // Returns a velocity vector
  Vector2d * GetResponse(){

    float theta = M_PI/2. * (1. + activationLevel);
   
    if(leftHanded)
      theta = theta * -1.;
    
    return new Vector2d( bearing2Obstacle + theta  );
  }

  // Takes in a list of vectors giving the local position of neighbours
  void UpdateStimulus( std::vector<SensorContact*> contacts){

    // Find closest neighbour
    float closest = 99999999.;
    int id = 0;

    for(int i =0; i < contacts.size(); i++ ){

      Vector2d * localPos = contacts[i]->GetVector();
      float dist = localPos->GetNorm();
      
      if(dist < closest && dist > 0.){
	closest = dist;
	id = i;
      }
           
    }
    
 

    if(closest < minDist){
      activationLevel = 1. - closest/minDist;
      bearing2Obstacle = localPositions[id]->GetAngle(); 
    }
    else{
      activationLevel = 0.;
    }

  };
  
  // Display information to the screen.
  void Print(){
    printf("Avoid\t%f\n",activationLevel);
  };

  // Return the activation level of the behaviour
  virtual float GetActivationLevel(){
    return activationLevel;
  };
  
 private:
  
  float Random(){
    return rand()/(float)(RAND_MAX);
  }

  float minDist;
  bool leftHanded;

  float bearing2Obstacle;
  
};
#endif
