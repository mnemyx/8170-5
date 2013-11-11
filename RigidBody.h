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

class RigidBody{
public:
  int rbi;		// rigid body index of this rigid body
  
  // parameters
  double M, Minv;
  double I, Iinv;
  
  // state
  Vector2d X;
  double Theta;
  Vector2d P;
  double L;
  
  // auxiliaries
  Vector2d v;
  double omega;
  Matrix2x2 R;
  
  // computed
  Vector2d F;
  double Tau;
  
  // motor inputs, as function of t
  Vector2d F_m;
  double Tau_m;

  // geometry description
  Geometry *geom;
  
  RigidBody(double m = 1, double moi = 1, int type = PLANE, 
	    double d1 = 1.0, double d2 = 1.0);
  ~RigidBody();
  
  void setParams(double m = 1, double moi = 1, int type = PLANE, 
		 double d1 = 1.0, double d2 = 1.0);
  
  void setICs(Vector2d x0, double angle,
	      Vector2d v0, double omega0);
  
  void ComputeAuxiliaries();
  
  Vector2d r(const Vector2d &p);      // vector from COM to p
  Vector2d dpdt(const Vector2d &p);   // velocity of point p
  
  // inv-inertia term at COM + r in direction of n
  double invInertia(const Vector2d &r, const Vector2d &n);
  
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