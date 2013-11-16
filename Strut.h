/*
 *  Struts.h
 *  GBG - Proj #4 - Springy Mesh
 *
 */

#ifndef _STRUT_H_
#define _STRUT_H_

#include "Vector.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

class Strut {
	private:
        double K;
        double D;
        float L0;
        Vector3d P0;

	public:
        Strut();
        Strut(double k, double d, float l0, Vector3d p0);
        Strut(const Strut& o);

        void SetStrut(double k, double d, float l0, Vector3d p0);

        void SetK(double k) { K = k; }
        void SetD(double d) { D = d; }
        void SetL0(float l) { L0 = l; }
        void SetP(Vector3d p0) { P0 = p0; }

        double GetK() { return K; }
        double GetD() { return D; }
        float GetL0() { return L0; }
        Vector3d GetP0() { return P0; }

        const double GetK() const { return K; }
        const double GetD() const { return D; }
        const float GetL0() const { return L0; }
        const Vector3d GetP0() const { return P0; }

		void PrintStrut();          // debugging
};

#endif

