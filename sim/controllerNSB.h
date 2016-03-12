#ifndef CONTROLLER_NSB_H
#define CONTROLLER_NSB_H

#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "controller.h"
#include "behaviour.h"

class ControllerNSB: public Controller {

 public:
  ControllerNSB(){
    

  }

  void AddBehaviour(Behaviour * newBehaviour){
    behaviourSet.push_back(newBehaviour);
  }

  int GetHighestActivation(){
    float highest = 0.;
    int id = 0;
    for(int i = 0; i < behaviourSet.size(); i++){
      float act = behaviourSet[i]->GetActivationLevel();
      if( act > highest){
	id = i;
	highest = act;
      }
    }
    return id;
  }

  int GetSecondHighestActivation(int max){
    float secondHighest = 0.;
    int id = 0;
    for(int i = 0; i < behaviourSet.size(); i++){
      float act = behaviourSet[i]->GetActivationLevel();
      if( act > secondHighest && i != max){
	id = i;
	secondHighest = act;
      }
    }
    return id;
  }

  Vector2d * GetVelocityCommand(){
    Print();

    int id0 = GetHighestActivation();
    int id1 = GetSecondHighestActivation(id0);

    Vector2d * v0 = behaviourSet[id0]->GetResponse();
    Vector2d * v1 = behaviourSet[id1]->GetResponse();

    float mag = DotProduct(v1, v0);

    Vector2d * v0Perp = v0->GetPerp();

    v0Perp = ScalarMultipleVector(mag, v0Perp);

    //Vector2d * velocityCommand = AddVectors(v0, v0Perp);

    Vector2d * velocityCommand = behaviourSet[id0]->GetResponse();
    
    return velocityCommand;
  }


  void UpdateSensors(std::vector<Vector2d*> bearingVectors){
    for(int i = 0; i < behaviourSet.size(); i++){

      behaviourSet[i]->UpdateStimulus(bearingVectors);

    }
  }

  void Print(){
    /*
      for(int i = 0; i < behaviourSet.size(); i++)
	behaviourSet[i]->Print();

      printf("====================\n");
    /**/
  }


 private:

  
};


#endif
