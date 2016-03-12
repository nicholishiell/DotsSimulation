#include <iostream>
#include <math.h>

#include "Vector2d.h"

using namespace std;

int main(){
  
  Vector2d * v = new Vector2d(0.75*M_PI);
  Vector2d * vPerp = v->GetPerp();

  v->Print();
  vPerp->Print();

  float ang = vPerp->GetAngle();
  printf("%f\n", ang);

  return 0;
}
