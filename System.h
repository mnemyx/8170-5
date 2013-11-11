//
// System Object
// D. House, June 25, 2008
// for CCLI Project
//

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "RigidBody.h"
#include "ExtentList.h"
#include "OverlapList.h"
#include "ContactList.h"
#include "StateVector.h"

class RBSystem{
private:
  enum {YEXTENTS, XEXTENTS};

  int nbodies;
  
  RigidBody *rblist;
  RigidBody *trylist;
  
  StateVector Y;
  StateVector Ynew;
  
  ExtentList xextents(XEXTENTS);
  ExtentList yextents(YEXTENTS);
  
  OverlapList alloverlaps;
  OverlapList allcollisions;
  ContactList allcontacts;

public:
  System(int nbods = 0);
  
  int laodParams(char *filename);
  void setParams(double m[], double moi[], int type[], double d1[], double d2[]);
  
  void initializeState(Vector2d x0[], double angle[],
		       Vector2d v0[], double omega0[]);
  
  void settryState(const StateVector &Y);
  void acceptState();
  
  StateVector *getState();

  StateVector *Dynamics(const StateVector &X, double t);
};

#endif // _SYSTEM_