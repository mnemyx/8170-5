//
// Dynamics Object
// D. House, June 25, 2008
// for CCLI Project
//

using namespace std;

#include "Dynamics.h"

StateVector::StateVector(int numentries){
  N = numentries;
  states = new double[N];
}

StateVector::~StateVector(){
  delete []states;
}

Dynamics::Dynamics(){
  nstates = 0;
  statevector = NULL;
}

void Dynamics::setstatevector(Vector *sv){
  delete statevector;
  statevector = sv;
}
  
int Dynamics::NStates(){
  return nstates;
}

void Dynamics::setstateindex(int n){
  
}

#endif // _DYNAMIC_H_