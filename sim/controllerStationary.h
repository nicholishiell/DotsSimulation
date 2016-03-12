#ifndef CONTROLLER_STATIONARY_H
#define CONTROLLER_STATIONARY_H

#include <stdio.h>
#include <math.h>

#include "controller.h"
#include "Vector2d.h"

class ControllerStationary: public Controller{

 public:

  ControllerStationary(){};
  ~ControllerStationary(){};
  
  Vector2d * GetVelocityCommand(){
    return new Vector2d(0., 0.);
  };

  void UpdateSensors(std::vector<Vector2d*> bearingVectors){};

};
#endif
