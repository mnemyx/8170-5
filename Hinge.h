
/*
 *  Hinge.h
 *  GBG - Proj #4 - Springy Mesh
 *
 */

#ifndef _HINGE_H_
#define _HINGE_H_

#include <iostream>
#include <cstdlib>

class Hinge {
	private:
        double A0;
        double K;
        int X0;
        int X1;
        int X2;
        int X3;

	public:
        Hinge();
        Hinge(double a0, double k, int x0, int x1, int x2, int x3);

        void SetHinge(double a0, double k, int x0, int x1, int x2, int x3);

        void SetA0(double a0) { A0 = a0; }
        void SetX0(int x0) { X0 = x0; }
        void SetX1(int x1) { X1 = x1; }
        void SetX2(int x2) { X2 = x2; }
        void SetX3(int x3) { X3 = x3; }
        void SetK(double k) { K = k; }

        double GetA0() { return A0; }
        int GetX0() { return X0; }
        int GetX1() { return X1; }
        int GetX2() { return X2; }
        int GetX3() { return X3; }
        double GetK() { return K; }

		void PrintHinge();          // debugging
};

#endif

