//
// State Vector Object
// D. House, June 25, 2008
// for CCLI Project
//

#ifndef _STATEVECTOR_H_
#define _STATEVECTOR_H_

#include "Vector.h"

struct StateVector{
  int N;
  Vector states;

  StateVector(int numentries = 0);
  ~StateVector();

  void setSize(int numentires);

  void print();
};

#endif // _STATEVECTOR_
