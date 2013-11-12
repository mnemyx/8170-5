//
// Rigid Body Object
// D. House, May 28, 2007
// for CCLI Project
//
#include "RigidBody.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

//
// helper function for save a and 1/a avoiding divide by zero
//
void a_ainv(double a, double &A, double &Ainv){
  if(a < SMALLNUMBER){
    A = 0;
    Ainv = HUGENUMBER;
  }
  else if(a > HUGENUMBER){
    A = HUGENUMBER;
    Ainv = 0;
  }
  else{
    A = a;
    Ainv = 1.0 / a;
  }
}

//
// Constructor for rigid body
//
RigidBody::RigidBody(double m, double moi, int type, double d1, double d2, double d3){
  Vector3d zero;

  rbi = 0;      // this rigid body's index is currently not set

  geom = NULL;
  setParams(m, moi, type, d1, d2, d3);

  zero.set(0, 0, 0);
  setICs(zero, 0, zero, 0);
}

//
// Destructor for rigid body
//
RigidBody::~RigidBody(){
  // don't decrement class count of rigid bodies or array indices will be botched
  delete geom;
}

void RigidBody::setParams(double m, double moi, int type, double d1, double d2, double d3){
  delete geom;
  geom = new Geometry(type, d1, d2, d3);

  a_ainv(m, M, Minv);
  a_ainv(moi, I, Iinv);
}

void RigidBody::setICs(Vector3d x0, double angle,
		       Vector3d v0, double omega0, Quaternion quat){
  X = x0;
  Theta = angle;
  P = M * v0;
  L = I * omega0;
  Q = quat;

  ComputeAuxiliaries();
}

void RigidBody::ComputeAuxiliaries(){
  v = Minv * P;
  omega = Iinv * L;
  R = Q.rotation();

  geom->place_in_world(X, R);
}

Vector3d RigidBody::r(const Vector3d &p){
  return p - X;
}

Vector3d RigidBody::dpdt(const Vector3d &p){
  return v + omega % r(p);
}

double RigidBody::invInertia(const Vector3d &r, const Vector3d &n){
  return Minv + Iinv * n * ((r % n) % r);
}

int RigidBody::checkWitnessPlane(const Plane &witnessplane) const{
  bool done;
  bool haveon;
  int region;
  Vector3d vtx;

  if(geom->type == CIRCLE)
    return witnessplane.region(geom->center, geom->r);
  else{		    // PLANE or PRISM
    haveon = false;
    for(vtx = geom->FirstVertex(done); !done; vtx = geom->NextVertex(done))
      if((region = witnessplane.region(vtx)) == BELOW)
	return BELOW;
      else if(region == ON)
	haveon = true;
    if(haveon)
      return ON;
    else
      return ABOVE;
  }
}

//
// Check if a witness plane candidate points away from the
// object it is attached to. If not, it is not a witness.
//
int RigidBody::checkLocalWitnessPlaneValidity(const Plane &witnessplane) const{
  int region = witnessplane.region(geom->center);

  if(region == ABOVE)
    return BELOW;
  else
    return ABOVE;
}

//
// Given this rigid body and another, find a witness plane. Final plane is
// labeled BELOW if it is invalid, ON if it is valid and also a contact,
// ABOVE if it is valid but not a contact
//
Witness RigidBody::findWitness(RigidBody *rb, int swapping){
  Vector3d n, p;
  Vector3d vtx;
  Plane witnessplane;
  int idx;
  int region, localregion;
  Witness witness;
  bool done;

  // in comments below, *this is object a, *rb is object b.
  // a plane BELOW is not a witness, a plane ABOVE or ON is a witness.
  // if the plane is ON then this is an actual contact, ABOVE implies separation
  switch(geom->type){
      // object a is a CIRCLE
    case CIRCLE:
      switch(rb->geom->type){
	case CIRCLE:
	  // a=Circle/b=Circle: Witness plane normal is vector from b center to a center
	  // Point on plane is tangent to b where line between centers intersects b
	  n = (geom->center - rb->geom->center).normalize();
	  p = rb->geom->center + rb->geom->r * n;
	  witnessplane.set(p, n);
	  region = checkWitnessPlane(witnessplane);
	  witness.set(witnessplane, 0, region, this, rb);
	  break;
	case PLANE:
	case RECT_PRISM:
	  // a=Circle/b=Plane or b=Prism: First check each plane of b to see
	  // if it is a witness
	  for(idx = 0, witnessplane = rb->geom->FirstPlane(done); !done;
	      idx++, witnessplane = rb->geom->NextPlane(done)){
	    region = checkWitnessPlane(witnessplane);
	    if(region != BELOW) break;
	  }
	  // if no plane of b is a witness, then check to see if any vertex
	  // of b can be used to construct a witness, with point on plane
	  // being the vertex, and normal to plane being vector from vertex
	  // to center of a
	  if(region == BELOW){
	    for(vtx = rb->geom->FirstVertex(done); !done && region == BELOW;
		idx++, vtx = rb->geom->NextVertex(done)){
	      witnessplane.set(vtx, (geom->center - vtx).normalize());
	      region = checkWitnessPlane(witnessplane);
	      if(region != BELOW){
		localregion = rb->checkLocalWitnessPlaneValidity(witnessplane);
		if(localregion == BELOW)
		  region = BELOW;
	      }
	    }
	  }
	  witness.set(witnessplane, idx, region, this, rb);
	  break;
      }
      break;

      // object a is a PLANE or a RECTANGULAR PRISM
    case PLANE:
    case RECT_PRISM:
      switch(rb->geom->type){
	case CIRCLE:
	  // a=Plane or a=Prism/b=Circle: flip a and b and solve this
	  // using using CIRCLE/PLANE code
	  swapping = 1;
	  witness = rb->findWitness(this, swapping);
	  break;
	case PLANE:
	case RECT_PRISM:
	  // a=Plane or a=Prism/b=Plane or b=Prism: First check each plane of b
	  // to see if it is a witness
	  for(idx = 0, witnessplane = rb->geom->FirstPlane(done); !done;
	      idx++, witnessplane = rb->geom->NextPlane(done)){
	    region = checkWitnessPlane(witnessplane);
	    if(region != BELOW) break;
	  }
	  // if no plane of b is a witness, then swap a and b and solve recursively
	  if(region == BELOW && !swapping){
	    swapping = 1;
	    witness = rb->findWitness(this, swapping);
	  }
	  else    // either this is a witness or we have checked everything in vain
	    witness.set(witnessplane, idx, region, this, rb);
	  break;
      }
  }

  return witness;
}

Plane RigidBody::getPlane(RigidBody *other, int which){
  return geom->ThisPlane(other->geom, which);
}

void RigidBody::print(){
  cout << "RIGIDBODY #" << rbi << '\n';
  cout << "M " << M << ", Minv " << Minv << ", I " << I << ", Iinv " << Iinv << '\n';
  cout << "X "; X.print(); cout << ", Theta " << Theta << ", P "; P.print(); cout << ", L " << L << '\n';
  cout << "v "; v.print(); cout << ", Omega " << omega << ", Q:\n";
  Q.print();
  cout << endl;
  geom->print();
}

void RigidBody::draw(){
  geom->draw();
}

