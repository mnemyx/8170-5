/*
  Strut.cpp

   OpenGL object to represent a spring/mass/damper strut

   VIZA 659            Donald H. House         3/9/00
*/

#include "Strut.h"

using namespace std;

Strut::Strut(){
  setposition(0, 0);
  setsize(1);
  setconstants(1, 1, 1);
  setxscale(1);
  setICs(0, 0);
}
Strut::Strut(double xs, double mass, double spring, double damper){
  setposition(0, 0);
  setsize(1);
  setconstants(mass, spring, damper);
  setxscale(xs);
  setICs(0, 0);
}Strut::Strut(double xloc, double yloc, double s, 
	      double xs, double mass, double spring, double damper){
  setposition(xloc, yloc);
  setsize(s);
  setconstants(mass, spring, damper);
  setxscale(xs);
  setICs(0, 0);
}

void Strut::setxscale(double xs){
  xscale = xs;
}

void Strut::setposition(double xloc, double yloc){
  posx = xloc;
  posy = yloc;
}
void Strut::setsize(double s){
  size = s;
}
void Strut::setconstants(double mass, double spring, double damper){
  m = mass;
  k = spring;
  d = damper;
}

void Strut::setICs(double x0, double v0){
  x = x0;
  v = v0;
}
double Strut::displacement(){
  return x;
}
double Strut::velocity(){
  return v;
}
void Strut::state(double &xt, double &vt){
  xt = x;
  vt = v;
}

void Strut::forces(double &fs, double &fd){
  if(m == 0){
    fs = fd = 0;
  }
  else{
    fs = - (k / m) * x;
    fd = - (d / m) * v;
  }
}
void Strut::derivs(double &dxdt, double &dvdt){
  double fs, fd;

  forces(fs, fd);
  if(m == 0)
    dxdt = - (k / d) * x;
  else
    dxdt = v;
  dvdt = fs + fd;
}

//
// Extrapolate state one time step using 4th Order Runga-Kutta Integrator
//
void Strut::dotimestep(double dt){
  double dxdt, dvdt;
  double oldx, oldv;
  double k1x, k1v, k2x, k2v, k3x, k3v, k4x, k4v;

  oldx = x; oldv = v;
  derivs(dxdt, dvdt);
  k1x = dxdt * dt; k1v = dvdt * dt;

  x = oldx + 0.5 * k1x;
  v = oldv + 0.5 * k1v;
  derivs(dxdt, dvdt);
  k2x = dxdt * dt; k2v = dvdt * dt;

  x = oldx + 0.5 * k2x;
  v = oldv + 0.5 * k2v;
  derivs(dxdt, dvdt);
  k3x = dxdt * dt; k3v = dvdt * dt;

  x = oldx + k3x;
  v = oldv + k3v;
  derivs(dxdt, dvdt);
  k4x = dxdt * dt; k4v = dvdt * dt;

  x = oldx + (k1x + 2 * k2x + 2 * k3x + k4x) / 6;
  v = oldv + (k1v + 2 * k2v + 2 * k3v + k4v) / 6;
}

void Strut::draw(){
  double springlength, springoffset;

  glPushMatrix();
    glLineWidth(2);
    glTranslatef(posx, posy, 0);
    glScalef(size / 12, size / 12, 1);
    glTranslatef(-6, 0, 0);

    glPushMatrix();				// wall
      glTranslatef(1, 0, 0);
      glScalef(2, 4, 1);
      glColor3f(0.5, 0.5, 0.5);
      drawBox(0, 0, GL_POLYGON);
      glColor3f(1, 1, 1);
      drawBox(0, 0, GL_POLYGON);
    glPopMatrix();

    glColor3f(1, 1, 1);
    glTranslatef(2, 0, 0);			// cylinder
    glPushMatrix();
      glTranslatef(0, -0.55, 0);
      glPushMatrix();
        glTranslatef(2.5, 0, 0);
        glScalef(5, 0.1, 1);
        drawBox(0, 0, GL_POLYGON);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(5.05, 0.1, 0);
        glScalef(0.1, 0.3, 1);
        drawBox(0, 0, GL_POLYGON);
      glPopMatrix();
      glTranslatef(0, 1.1, 0);
      glPushMatrix();
        glTranslatef(2.5, 0, 0);
        glScalef(5, 0.1, 1);
        drawBox(0, 0, GL_POLYGON);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(5.05, -0.1, 0);
        glScalef(0.1, 0.3, 1);
        drawBox(0, 0, GL_POLYGON);
      glPopMatrix();
      
      springoffset = 2.5 + 2.5 * x / xscale;
      if(springoffset > 4.5) springoffset = 4.5;
      else if(springoffset < 0.5) springoffset = 0.5;
      springlength = 5 - springoffset;
      glTranslatef(springoffset, -0.55, 0);
      glLineWidth(4);				// occluded spring part
      glColor3f(0.6, 0.6, 0.6);
      glPushMatrix();
        glScalef(springlength / 2.5, 1, 1);
        glBegin(GL_LINES);
          glVertex2f(0.25, -0.5);
	  glVertex2f(0.5, 0.5);
	  glVertex2f(0.75, -0.5);
	  glVertex2f(1, 0.5);
	  glVertex2f(1.25, -0.5);
	  glVertex2f(1.5, 0.5);
	  glVertex2f(1.75, -0.5);
	  glVertex2f(2, 0.5);
	  glVertex2f(2.25, -0.5);
	  glVertex2f(2.5, 0.5);
	glEnd();
      glPopMatrix();

      glColor3f(1, 1, 1);
      glPushMatrix();				// piston
        glTranslatef(-0.05, 0, 0);
        glScalef(0.1, 0.95, 1);
        drawBox(0, 0, GL_POLYGON);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(2.5, 0, 0);
        glScalef(5, 0.5, 1);
        drawBox(0, 0, GL_POLYGON);
      glPopMatrix();

      glLineWidth(4);				// unoccluded spring part
      glColor3f(0.6, 0.6, 0.6);
      glPushMatrix();
        glScalef(springlength / 2.5, 1, 1);
	glBegin(GL_LINES);
          glVertex2f(0, 0.5);
	  glVertex2f(0.25, -0.5);
	  glVertex2f(0.5, 0.5);
	  glVertex2f(0.75, -0.5);
	  glVertex2f(1, 0.5);
	  glVertex2f(1.25, -0.5);
	  glVertex2f(1.5, 0.5);
	  glVertex2f(1.75, -0.5);
	  glVertex2f(2, 0.5);
	  glVertex2f(2.25, -0.5);
	glEnd();
      glPopMatrix();

      glLineWidth(2);
      glColor3f(0.4, 0.4, 0.4);
      drawCircle(6.25, 0, 1.255, GL_POLYGON);	// weight
      glColor3f(1, 1, 1);
      drawCircle(6.25, 0, 1.255, GL_LINE_LOOP);

    glPopMatrix();
  glPopMatrix();
}

