/*
  Strut.h

   OpenGL object to represent a spring/mass/damper strut

   VIZA 659            Donald H. House         3/9/00
*/

#include "Graphics.h"

class Strut{
protected:
  double posx, posy;
  double size;
  double k, m, d;
  double x, v;
  double xscale;

public:
  Strut();
  Strut(double xs, double mass, double spring, double damper);
  Strut(double xloc, double yloc, double s, 
	double xs, double mass, double spring, double damper);

  void setxscale(double xs);
  void setposition(double xloc, double yloc);
  void setsize(double s);
  void setconstants(double mass, double spring, double damper);

  void setICs(double x0, double v0);
  double displacement();
  double velocity();

  void state(double &xt, double &vt);
  void forces(double &fs, double &fd);
  void derivs(double &dxdt, double &dvdt);
  void dotimestep(double dt);

  void draw();
};
