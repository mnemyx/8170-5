//
// Rigid Body Object
// D. House, May 28, 2007
// for CCLI Project
//

#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "Matrix.h"
#include "Geometry.h"
#include "Plane.h"
#include "Witness.h"
#include "Quaternion.h"
#include "rkstuff.cpp"

class RigidBody{
public:
  int rbi;		// rigid body index of this rigid body

  // parameters
  double M, Minv;
  double I, Iinv;

  // state
  Vector3d X;
  Vector3d P;
  double L;
  Quaternion Q;

  // auxiliaries
  Vector3d v;
  double omega;
  double Theta;
  Matrix3x3 R;

  // computed
  Vector3d F;
  double Tau;

  // motor inputs, as function of t
  Vector3d F_m;
  double Tau_m; // torque

  // geometry description
  Geometry *geom;

  RigidBody(double m = 1, double moi = 1, int type = PLANE,
	    double d1 = 1.0, double d2 = 1.0, double d3 = 1.0);
  ~RigidBody();

  void setParams(double m = 1, double moi = 1, int type = PLANE,
		 double d1 = 1.0, double d2 = 1.0,  double d3 = 1.0);

  void setICs(Vector3d x0, double angle,
	      Vector3d v0, double omega0, Quaternion quat);

  void ComputeAuxiliaries();

  Vector3d r(const Vector3d &p);      // vector from COM to p
  Vector3d dpdt(const Vector3d &p);   // velocity of point p

  // inv-inertia term at COM + r in direction of n
  double invInertia(const Vector3d &r, const Vector3d &n);

  int checkWitnessPlane(const Plane &witnessplane) const;
  int checkLocalWitnessPlaneValidity(const Plane &witnessplane) const;

  Witness findWitness(RigidBody *rb, int swapping = 0);
  Plane getPlane(RigidBody *other, int which);

  void print();
  void draw();

  void putstates();
  void getstates();
};


#endif // _RIGIDBODY_H_
