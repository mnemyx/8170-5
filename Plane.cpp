//
// Plane Object
// D. House, June 13, 2008
// for CCLI Project
//
#include "Plane.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

//
// Constructor for Plane
//
Plane::Plane(){
}

Plane::Plane(const Vector3d &point, const Vector3d &normal){
  p = point;
  n = normal;
}

void Plane::set(const Vector3d &point, const Vector3d &normal){
  p = point;
  n = normal.normalize();
}

double Plane::distance(const Vector3d &x) const{
  Vector3d ray;

  ray = x - p;
  return ray * n;
}

int Plane::region(const Vector3d &x, double offset) const{
  double d = distance(x);

  return ((d > offset + EPS)? ABOVE: ((d > offset - EPS)? ON: BELOW));
}

void Plane::print(){
  cout << "p "; p.print(); cout << ", n "; n.print(); cout << endl;
}

void Plane::drawnormal(Vector3d p, Vector3d n){
  const float NORMSIZE = 0.5;
  Vector3d endpt;

  glColor3f(1, 0, 1);
  endpt = p + NORMSIZE * n;

  glBegin(GL_LINES);
  glVertex3f(p.x, p.y, p.z);
  glVertex3f(endpt.x, endpt.y, endpt.z);
  glEnd();
}

void Plane::drawplane(Vector3d p, Vector3d n, double length){
  Vector3d u;
  Vector3d p0, p1;

  u = ~n;
  p0 = p + 0.5 * length * u;
  p1 = p - 0.5 * length * u;

  glBegin(GL_LINES);
    glVertex3f(p0.x, p0.y, p0.z);
    glVertex3f(p1.x, p1.y, p1.z);
  glEnd();
}

void Plane::draw(double length){
  const double PLANELENGTH = 1;

  if(length == 0)
    length = PLANELENGTH;

  drawplane(p, n, length);
  drawnormal(p, n);
}
