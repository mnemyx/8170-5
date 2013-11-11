//
// ExtentList Object
// D. House, June 13, 2008
// for CCLI Project
//

#ifndef _EXTENTLIST_H_
#define _EXTENTLIST_H_

#include "RigidBody.h"
#include "OverlapList.h"

struct extent{
  RigidBody *rb;
  double a;
  bool start;
  
  void print();
};

struct extentstacknode{
  extent entry;
  extentstacknode *next; 
};

struct extentstack{
  extentstacknode *top;
  
  extentstack();
  ~extentstack();
  
  void clear();
  void push(extent ext);
  void pop();
  void remove(RigidBody *r);
  void outputoverlaps(RigidBody *r, OverlapList *overlaps);
};

class ExtentList{
private:
  extent *extlist;
  int xextents;
  int MaxBodies;
  int NEntries;
  extentstack active_extents;
  OverlapList overlaps;
  
  void record_exchange(extent a, extent b);
  
public:
  ExtentList(int xext = 1, int MaxBods = 0);
  ~ExtentList();
  
  void Clear();
  
  void setMaxBodies(int MaxBods);
  void setListType(int xext);
  
  void insertBody(RigidBody *r);
  
  void Sweep();
  void Sort(int initial = 0);
    
  void UpdateExtents();
  
  OverlapList &Overlaps();

  void print();
};

#endif // _EXTENTLIST_H_