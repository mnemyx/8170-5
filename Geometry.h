//
// Geometry Object
// D. House, June 13, 2008
// for CCLI Project
//
// Modified to allow 3d geometry - GBG Fall 2013
//

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Matrix.h"
#include "Plane.h"

enum {PLANE, CIRCLE, RECT_PRISM, OCTA};

class Geometry{
private:
  void compute_points(const Vector3d &x, const Matrix3x3 &R);

  void compute_AABB();

  int current_vtx;
  int current_plane;

public:
  int type;		// 0 = plane, 1 = circle, 2 = rectangular prism
  double r;		// radius for circle
  double w;		// width for plane or for rectangular prism
  double h;		// height for rectangular prism
  double d;		// depth for 3d objects (cube, octrahedron)
  double left, right, bottom, top, head, rear; // AABB

  Vector3d center;
  Vector3d points[8];	// corners of object (x & y & z extrema for circle)
  Plane planes[8];

  Geometry(int kind = RECT_PRISM, double d1 = 1.0, double d2 = 1.0, double d3 = 1.0);

  void set(int kind = RECT_PRISM, double d1 = 1.0, double d2 = 1.0, double d3 = 1.0);

  void place_in_world(const Vector3d &x, const Matrix3x3 &R);

  void getAABB(double &l, double &r, double &b, double &t, double &f, double &re);

  Vector3d FirstVertex(bool &done);
  Vector3d NextVertex(bool &done);

  Plane FirstPlane(bool &done);
  Plane NextPlane(bool &done);

  Plane ThisPlane(Geometry *other, int which);

  void print();
  void draw();
};


#endif // _GEOMETRY_H_
