//
// Plane Object
// D. House, June 13, 2008
// for CCLI Project
//

#ifndef _PLANE_H_
#define _PLANE_H_

#include "Vector.h"

enum {BELOW, ON, ABOVE};
const double EPS = 0.001;

struct Plane{
  Vector3d p;
  Vector3d n;

  Plane();
  Plane(const Vector3d &point, const Vector3d &normal);

  void set(const Vector3d &point, const Vector3d &normal);

  double distance(const Vector3d &x) const;

  int region(const Vector3d &x, double offset = 0) const;

  void print();
  void draw(double length = 0.0);

  void drawnormal(Vector3d p, Vector3d n);
  void drawplane(Vector3d p, Vector3d n, double length);
};

#endif // _PLANE_H_
