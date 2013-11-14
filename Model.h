/*
  Model.h

  Header File for Geometric Model Class
  Provides for construction of cuboid, cylinder and cone shapes tiled by triangles

  BIHE Computer Graphics    Donald H. House     6/22/06
  Modified - Gina Guerrero - Fall 2013
*/

#ifndef _MODEL_H_
#define _MODEL_H_

#include "Vector.h"
#include "Matrix.h"

#define MAXVERTICES   3600		  // shapes limited to 1000 vertices
#define MAXTRIANGLES  (MAXVERTICES / 3)

#define VERTICALX   0
#define VERTICALZ   1
#define HORIZON     2

#define FRONTBACK		0
#define SIDES			1
#define TOPBOTTOM		2

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

class Model{
protected:
  Vector3d vertices[MAXVERTICES];   // vertex coordinates
  int nvertices;		            // count of the number of vertices
  int triangles[MAXTRIANGLES][3];   // 3 vertex indices for each triangle
  Vector3d normals[MAXTRIANGLES];   // unit normal vector for each triangle
  int ntriangles;		    // count of the number of triangles

  void Clean();			    // bookkeeping, remove all vertices and triangles

  int AddVertex(const Vector3d &v); // insert a simple vertex into vertex table
  int AddTriangle(int v0, int v1, int v2);  // insert a triangle, and its normal

  Vector3d Center;          // center of mass...

public:
  // Constructor, make sure model is empty
  Model();

  // Make a cuboid model
  void BuildCuboid(float width = 1.0, float height = 1.0, float depth = 1.0, double x = 0.0, double y = 0.0, double z = 0.0);

  // Make a cylinder model
  void BuildCylinder(float radius = 0.5, float height = 1.0);

  // Make a cone model
  void BuildCone(float radius = 0.5, float height = 1.0);

  // alternate way to build a sphere
  void BuildSphere(double radius = 70, double x = 0.0, double y = 0.0, double z = 0.0);

  // build a plane
  void BuildPlane(Vector3d p0, Vector3d p1, Vector3d p2, Vector3d p3, Vector3d c);
  void BuildPlane(float length = 1.0, float width = 1.0, int orientation = 1, double x = 0, double y = 0, double z = 0);

  // build circle
  void BuildCircle(float radius = 10, int orientation = 1, double x = 0, double  y = 0, double  z = 0);

  // draw the current model
  void Draw(int wireframe = 1);
  void Draw(Vector4d color);
  void Draw(const float* frontC, const float* backC);

  // get triangle @ index & vertex @ index
  Vector3d GetTriangle(int indx);
  Vector3d GetVertex(int indx);
  int GetNtriangles();
  Vector3d GetNormal(int indx);

  void place_in_world(const Vector3d &x, const Matrix3x3 &R);
  void print();

};

#endif

