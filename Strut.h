/*
 *  Struts.h
 *  GBG - Proj #4 - Springy Mesh
 *
 */

#ifndef _STRUT_H_
#define _STRUT_H_

#include <iostream>
#include <cstdlib>

class Strut {
	private:
        double K;
        double D;
        float L0;
        int P0;
        int P1;
        int HasSprings;

	public:
        Strut();
        Strut(double k, double d, float l0, int p0, int p1, int grpid);

        void SetStrut(double k, double d, float l0, int p0, int p1, int grpid);

        void SetK(double k) { K = k; }
        void SetD(double d) { D = d; }
        void SetL0(float l) { L0 = l; }
        void SetP(int p0, int p1) { P0 = p0; P1 = p1; }

        double GetK() { return K; }
        double GetD() { return D; }
        float GetL0() { return L0; }
        int GetP0() { return P0; }
        int GetP1() { return P1; }
        int IsStrut() { return HasSprings; }

		void PrintStrut();          // debugging
};

#endif

