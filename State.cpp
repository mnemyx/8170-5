/********************************************************
  State.cpp

  Source File for the state vector class

  Gina Guerrero - Fall 2013
********************************************************/

#include "State.h"

using namespace std;

///////////////////////////// PUBLIC FUNCTIONS /////////////////////////
State::State() {
    nmaxp = 0;
    StateVector = NULL;
}

State::~State() {
    if(StateVector != NULL) {
        delete[] StateVector;
        StateVector = NULL;
    }
}

void State::SetSize(int numofp) {
    nmaxp = numofp;
    StateVector = new Vector3d[numofp * 2];

    int i;
    for (i = 0; i < nmaxp; i++) {
        StateVector[i] = Vector(0,0,0);
        StateVector[i + nmaxp] = Vector(0,0,0);
    }
}

State::State(const State& o) {
    SetSize(o.nmaxp);
    int i;

    for (i = 0; i < o.nmaxp; i++) {
        StateVector[i] = o.StateVector[i];
        StateVector[i + nmaxp] = o.StateVector[i + nmaxp];
    }
}

State& State::operator=(const State& o) {
    int i;

    if(nmaxp != o.nmaxp) {
        delete[] StateVector;
        SetSize(o.nmaxp);
    }

    for (i = 0; i < o.nmaxp; i++) {
        StateVector[i] = o.StateVector[i];
        StateVector[i + o.nmaxp] = o.StateVector[i + o.nmaxp];
    }

    return *this;
}

Vector3d& State::operator[](int i) {
    if(i < 0 || i > (nmaxp * 2)) {
        cerr << " out of bounds: state class (state.cpp operator[]) -- i: " << i << " & nmaxp*2: " << nmaxp * 2 << endl;
        exit(1);
    }

    return StateVector[i];
}

State State::operator+ (const State& o) {
    int i;
    State temp;

    if(nmaxp != o.nmaxp) {
        cerr << " mismatch sizes (operator+)! -- nmaxp: " << nmaxp << " & o.nmaxp: " << o.nmaxp << endl;
        exit(1);
    }

    temp.SetSize(nmaxp);

    for (i = 0; i < nmaxp; i ++) {
        temp[i] = StateVector[i] + o.StateVector[i];
        temp[i + nmaxp] = StateVector[i + nmaxp] + o.StateVector[i + nmaxp];
    }

    return temp;
}

State State::operator- (const State& o) {
    int i;
    State temp;

    if(nmaxp != o.nmaxp) {
        cerr << " mismatch sizes (operator-)! -- nmaxp: " << nmaxp << " & o.nmaxp: " << o.nmaxp << endl;
        exit(1);
    }

    temp.SetSize(nmaxp);

    for (i = 0; i < nmaxp; i ++) {
        temp[i] = StateVector[i] - o.StateVector[i];
        temp[i + nmaxp] = StateVector[i + nmaxp] - o.StateVector[i + nmaxp];
    }

    return temp;
}

State State::operator* (const State& o) {
    int i;
    State temp;

    if(nmaxp != o.nmaxp) {
        cerr << " mismatch sizes (operator*)! -- nmaxp: " << nmaxp << " & o.nmaxp: " << o.nmaxp << endl;
        exit(1);
    }

    temp.SetSize(nmaxp);

    for (i = 0; i < nmaxp; i ++) {
        temp[i] = StateVector[i] * o.StateVector[i];
        temp[i + nmaxp] = StateVector[i + nmaxp] * o.StateVector[i + nmaxp];
    }

    return temp;
}

State State::operator* (const double o) {
    int i;
    State temp;

    temp.SetSize(nmaxp);

    for (i = 0; i < nmaxp; i ++) {
        temp[i] = StateVector[i] * o;
        temp[i + nmaxp] = StateVector[i + nmaxp] * o;
    }

    return temp;
}

State& State::operator+= (const State& o) {
    *this = *this + o;
    return *this;
}

int State::GetSize() {
    return nmaxp;
}

//////////////////////////// ACCESS FUNCTIONS /////////////////////////

void State::AddState(int indx, Vector3d c, Vector3d v) {
    StateVector[indx] = c;
    StateVector[indx + nmaxp] = v;
}

void State::MoveState(int indx, int maxused) {
    Vector3d tempc = StateVector[indx];
    Vector3d tempv = StateVector[indx + nmaxp];

    StateVector[indx] = StateVector[maxused];
    StateVector[indx + nmaxp] = StateVector[maxused + nmaxp];
}

void State::RemoveState(int indx) {
    Vector3d tempc = StateVector[indx];
    Vector3d tempv = StateVector[indx + nmaxp];

    StateVector[indx].set(0,0,0);
    StateVector[indx + nmaxp].set(0,0,0);
}

void State::GetState(int indx, Vector3d *c, Vector3d *v) {
    *c = StateVector[indx];
    *v = StateVector[indx + nmaxp];
}

Vector3d State::GetCenter(int indx) {
    return StateVector[indx];
}

void State::PrintState() {
    int i;

    for (i = 0; i < nmaxp * 2; i++) {
        cout << i << ": ";
        StateVector[i].print();
        cout << endl;
    }
}

