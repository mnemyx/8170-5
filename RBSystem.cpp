/*
 *
 *  RBSystem.h
 *  GBG - Proj #5 - Rigid Bodies
 *
 */

#include "RBSystem.h"

RBSystem::RBSystem(int nbods) {
    nbodies = nbods;
    rblist = new RBody[nbodies];

    for(int i = 0; i < nbodies; i++)
        rblist[i].setRBI(i);

    Y.setSize(nbodies * 13);
    Ydot.setSize(nbodies * 13);
}

RBSystem::~RBSystem() {
    delete []rblist;
}

void RBSystem::setSize(int nbods) {
    delete []rblist;

    nbodies = nbods;
    rblist = new RBody[nbodies];

    for(int i = 0; i < nbodies; i++)
        rblist[i].setRBI(i);

    Y.setSize(nbodies * 13);
    Ydot.setSize(nbodies * 13);
}

void RBSystem::setParams(double m[], double width[], double height[], double depth[], int type[], double d1[], double d2[], double d3[], Vector4d c[]) {
    for(int i = 0; i < nbodies; i++) {
        rblist[i].setParams(m[i], width[i], height[i], depth[i], type[i], d1[i], d2[i], d3[i]);
        rblist[i].setColor(c[i]);
    }
}

void RBSystem::setEnv(Vector3d g, Vector3d w, double v) {
    Env.G = g;
    Env.W = w;
    Env.Vis = v;
}

void RBSystem::setSpring(double k, double d, Vector3d p0, double l0, int rbinum, int ivert) {
    Spring.SetStrut(k, d, l0, p0);
    springmeta[0] = rbinum;
    springmeta[1] = ivert;
}

void RBSystem::initializeState(Vector3d x0[], Quaternion q[], Vector3d v0[], Vector3d omega0[]) {
    for(int i = 0; i < nbodies; i++) {
        rblist[i].setICs(x0[i], q[i], v0[i], omega0[i]);
        rblist[i].ComputeAuxiliaries();
        rblist[i].print();
    }
}

void RBSystem::drawSys(){
    // draw strut/spring
    for(int i = 0; i < nbodies; i++)
        rblist[i].drawbody();
}

void RBSystem::printsys() {
    cout << "# OF RIGID BODIES: " << nbodies << endl << "     ";
    for (int i = 0; i < nbodies; i++)
        rblist[i].print();
    cout << endl << "Y: " << endl << "     ";
        Y.print();
    cout << endl << "Ydot: " << endl << "     ";
        Ydot.print();
    cout << endl << "Spring (attached to RBI: " << springmeta[0] << ", vertex index: " << springmeta[1] << "): " << endl << "     ";
        Spring.PrintStrut();
    cout << endl << "Environment: " << endl << "     ";
        cout << "G: " << Env.G << endl << "     ";
        cout << "W: " << Env.W << endl << "     ";
        cout << "Viscosity: " << Env.Vis << endl;

}
