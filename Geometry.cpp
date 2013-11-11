//
// Geometry Object
// D. House, June 13, 2008
// for CCLI Project
//
// Modified to allow 3d geometry - GBG Fall 2013
//

#include <iostream>
#include "Geometry.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

void Geometry::compute_AABB(int in3d){
  left = right = points[0].x;
  bottom = top = points[0].y;
  head = rear = points[0].z;

  for(int i = 1; i < 8; i++){
    if(points[i].x < left)
      left = points[i].x;
    else if(points[i].x > right)
      right = points[i].x;
    if(points[i].y < bottom)
      bottom = points[i].y;
    else if(points[i].y > top)
      top = points[i].y;
    if(points[i].z < head)
	  head = points[i].z;
	else if(points[i].z > rear)
	  rear = points.[i].z;
  }
}

Geometry::Geometry(int kind, double d1, double d2, double d3){
  current_vtx = current_plane = 0;
  set(kind, d1, d2, d3);
}

void Geometry::set(int kind, double d1, double d2,double d3){
  type = kind;
  r = w = d1;
  h = d2;
  d = d3;
}

void Geometry::place_in_world(const Vector3d &x, const Matrix3x3 &R){
  Vector3d dx, dy, dz;
  Vector3d n;
  Vector3d radius(0, 0, 0);

  center = x;

  switch(type){
case PLANE:
    dx.set(w / 2, 0, 0);
    points[0] = x + R * dx;
    points[1] = x - R * dx;
    points[2] = x - R * dx;
    points[3] = x + R * dx;
    planes[0].set(points[0], ~(R * dx), 0);
    break;
case CIRCLE:
    dx.set(r, 0, 0);
    dy.set(0, r, 0);
    points[0] = x + dx;
    points[1] = x + dy;
    points[2] = x - dx;
    points[3] = x - dy;
    radius.set(r, r);
    planes[0].set(points[0], radius, 0);
    break;
case RECT_PRISM:
    dx.set(w / 2, 0, 0);
    dy.set(0, h / 2, 0);
    dz.set(0, 0, z / 2)

    points[0] = dx - dy + dz;
    points[1] = dx + dy + dz;
    points[2] = -dx + dy - dz;
    points[3] = -dx - dy - dz;
    points[4] = dx - dy - dz;
    points[5] = dx + dy - dz;
    points[6] = -dx + dy + dz;
    points[7] = -dx - dy + dz;

    for(int i = 0; i < 8; i++)
      points[i] = x + R * points[i];

    for(int i = 0; i < 6; i++){
      switch(i){
      case 0:
        n.set(1, 0, 0);
        break;
      case 1:
        n.set(0, 1, 0);
        break;
      case 2:
        n.set(-1, 0, 0);
        break;
      case 3:
        n.set(0, -1, 0);
        break;
      case 4:
      case 5:
        n.set(0, 0, -1);
        break;
      case 6:
      case 7:
        n.set(0, 0, 1);
        break;
      }
      planes[i].set(points[i], R * n);
    }
    break;
  }

  compute_AABB();
}

void Geometry::getAABB(double &l, double &r, double &b, double &t, double &h, double &re){
  l = left; r = right; b = bottom; t = top; h = head; re = rear;
}

Vector3d Geometry::FirstVertex(bool &done){
  current_vtx = 0;
  done = false;

  if(type == CIRCLE)
    return center;
  else
    return points[0];
}

Vector3d Geometry::NextVertex(bool &done){
  int numvertices;

  switch(type){
case PLANE:
    numvertices = 2;
    break;
case CIRCLE:
    numvertices = 1;
    break;
case RECT_PRISM:
    numvertices = 8;
    break;
  }

  if(current_vtx >= numvertices - 1)
    done = true;
  else{
    done = false;
    current_vtx++;
  }

  if(type == CIRCLE)
    return center;
  else
    return points[current_vtx];
}

Plane Geometry::FirstPlane(bool &done){
  current_plane = 0;

  if(type == CIRCLE)
    done = true;
  else
    done = false;

  return planes[0];
}

Plane Geometry::NextPlane(bool &done){
  if(type != RECT_PRISM)
    done = true;
  else if(current_plane < 7){
    done = false;
    current_plane++;
  }
  else
    done = true;

  return planes[current_plane];
}

Plane Geometry::ThisPlane(Geometry *other, int which){
  Plane thisplane;
  Vector3d p, n;

  if(type == CIRCLE){
    n = (other->center - center).normalize();
    p = center + r * n;
    thisplane.set(p, n);  // (other must be circle)
  }
  else if(type == PLANE){
    if(which == 0)	  // plane, plane itself is witness
      thisplane = planes[0];
    else{		  // plane, witness on vertex (other must be circle)
      n = (other->center - points[which - 1]).normalize();
      p = points[which - 1];
      thisplane.set(p, n);
    }
  }
  else if(which <=7)	  // rectangular prism, witness is an edge
    thisplane = planes[which];
  else{			  // rect prism, witness on vertex (other must be circle)
    n = (other->center - points[which - 8]).normalize();
    p = points[which - 8];
    thisplane.set(p, n);
  }

  return thisplane;
}

void Geometry::print(){
  cout << "GEOMETRY\n";
  cout << "center "; center.print(); cout << " ";
  if(type == CIRCLE)
    cout << "CIRCLE, r " << r << '\n';
  else if(type == PLANE)
    cout << "PLANE, w " << w << '\n';
  else
    cout << "RECT_PRISM, w " << w << ", h " << h << '\n';

  cout << "AABB: [(" << left << ", " << right << "), (" << bottom << ", " << top << "), (" << head << ", " << rear << ")]\n";

  if(type != CIRCLE){
    cout << "corners: \n";
    for(int i = 0; i < (type == PLANE? 2: 8); i++){
      points[i].print(); cout << " ";
      cout << '\n';
    }
  }

  if(type != CIRCLE){
    cout << "planes: \n";
    for(int i = 0; i < (type == PLANE? 1: 8); i++){
      planes[i].print();
    }
  }
  cout << endl;
}

void drawcircle(Vector3d center, double r){
  const int dangle = 10;
  int angle;
  float theta;
  Vector3d p;
  float x, y;

  glColor3f(0.3, 0.3, 1.0);
  glBegin(GL_LINE_LOOP);
  for(angle = 0; angle < 360; angle += dangle){
    theta = DegToRad(angle);
    x = center.x + r * cos(theta);
    y = center.y + r * sin(theta);
    glVertex2f(x, y);
  }
  glEnd();
}

// draw normal in magenta
void drawnormal(Vector3d p, Vector3d n){
  const float NORMSIZE = 0.5;
  Vector3d endpt;

  glColor3f(1, 0, 1);
  endpt = p + NORMSIZE * n;

  glBegin(GL_LINES);
    glVertex3f(p.x, p.y, p.z);
    glVertex3f(endpt.x, endpt.y, endpt.z);
  glEnd();
}

void drawplane(Vector3d *points, Plane plane){
  Vector3d u, v;
  double l;

  glColor3f(0.3, 0.3, 1.0);
  glBegin(GL_LINES);
    glVertex3f(points[0].x, points[0].y, points[0].z);
    glVertex3f(points[1].x, points[1].y, points[1].z);
  glEnd();

  drawnormal(plane.p, plane.n);
}

void drawrectangle(Vector3d *points, Plane *planes){
  Vector3d p0, p1;

  double minx = points[0].x, maxx = points[0].x;
  double miny = points[0].y, maxy = points[0].y;
  double maxz = points[0].z, minx = points[0].z;

  for(int i = 0; i < 8; i++){
    p0 = points[i];
    if(p0.x < minx) minx = p0.x;
    else if(p0.x > maxx) maxx = p0.x;
    if(p0.y < miny) miny = p0.y;
    else if(p0.y > maxy) maxy = p0.y;
    if(p0.z < minz) minz = p0.z;
    else if (p0.z > maxz) mazx = p0.z;
  }

  glColor3f(0.2, 0.2, 0.2);
  glBegin(GL_LINE_LOOP);
    glVertex3f(minx, miny, minz);
    glVertex3f(maxx, miny, minz);
    glVertex3f(maxx, maxy, minz);
    glVertex3f(minx, maxy, minz);
    glVertex3f(minx, miny, maxz);
    glVertex3f(maxx, miny, maxz);
    glVertex3f(maxx, maxy, maxz);
    glVertex3f(minx, maxy, maxz);
  glEnd();

  for(int i = 0; i < 8; i++){
    glColor3f(0.3, 0.3, 1.0);
    p0 = points[i];
    p1 = points[(i + 1) % 8];
    glBegin(GL_LINES);
      glVertex3f(p0.x, p0.y, p0.z);
      glVertex3f(p1.x, p1.y, p1.z);
    glEnd();

    drawnormal(planes[i].p, planes[i].n);
  }
}

// draw geometry in blue
void Geometry::draw(){
  if(type == CIRCLE)
    drawcircle(center, r);
  else if(type == PLANE)
    drawplane(points, planes[0]);
  else	      // type == RECT_PRISM
    drawrectangle(points, planes);
}
