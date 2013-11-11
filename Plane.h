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
  Vector2d p;
  Vector2d n;
  
  Plane();
  Plane(const Vector2d &point, const Vector2d &normal);
  
  void set(const Vector2d &point, const Vector2d &normal);
  
  double distance(const Vector2d &x) const;
  
  int region(const Vector2d &x, double offset = 0) const;
  
  void print();
  void draw(double length = 0.0);
  
  void drawnormal(Vector2d p, Vector2d n);
  void drawplane(Vector2d p, Vector2d n, double length);
};

#endif // _PLANE_H_