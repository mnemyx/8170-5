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
  enum {YEXTENTS, XEXTENTS, ZEXTENTS};

  int nbodies;

  RigidBody *rblist;
  RigidBody *trylist;

  StateVector Y;
  StateVector Ynew;

  ExtentList xextents;
  ExtentList yextents;
  ExtentList zextents;

  OverlapList alloverlaps;
  OverlapList allcollisions;
  ContactList allcontacts;

public:
  RBSystem(int nbods = 0);
  ~RBSystem();

  int laodParams(char *filename);
  void setParams(double m[], double moi[], int type[], double d1[], double d2[], double d3[]);

  void initializeState(Vector2d x0[], double angle[],
		       Vector2d v0[], double omega0[], Quaternion quat[]);

  void settryState(const StateVector &Y);
  void acceptState();

  StateVector *getState();

  StateVector *Dynamics(const StateVector &X, double t);

  void TakeTimestep(double &time, double dt);

  void Draw();
};

#endif // _SYSTEM_
