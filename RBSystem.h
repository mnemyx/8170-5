/*
 *
 *  RBSystem.h
 *  GBG - Proj #5 - Rigid Bodies
 *
 */

#ifndef _RBS_H_
#define _RBS_H_

#include "RBody.h"
#include "StateVector.h"
#include "Strut.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

struct Environment {
    Vector3d G;
    Vector3d W;
    double Vis;
};

class RBSystem{
    private:
        int nbodies;

        RBody *rblist;

        StateVector Y;
        StateVector Ydot;

        Environment Env;
        Strut Spring;

        int springmeta[2];

    public:
        RBSystem(int nbods = 0);
        ~RBSystem();

        void setSize(int nbods);
        void setParams(double m[], double width[], double height[], double depth[], int type[], double d1[], double d2[], double d3[], Vector4d c[]);
        void setEnv(Vector3d g, Vector3d w, double v);
        void setSpring(double k, double d, Vector3d p0, double l0, int rbinum, int ivert);
        void initializeState(Vector3d x0[], Quaternion q[], Vector3d v0[], Vector3d omega0[]);

        void drawSys();

        StateVector stateToArr();
        void arrToState(const StateVector &X);

        StateVector *getStateY() { return &Y; }
        StateVector *getStateYdot() { return &Ydot; }
        void putStateY(StateVector X) { Y = X; }
        void putStateYdot(StateVector X) { Ydot = X; }

        void printsys();
};


#endif
