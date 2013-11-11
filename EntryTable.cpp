//
// Entry Table Object
// D. House, June 13, 2008
// for CCLI Project
//
#include <cstdlib>
#include "EntryTable.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <cstdio>

using namespace std;

//
// helper function to swap two ints
//
void EntryTable::swap(int &a, int &b) const{
  int tmp = a;
  a = b;
  b = tmp;
}

//
// Constructor for Contact
//
EntryTable::EntryTable(int size){
  n = 0;
  table = NULL;
  setsize(size);
}

//
// Destructor for Contact
//
EntryTable::~EntryTable(){
  if(n > 0){
    delete []table[0];
    delete []table;
  }
}

void EntryTable::setsize(int size){
  if(size != n){
    if(n > 0){
      delete []table[0];
      delete []table;
    }
    table = NULL;
  }
  
  n = size;
  if(n == 0) return;
  
  table = new int*[n - 1];
  table[0] = new int[(n * (n - 1)) / 2];
  for(int i = 1; i < n - 1; i++)
    table[i] = table[i - 1] + n - i;
  
  clear();
}

void EntryTable::clear(){
  for(int i = 0; i < n - 1; i++)
    for(int j = i + 1; j < n; j++)
      set(i, j, -1);
}

void EntryTable::set(int obja, int objb, int idx){
  if(obja == objb)
    return;
  else if(obja > objb)
    swap(obja, objb);
  
  table[obja][objb - (obja + 1)] = idx;
}

void EntryTable::remove(int obja, int objb){
  if(obja == objb)
    return;
  else if(obja > objb)
    swap(obja, objb);

  table[obja][objb - (obja + 1)] = -1;
}

int EntryTable::get(int obja, int objb) const{
  if(obja == objb)
    return -1;
  else if(obja > objb)
    swap(obja, objb);
  
  return table[obja][objb - (obja + 1)];
}

