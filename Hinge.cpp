/*
 *  Hinge.cpp
 *  GBG - Proj #4 - Springy Mesh
 *
 */

#include "Hinge.h"

using namespace std;

Hinge::Hinge() {
    A0 = K = 0.0;
    X0 = X1 = X2 = X3 = 0;
}

Hinge::Hinge(double a0, double k, int x0, int x1, int x2, int x3) {
    A0 = a0;
    X0 = x0;
    X1 = x1;
    X2 = x2;
    X3 = x3;
    K = k;
}


void Hinge::SetHinge(double a0, double k, int x0, int x1, int x2, int x3) {
    A0 = a0;
    X0 = x0;
    X1 = x1;
    X2 = x2;
    X3 = x3;
    K = k;
}

void Hinge::PrintHinge() {
    cout << "A0: " << A0 << ", K: " << K << ", [X0, X1, X2, X3]: [" << X0 << ", " << X1 << ", " << X2 << ", " << X3 << "]" << endl;
}

