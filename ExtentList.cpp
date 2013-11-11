//
// Rigid Body Object
// D. House, June 13, 2008
// for CCLI Project
//
#include "ExtentList.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <iostream>
#include <cstdlib>

using namespace std;

void extent::print(){
  cout << "[" << rb << ", " << a << ", " << (start? "START": "END") << ']' << endl;
}

//
// Constructor for extentstack
//
extentstack::extentstack(){
  top = NULL;
}

extentstack::~extentstack(){
  clear();
}

void extentstack::clear(){
  while(top != NULL)
    pop();
}

void extentstack::push(extent ext){
  extentstacknode *temp;
  
  temp = new extentstacknode;
  temp->entry = ext;
  temp->next = top;
  top = temp;
}

void extentstack::pop(){
  extentstacknode *temp;
  
  if(top == NULL) return;
  
  temp = top;
  top = top->next;
  delete temp;
}

void extentstack::remove(RigidBody *r){
  extentstacknode *prior, *ptr;
  
  for(prior = NULL, ptr = top; ptr != NULL && ptr->entry.rb != r; prior = ptr, ptr = ptr->next);
  if(ptr == NULL){
    cout << "end with no start in extentstack remove\n";
    exit(1);
  }
  
  if(prior == NULL)
    top = ptr->next;
  else
    prior->next = ptr->next;
  
  delete ptr;
}

void extentstack::outputoverlaps(RigidBody *r, OverlapList *overlaps){
  extentstacknode *ptr;
  
  for(ptr = top; ptr != NULL; ptr = ptr->next){
    if(ptr->entry.rb != r)
      overlaps->Insert(r, ptr->entry.rb);
  }
}

void ExtentList::record_exchange(extent a, extent b){
  if(a.start && !b.start)	  // end of b moves to left of start of a
    overlaps.Remove(a.rb, b.rb);
  else if(!a.start && b.start)	  // start of b moves to left of end of a
    overlaps.Insert(a.rb, b.rb);
}

//
// Constructor for ExtentList
//
ExtentList::ExtentList(int xext, int MaxBods){
  MaxBodies = 0;
  setListType(xext);
  
  NEntries = 0;
  extlist = NULL;
  
  setMaxBodies(MaxBods);
}

//
// Destructor for ExtentList
//
ExtentList::~ExtentList(){
  delete []extlist;
}

void ExtentList::Clear(){
  NEntries = 0;
}

void ExtentList::setMaxBodies(int MaxBods){
  if(MaxBods != MaxBodies){
    delete []extlist;
    extlist = new extent[2 * MaxBods];
  }
  MaxBodies = MaxBods;
  overlaps.setSize(MaxBodies);
  NEntries = 0;
}

void ExtentList::setListType(int xext){
  xextents = xext;
}

void ExtentList::insertBody(RigidBody *r){
  
  extlist[NEntries].rb = r;
  if(xextents)
    extlist[NEntries].a = r->geom->left;
  else
    extlist[NEntries].a = r->geom->bottom;
  extlist[NEntries].start = true;
  
  extlist[NEntries + 1].rb = r;
  if(xextents)
    extlist[NEntries + 1].a = r->geom->right;
  else
    extlist[NEntries + 1].a = r->geom->top;
  extlist[NEntries + 1].start = false;
  
  NEntries += 2;
}

void ExtentList::Sweep(){
  active_extents.clear();
  for(int i = 0; i < NEntries; i++){
    if(extlist[i].start)
      active_extents.push(extlist[i]);
    else{
      active_extents.outputoverlaps(extlist[i].rb, &overlaps);
      active_extents.remove(extlist[i].rb);
    }
  }
}

void ExtentList::Sort(int initial){
  int i, j;
  extent top;
  
  for(i = 0; i < NEntries - 1; i++){
    top = extlist[i + 1];
    for(j = i; j >= 0 && extlist[j].a > top.a; j--){
      if(!initial)
	record_exchange(extlist[j], top);
      extlist[j + 1] = extlist[j];
    }
    if(j < i)
      extlist[j + 1] = top;
  }
  
  if(initial)
    Sweep();
}

void ExtentList::UpdateExtents(){
  for(int i = 0; i < NEntries; i++){
    if(xextents)
      extlist[i].a = extlist[i].start? extlist[i].rb->geom->left: extlist[i].rb->geom->right;
    else
      extlist[i].a = extlist[i].start? extlist[i].rb->geom->bottom: extlist[i].rb->geom->top;
  }
  Sort();
}

OverlapList &ExtentList::Overlaps(){
  return overlaps;
}

void ExtentList::print(){
  cout << "EXTENTLIST\n";
  cout << (xextents? "x-extents": "y-extents") << ", MaxBodies " << MaxBodies << 
	  ", NEntries " << NEntries << '\n';

  for(int i = 0; i < NEntries; i++)
    extlist[i].print(); cout << " ";
  cout << '\n';
  
//  overlaps.print();
}