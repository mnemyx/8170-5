//
// State Vector Object
// D. House, June 25, 2008
// for CCLI Project
//

#ifndef _STATEVECTOR_H_
#define _STATEVECTOR_H_

#include "Vector.h"

struct StateVector{
    int N;
    Vector states;

    StateVector(int numentries = 0);
    ~StateVector();

    void setSize(int numentires);

    void print();

    double &operator[](int i) { return states[i]; }
    const double &operator[](int i) const;
    StateVector(const StateVector& o);
    StateVector& operator= (const StateVector& other);
    friend StateVector operator+ (const StateVector& s1, const StateVector &s2);
    friend StateVector operator- (const StateVector& s1, const StateVector &s2);
    friend StateVector operator* (const StateVector& s1, const double scalar);
    friend StateVector operator* (const double scalar, const StateVector& s1);
    friend StateVector operator/ (const StateVector& s1, const double scalar);

};

#endif // _STATEVECTOR_
