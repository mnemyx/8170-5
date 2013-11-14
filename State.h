/********************************************************
  State.h

  Header File for state vector class

  Gina Guerrero - Fall 2013
********************************************************/


#ifndef _STATEVECTOR_H_
#define _STATEVECTOR_H_

#include "Vector.h"

class State {
	private:
        int nmaxp;
		Vector3d *StateVector;          // Vector of Vector3ds

	public:
        State();
		~State();

        void SetSize(int numofp);

		State(const State& other);
		State& operator= (const State& other);
		Vector3d& operator[](int i);
		State operator+ (const State& other);
		State operator- (const State& other);
		State operator* (const State& other);
		State operator* (const double other);
		State& operator+= (const State& other);

		int GetSize();

		void AddState(int indx, Vector3d center, Vector3d velocity);
		void MoveState(int indx, int maxindx);
		void RemoveState(int indx);
		void GetState(int indx, Vector3d *c, Vector3d *v);
		Vector3d GetCenter(int indx);

		void PrintState();           // debugging
};



#endif
