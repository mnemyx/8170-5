/*
 *  Struts.cpp
 *  GBG - Proj #4 - Springy Mesh
 *
 */

#include "Strut.h"

using namespace std;

Strut::Strut() {
    K = D = 0.0;
    L0 = 0.0;
    P0.set(0,0,0);
}

Strut::Strut(double k, double d, float l, Vector3d p0) {
    K = k;
    D = d;
    L0 = l;
    P0 = p0;
}

Strut::Strut(const Strut& o) {
    K = o.GetK();
    D = o.GetD();
    L0 = o.GetL0();
    P0 = o.GetP0();
}


void Strut::SetStrut(double k, double d, float l, Vector3d p0) {
    K = k;
    D = d;
    L0 = l;
    P0 = p0;
}

void Strut::PrintStrut() {
    cout << "SPRING\nK: " << K << ", D: " << D << ", L0: " << L0 << ", P0: " << P0 << endl;
}
