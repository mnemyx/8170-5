//
// OverlapList Object
// D. House, June 13, 2008
// for CCLI Project
//

#ifndef _OVERLAPLIST_H_
#define _OVERLAPLIST_H_

#include "RigidBody.h"
#include "Witness.h"
#include "EntryTable.h"

struct Overlap{
  RigidBody *a, *b;
  Witness witness;  
};

struct OverlapList{
  Overlap *overlaps;
  EntryTable entries;
  
  int noverlaps;
  int current;
  
  OverlapList(int Maxbodies = 0);
  ~OverlapList();
  
  void setSize(int Maxbodies);
  
  void Clear();
  
  bool Inlist(RigidBody *rb1, RigidBody *rb2);
  
  void Insert(RigidBody *rb1, RigidBody *rb2);
  void Remove(RigidBody *rb1, RigidBody *rb2);
  
  void MergeOverlaps(OverlapList &x, OverlapList &y);
  
  void FindWitnesses();

  Overlap First(bool &done);
  Overlap Next(bool &done);
  
  void print();
  void draw();
};

#endif // _OVERLAPLIST_H_