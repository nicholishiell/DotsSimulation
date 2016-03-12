#ifndef CONTROLLER_COMP_H
#define CONTROLLER_COMP_H

#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "controller.h"
#include "behaviour.h"

class ControllerComp: public Controller {

 public:
  ControllerComp(){}

  void AddBehaviour(Behaviour * newBehaviour){
    behaviourSet.push_back(newBehaviour);
  }

  
  Vector2d * GetVelocityCommand(){

  

    float highest = -1.;
    int id = 0;

    for(int i = 0; i < behaviourSet.size(); i++){
      
      float activationLevel = behaviourSet[i]->GetActivationLevel();   
      if(activationLevel > highest){
	highest = activationLevel;
	id = i;
      }
    }
    
    return behaviourSet[id]->GetResponse();
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
