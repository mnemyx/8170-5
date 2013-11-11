//
// Dynamics Object
// D. House, June 25, 2008
// for CCLI Project
//

#ifndef _DYNAMICS_H_
#define _DYNAMICS_H_

#include "Vector.h"

struct StateVector{
  int N;
  double *states;
  
  StateVector(int numentries = 0);
  ~StateVector();
};

class Dynamics{
private:
  int nstates;
  int stateindex;
  Vector *statevector;

public:
  Dynamics();

  void setstatevector(Vector *sv);
  
  int NStates();
  void setstateindex(int n);

  virtual void putstates();
  virtual void getstates();
};


#endif // _DYNAMICS_H_