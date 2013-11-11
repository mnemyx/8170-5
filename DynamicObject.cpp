//
// Dynamic Object
// D. House, May 28, 2007
// for CCLI Project
//

using namespace std;

#include "DynamicObject.h"

DynamicObject::Dynamic(){
  nstates = 0;
  statevector = NULL;
}

void DynamicObject::setstatevector(Vector *sv){
  delete statevector;
  statevector = sv;
}
  
int DynamicObject::NStates(){
  return nstates;
}

void DynamicObject::setstateindex(int n){
  
}

#endif // _DYNAMIC_H_