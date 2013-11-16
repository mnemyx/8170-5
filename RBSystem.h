/*
 *
 *  RBSystem.h
 *  GBG - Proj #5 - Rigid Bodies
 *
 */
#ifndef _RBS_H_
#define _RBS_H_

#define STATE_SIZE  13

#include "RBody.h"
#include "StateVector.h"
#include "Strut.h"
#include "Quaternion.h"

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
        RBSystem(int nbods = 1);
        ~RBSystem();

        void setParams(double m[], double width[], double height[], double depth[], int type[], double d1[], double d2[], double d3[], Vector4d c[]);
        void setEnv(Vector3d g, Vector3d w, double v);
        void setSpring(double k, double d, Vector3d p0, double l0, int rbinum, int ivert);
        void initializeState(Vector3d x0[], Quaternion q[], Vector3d v0[], Vector3d omega0[]);
        /* kind of messed up with the ones below:
        void stateToArr(int indx);
        void arrToState(int indx);

        void arrToBodies();
        void bodiesToArr();

        void computeFT(double t, double dt, int rindx);
        void ddtStateToArr(int indx);
        void dydt(double t, double dt);
        */

        // auxiliary functions...
        //void RK4(const StateVector &X, const StateVector &Xdot, double t, double dt);
        //void XtoState(Vector3d &x, Quaternion &q, Vector3d &p, Vector3d &l, const StateVector X);
        //void StatetoX(const Vector3d &x, const Quaternion &q, const Vector3d &p, const Vector3d &l, StateVector X);
        //StateVector dynamics(const StateVector &X, double t, double dt, const RBody &rb, const Strut &spring, int spi);

        StateVector *getStateY() { return &Y; }
        StateVector *getStateYdot() { return &Ydot; }
        void putStateY(StateVector X) { Y = X; }
        void putStateYdot(StateVector X) { Ydot = X; }

        void printsys();
        void drawSys();

        void takeTimestep(double t, double dt);
};


#endif
