//
// RBSystem Object
// D. House, June 25, 2008
// for CCLI Project
//

using namespace std;

#include "RBSystem.h"

RBSystem::RBSystem(int nbods){
  nbodies = nbods;
  rblist = new RigidBody[nbodies];
  trylist = new RigidBody[nbodies];
  for(int i = 0; i < nbodies; i++){
    rblist[i].rbi = i;
    trylist[i].rbi = i;
  }

  Y.setSize(nbodies);
  Ynew.setSize(nbodies);

  xextents.setMaxBodies(nbodies);
  xextents.setListType(XEXTENTS);

  yextents.setMaxBodies(nbodies);
  yextents.setListType(YEXTENTS);

  zextents.setMaxBodies(nbodies);
  zextents.setListType(ZEXTENTS);

  alloverlaps.setSize(nbodies);
  allcollisions.setSize(nbodies);

  allcontacts.setSize(nbodies);
}

RBSystem::~RBSystem(){
  delete []trylist;
  delete []rblist;
}

void RBSystem::setParams(double m[], double moi[], int type[],
			      double d1[], double d2[], double d3[]){
  for(int i = 0; i < nbodies; i++){
    rblist[i].setParams(m[i], moi[i], type[i], d1[i], d2[i], d3[i]);
    trylist[i].setParams(m[i], moi[i], type[i], d1[i], d2[i], d3[i]);
  }
}

void RBSystem::initializeState(Vector2d x0[], double angle[],
				    Vector2d v0[], double omega0[], Quaternion quat[]){
  xextents.Clear();
  yextents.Clear();
  for(int i = 0; i < nbodies; i++){
    rblist[i].setICs(x0[i], angle[i], v0[i], omega0[i], quat[i]);
    trylist[i].setICs(x0[i], angle[i], v0[i], omega0[i], quat[i]);

    rblist[i].ComputeAuxiliaries();
    rblist[i].print();

    xextents.insertBody(&rblist[i]);
    yextents.insertBody(&rblist[i]);
    zextents.insertBody(&rblist[i]);
  }

  xextents.Sort(1);
  xextents.print();

  yextents.Sort(1);
  yextents.print();

  zextents.Sort(1);
  zextents.print();

  alloverlaps.Clear();
  alloverlaps.MergeOverlaps(xextents.Overlaps(), yextents.Overlaps(), zextents.Overlaps());
  alloverlaps.FindWitnesses();
  alloverlaps.print();

  allcontacts.Clear();
  allcontacts.ExtractContacts(alloverlaps);
  allcontacts.print();
  cout << endl;
}

void RBSystem::settryState(const StateVector &Y){

}

void RBSystem::acceptState(){

}

void RBSystem::TakeTimestep(double &time, double dt){
  for(int i = 0; i < nbodies; i++){
    rblist[i].X = rblist[i].X + dt * rblist[i].v;
    rblist[i].Theta = rblist[i].Theta + dt * rblist[i].omega;
    rblist[i].ComputeAuxiliaries();
  }

  xextents.UpdateExtents();
  xextents.print();
  yextents.UpdateExtents();
  yextents.print();
  alloverlaps.MergeOverlaps(xextents.Overlaps(), yextents.Overlaps());
  alloverlaps.FindWitnesses();
  alloverlaps.print();

  allcontacts.ExtractContacts(alloverlaps);
  allcontacts.print();
  cout << endl;
}

StateVector *RBSystem::getState(){

}

StateVector *RBSystem::Dynamics(const StateVector &X, double t){

}

void RBSystem::Draw(){
  for(int i = 0; i < nbodies; i++)
    rblist[i].draw();

  // draw the bounding boxes of all overlapping rigid bodies
  // and witness planes of all non-intersecting rigid bodies
  alloverlaps.draw();

  // redraw the witness planes for all contacting rigid bodies
  allcontacts.draw();

}

