/*
 *
 *  RBody.h
 *  GBG - Proj #5 - Rigid Bodies
 *
 */

#ifndef _RBODY_H_
#define _RBODY_H_

#include "Quaternion.h"
#include "Vector.h"
#include "Matrix.h"
#include "Model.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

enum {CUBE, PLANE};

class RBody {
	private:
        int rbi;            // rigid body index

        /** constant quantities **/
        double M, Minv;           // mass
        Matrix3x3 Ibody, Ibodyinv;           // moments of inertia...tensor?

        /** state variables **/
        Vector3d X;         // position
        Quaternion Q;       // rotation
        Vector3d P;         // momentum
        Vector3d L;         // angular momentum

        /** derived quantities (auxiliary variables) **/
        Matrix3x3 I, Iinv;
        Vector3d v, omega;          // velocity, angular velocity
        Matrix3x3 R;                // rotation, just to have

        /** computed quantities **/
        Vector3d force, torque;

        Model *shape;
        Vector4d color;

	public:
        RBody(double m = 1, double width = 10.0, double height = 10.0, double depth = 10.0, int type = CUBE, double d1 = 1.0, double d2 = 1.0, double d3 = 1.0);
        ~RBody();

        void setRBI(int indx) { rbi = indx; }
        void setParams(double m = 1, double width = 10.0, double height = 10.0, double depth = 10.0, int type = CUBE, double d1 = 1.0, double d2 = 1.0, double d3 = 1.0);
        void setColor(Vector4d c) {color = c;}
        void setICs(Vector3d x0, Quaternion q, Vector3d v0, Vector3d omega0);
        void ComputeAuxiliaries();

		void print();     // debugging
		void drawbody();
};

#endif

