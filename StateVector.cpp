//
// State Vector Object
// D. House, June 25, 2008
// for CCLI Project
//

#include "StateVector.h"

using namespace std;

StateVector::StateVector(int numentries){
  setSize(numentries);
}

StateVector::~StateVector(){
}

void StateVector::setSize(int nentries){
  N = nentries;
  states.setsize(N);
}

void StateVector::print() {
    cout << "StateVector Size: " << N << endl;
    for (int i = 0; i < N; i++)
        cout << "     StateVector[" << i << "]: " << states[i] << endl;
}

StateVector::StateVector(const StateVector& o) {
    setSize(o.N);

    for(int i = 0; i < o.N; i++)
        states[i] = o.states[i];
}

StateVector& StateVector::operator= (const StateVector& o) {
    if (N != o.N) {
        setSize(o.N);
    }

    for(int i = 0; i < o.N; i++)
        states[i] = o.states[i];

    return *this;
}

StateVector operator+ (const StateVector& s1, const StateVector &s2) {
    if(s1.N != s2.N) {
        cerr << "mismatched sizes in operator+ overload" << endl;
        exit(1);
    }

    StateVector temp;

    temp.setSize(s1.N);

    for(int i = 0; i < s1.N; i++)
        temp[i] = s1.states[i] + s2.states[i];

    return temp;
}

StateVector operator- (const StateVector& s1, const StateVector &s2) {
    if(s1.N != s2.N) {
        cerr << "mismatched sizes in operator+ overload" << endl;
        exit(1);
    }

    StateVector temp;

    temp.setSize(s1.N);

    for(int i = 0; i < s1.N; i++)
        temp[i] = s1.states[i] - s2.states[i];

    return temp;
}

StateVector operator* (const StateVector& s1, const double scalar) {
    StateVector temp;

    temp.setSize(s1.N);

    for(int i = 0; i < s1.N; i++)
        temp[i] = s1.states[i] * scalar;

    return temp;
}

StateVector operator* (const double scalar, const StateVector& s1) {
    StateVector temp;

    temp.setSize(s1.N);

    for(int i = 0; i < s1.N; i++)
        temp[i] = s1.states[i] * scalar;

    return temp;
}

StateVector operator/ (const StateVector& s1, const double scalar) {
    StateVector temp;

    temp.setSize(s1.N);

    for(int i = 0; i < s1.N; i++)
        temp[i] = s1.states[i] / scalar;

    return temp;
}



const double &StateVector::operator[](int i) const {
    return states[i];
}
