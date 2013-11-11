//
// Contact List Object
// D. House, June 13, 2008
// for CCLI Project
//
#include "ContactList.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

//
// Constructor for Contact
//
Contact::Contact(){
  a = b = NULL;
}

//
// Destructor for Contact
//
Contact::~Contact(){
  // don't delete a and b, since they may be being used elsewhere!
}

void Contact::set(RigidBody *abody, RigidBody *bbody, 
		  const Plane &plane){
  a = abody;
  b = bbody;
  p = plane.p;
  n = plane.n;
}

double Contact::vrel(){
  return (a->dpdt(p) - b->dpdt(p)) * n;
}

double Contact::impulse(){
  return -vrel() / (a->invInertia(a->r(p), n) + b->invInertia(b->r(p), n));
}

ContactList::ContactList(int Maxbodies){
  contacts = NULL;
  setSize(Maxbodies);
}

ContactList::~ContactList(){
  delete []contacts;
}

void ContactList::setSize(int Maxbodies){
  delete []contacts;
  contacts = NULL;
  entries.setsize(Maxbodies);

  if(Maxbodies > 0)
    contacts = new Contact[(Maxbodies * (Maxbodies - 1)) / 2];
  
  Clear();
}

void ContactList::Clear(){
  entries.clear();
  ncontacts = current = 0;
}

void ContactList::Insert(const Overlap &newoverlap){
  int i;

  // if overlap not yet in list, insert it
  if((i = entries.get(newoverlap.a->rbi, newoverlap.b->rbi)) == -1){
    i = ncontacts;
    contacts[i].a = newoverlap.a;
    contacts[i].b = newoverlap.b;
    entries.set(newoverlap.a->rbi, newoverlap.b->rbi, i);
    ncontacts++;
  }
  
  // now, update the plane
  contacts[i].p = newoverlap.witness.plane.p;
  contacts[i].n = newoverlap.witness.plane.n;
}

void ContactList::Remove(RigidBody *rb1, RigidBody *rb2){
  int i;
  
  if((i = entries.get(rb1->rbi, rb2->rbi)) != -1){
    entries.remove(contacts[i].a->rbi, contacts[i].b->rbi);
    if(current >= i)
      current--;
    for(; i < ncontacts - 1; i++){
      contacts[i] = contacts[i + 1];
      entries.set(contacts[i].a->rbi, contacts[i].b->rbi, i);
    }
    ncontacts--;
    if(ncontacts == 0)
      current = 0;
  }    
}

void ContactList::ExtractContacts(OverlapList &overlaps){
  Overlap overlap;
  Contact *contact;
  bool done;
  int i;
  
  // remove any contacts no longer in the overlap list
  for(contact = First(); contact != NULL; contact = Next())
    if(!overlaps.Inlist(contact->a, contact->b))
      Remove(contact->a, contact->b);
  
  // for all overlaps, insert any that are now contacts and remove
  // any that are no longer contacts
  for(overlap = overlaps.First(done); !done; overlap = overlaps.Next(done))
    if(overlap.witness.status == ON)
      Insert(overlap);
    else
      Remove(overlap.a, overlap.b);
}

Contact *ContactList::First(){
  current = 0;
  if(ncontacts > 0)
    return &contacts[0];
  else
    return NULL;
}

Contact *ContactList::Next(){
  if(current == ncontacts)
    return NULL;
  
  if(current < ncontacts){
    current++;
    if(current < ncontacts)
      return &contacts[current];
    else
      return NULL;
  }
}

void ContactList::print(){
  
  cout << "CONTACTLIST:\n";
  for(int i = 0; i < ncontacts; i++){
    cout << "a " << contacts[i].a << ", b " << contacts[i].b;
    cout << ", p "; contacts[i].p.print();
    cout << ", n "; contacts[i].n.print();
  }
  cout << endl;
}

void ContactList::draw(){
  Plane plane;
  
  for(int i = 0; i < ncontacts; i++){
    plane.p = contacts[i].p;
    plane.n = contacts[i].n;
    plane.draw();
  }
}
