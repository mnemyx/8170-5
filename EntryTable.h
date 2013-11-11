//
// Entry Table Object
// D. House, June 13, 2008
// for CCLI Project
//

#ifndef _ENTRYTABLE_H_
#define _ENTRYTABLE_H_

class EntryTable{
private:
  int n;
  int **table;
  
  void swap(int &a, int &b) const;
  
public:
  EntryTable(int size = 0);
  ~EntryTable();
  
  void setsize(int size);
  
  void clear();
  void set(int obja, int objb, int idx);  // store
  void remove(int obja, int objb);
  int get(int obja, int objb) const;	  // retrieve
};

#endif // _ENTRYTABLE_H_