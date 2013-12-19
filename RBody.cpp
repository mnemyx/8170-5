/*
 *
 *  RBody.cpp
 *  GBG - Proj #5 - Rigid Bodies
 *
 */

#include "RBody.h"

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
  //cout << "a: " << a << "; A: " << A << "; Ainv: " << Ainv << endl;
}

//
// Constructor for rigid body
//
RBody::RBody(double m, double width, double height, double depth, int type, double d1, double d2, double d3){
  Vector3d zero;

  rbi = 0;      // this rigid body's index is currently not set

  shape = NULL;
  setParams(m, type, width, height, depth, d1, d2, d3);

  zero.set(0, 0, 0);
  setICs(zero, 0, zero, 0);
}

void RBody::setParams(double m, double width, double height, double depth, int type, double d1, double d2, double d3){
  delete shape;

  shape = new Model;
  if (type == PLANE) {
    Vector3d p0, p1, p2, p3;
    Vector3d c;

    c.set(d1, d2, d3);

    p0.set(-width/2, height/2, -depth/2);
    p0 = p0 + c;

    p1.set(width/2, height/2, -depth/2);
    p1 = p1 + c;

    p2.set(width/2, height/2, depth/2);
    p2 = p2 + c;

    p3.set(-width/2, height/2, depth/2);
    p3 = p3 + c;

    shape->BuildPlane(p0, p1, p2, p3, c);
  } else
    shape->BuildCuboid(width, height, depth, d1, d2, d3);
//cout << "m: " << m << endl;
  a_ainv(m, M, Minv);
  //cout << "M: " << M << "; Minv: " << Minv << endl;
  //cout << "height: " << height << "; depth: " << depth << "; width: " << width << endl;
  //cout << "(height * height + depth * depth): " << (height * height + depth * depth) * M * 1 / 12<< endl;
//cout << "1/12 * M * (height * height + depth * depth): " << 1/12 * M  << endl;
  Ibody.set((height * height + depth * depth) * M * 1 / 12, 0, 0,
            0, (height * height + depth * depth) * M * 1 / 12, 0,
            0, 0, (height * height + depth * depth) * M * 1 / 12);
  //cout << "Ibody: " << endl;
  //Ibody.print();

  Ibodyinv = Ibody.inv();
  //cout << "Ibodyinv: " << endl;
  //Ibodyinv.print();

}

void RBody::initICs(Vector3d x0, Quaternion q, Vector3d v0, Vector3d omega0){
  Q = q;
  //cout << "Q: " << endl;
  //Q.print();
  //Q.normalize().print();
  //Q.normalize().rotation().print();
  //R = Q.normalize().rotation();
  //cout << "R: " << endl;
  //R.print();
  I = R * Ibody * R.transpose();

  X = x0;
  P = M * v0;
  L = I * omega0;

  ComputeAuxiliaries();
}

void RBody::setICs(Vector3d x, Quaternion q, Vector3d p, Vector3d l){
  Q = q;

  R = Q.normalize().rotation();
  I = R * Ibody * R.transpose();

  X = x;
  P = p;
  L = l;

  ComputeAuxiliaries();
}



void RBody::ComputeAuxiliaries(){
  v = Minv * P;
  Iinv = R * Ibodyinv * R.transpose();
  omega = Iinv * L;

  shape->place_in_world(X, R);
}


//
// Destructor for rigid body
//
RBody::~RBody(){
  delete shape;
}


void RBody::drawbody() {
    shape->Draw(color);
}

void RBody::print() {
  cout << "RIGIDBODY #" << rbi << '\n';
  cout << "M: " << M << ", Minv " << Minv << '\n';
  cout << "I: \n"; I.print();
  cout << "Iinv: \n"; Iinv.print();
  cout << "Ibody: \n";Ibody.print();
  cout << "Ibodyinv: \n"; Ibodyinv.print();
  cout << "X: "; X.print();
  cout << endl << "Q: "; Q.print();
  cout << endl << "P: "; P.print();
  cout << endl << "L: " << L << '\n';
  cout << "v "; v.print(); cout << "\nOmega " << omega << "\nR:\n"; R.print(); cout << endl;
  cout << "Force: " << force << endl;
  cout << "Torque: " << torque << endl;
  cout << "Color: " << color << endl;
  shape->print();
}
