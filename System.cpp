//
// System Object
// D. House, June 25, 2008
// for CCLI Project
//

using namespace std;

#include "System.h"

System::System(int nbods){
  nbodies = nbods;
  rblist = new RigidBody[nbodies];
  trylist = new RigidBody[nbodies];
  
  Y.setSize(nbodies);
  Ynew.setSize(nbodies);
  
  xextents.setMaxBodies(nbodies);
  yextents.setMaxBodies(nbodies);
  alloverlaps.setSize(nbodies);
  allcollisions.setSize(nbodies);
  allcontacts.setSize(nbodies);
}

System::~System(){
  delete []trylist;
  delete []rblist;
}

void System::setParams(double m[], double moi[], int type[], 
			      double d1[], double d2[]){
  for(int i = 0; i < nbodies; i++){
    rblist[i].setParams(m[i], moi[i], type[i], d1[i], d2[i]);
    trylist[i].setParams(m[i], moi[i], type[i], d1[i], d2[i]);
  }
}

void System::initializeState(Vector2d x0[], double angle[],
				    Vector2d v0[], double omega0[]){
  for(int i = 0; i < nbodies; i++){
    rblist[i].setICs(x0[i], angle[i], v0[i], omega0[i]);
    trylist[i].setICs(x0[i], angle[i], v0[i], omega0[i]);
  }  
}

void System::settryState(const StateVector &Y){
  
}

void System::acceptState(){
  
}

StateVector *System::getState(){
  
}

StateVector *System::Dynamics(StateVector *X, double t){
  
}

