//
// Dynamic Object
// D. House, May 28, 2007
// for CCLI Project
//

#ifndef _DYNAMIC_H_
#define _DYNAMIC_H_

#include "Vector.h"

class DynamicObject{
private:
  int nstates;
  int stateindex;
  Vector *statevector;

public:
  Dynamic();

  void setstatevector(Vector *sv);
  
  int NStates();
  void setstateindex(int n);

  virtual void putstates();
  virtual void getstates();
};


#endif // _DYNAMIC_H_