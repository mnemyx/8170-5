//
// Witness Object
// D. House, June 13, 2008
// for CCLI Project
//

#ifndef _WITNESS_H_
#define _WITNESS_H_

#include "Plane.h"

class RigidBody;

struct Witness{
  Plane plane;	      // plane with point p on body b, normal n away from b
  int which;	      // index of plane on object b
  int status;	      // BELOW, ON, ABOVE (if BELOW, not a separation witness)
  RigidBody *a, *b;
  
  Witness();
  Witness(const Plane &wplane, int pidx, int st, RigidBody *abody, RigidBody *bbody);
  
  void set(const Plane &wplane, int pidx, int st, RigidBody *abody, RigidBody *bbody);
  void updatePlane();	// update plane equation from object b
  
  void print();
};

#endif // _WITNESS_H_