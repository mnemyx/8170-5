//
// State Vector Object
// D. House, June 25, 2008
// for CCLI Project
//

#include "StateVector.h"

using namespace std;

StateVector::StateVector(int numentries){
  setSize(numentries);
}

StateVector::~StateVector(){
}

void StateVector::setSize(int nentries){
  N = nentries;
  states.setsize(N);
}