//
// Overlap List Object
// D. House, June 13, 2008
// for CCLI Project
//
#include "OverlapList.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <iostream>
#include <cstdlib>

using namespace std;

//
// Constructor for OverlapList
//
OverlapList::OverlapList(int Maxbodies){
  overlaps = NULL;
  setSize(Maxbodies);
}

OverlapList::~OverlapList(){
  delete []overlaps;
}

void OverlapList::setSize(int Maxbodies){
  delete []overlaps;
  overlaps = NULL;
  entries.setsize(Maxbodies);
  
  if(Maxbodies > 0)
    overlaps = new Overlap[(Maxbodies * (Maxbodies - 1)) / 2];
  
  Clear();
}

void OverlapList::Clear(){
  entries.clear();
  noverlaps = current = 0;
}

bool OverlapList::Inlist(RigidBody *rb1, RigidBody *rb2){
  return entries.get(rb1->rbi, rb2->rbi) != -1;
}

void OverlapList::Insert(RigidBody *rb1, RigidBody *rb2){

  if(!Inlist(rb1, rb2)){
    overlaps[noverlaps].a = rb1;
    overlaps[noverlaps].b = rb2;
    entries.set(rb1->rbi, rb2->rbi, noverlaps);
    noverlaps++;
  }
}

void OverlapList::Remove(RigidBody *rb1, RigidBody *rb2){
  int i;
  
  if((i = entries.get(rb1->rbi, rb2->rbi)) != -1){
    entries.remove(overlaps[i].a->rbi, overlaps[i].b->rbi);
    if(current >= i)
      current--;
    for(; i < noverlaps - 1; i++){
      overlaps[i] = overlaps[i + 1];
      entries.set(overlaps[i].a->rbi, overlaps[i].b->rbi, i);
    }
    if(--noverlaps == 0)
      current = 0;
  }
}

void OverlapList::MergeOverlaps(OverlapList &x, OverlapList &y){
  RigidBody *rb1, *rb2;
  Overlap ovl;
  bool done;
  
  Clear();
  for(ovl = x.First(done); !done; ovl = x.Next(done))
    if(y.Inlist(ovl.a, ovl.b))
      Insert(ovl.a, ovl.b);
}

void OverlapList::FindWitnesses(){
  
  for(int i = 0; i < noverlaps; i++)
    overlaps[i].witness = overlaps[i].a->findWitness(overlaps[i].b);
}

Overlap OverlapList::First(bool &done){
  Overlap ovl;
  
  current = 0;
  
  if(current == noverlaps)
    done = true;
  else{
    done = false;
    ovl = overlaps[current];
  }
  
  return ovl;
}

Overlap OverlapList::Next(bool &done){
  Overlap ovl;
  
  if(current != noverlaps)
    current++;
  
  if(current == noverlaps)
    done = true;
  else{
    done = false;
    ovl = overlaps[current];
  }
  
  return ovl;
}

void OverlapList::print(){

  cout << "OVERLAPLIST:\n";
  for(int i = 0; i < noverlaps; i++){
    cout << "a " << overlaps[i].a << ", b " << overlaps[i].b << ", ";
    overlaps[i].witness.print();
  }
  cout << endl;
}

void OverlapList::draw(){

  for(int i = 0; i < noverlaps; i++){
    switch(overlaps[i].witness.status){
      case BELOW:
	glColor3f(1, 0, 0);
	break;
      case ON:
	glColor3f(1, 1, 0);
	break;
      case ABOVE:
	glColor3f(0, 1, 0);
	break;
    }
    overlaps[i].witness.plane.draw();
  }
}
