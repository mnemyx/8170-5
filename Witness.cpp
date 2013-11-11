//
// Witness Object
// D. House, June 13, 2008
// for CCLI Project
//
#include "Witness.h"
#include "RigidBody.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

//
// Constructor for Witness
//
Witness::Witness(){
  a = b = NULL;
}

Witness::Witness(const Plane &wplane, int pidx, int st, RigidBody *abody, 
		 RigidBody *bbody){
  set(wplane, pidx, st, abody, bbody);
}

void Witness::set(const Plane &wplane, int pidx, int st, RigidBody *abody, 
		  RigidBody *bbody){
  plane = wplane;
  which = pidx;
  status = st;
  a = abody;
  b = bbody;
}

void Witness::updatePlane(){
  plane = b->getPlane(a, which);
}

void Witness::print(){
  cout << "status " << ((status == BELOW)? "BELOW": (status == ON)? "ON": "ABOVE") << ", ";
  plane.print();
}
