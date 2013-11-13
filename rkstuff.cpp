Vector RK4(const Vector &X, const Vector &Xdot, double t, double h, int resting, int stopped){
  Vector K1(4), K2(4), K3(4), K4(4);

  K1 = h * Xdot;
  K2 = h * dynamics(X + 0.5 * K1, t + 0.5 * h, resting, stopped);
  K3 = h * dynamics(X + 0.5 * K2, t + 0.5 * h, resting, stopped);
  K4 = h * dynamics(X + K3, t + h, resting, stopped);

  return X + (K1 + 2 * K2 + 2 * K3 + K4) / 6.0;
}

Vector Euler(const Vector &X, const Vector &Xdot, double h){

  return X + h * Xdot;
}

void XToxv(Vector3d &x, Vector3d &p, Vector3d &l, Quaternion &q, const Vector &X){
  x.x = X[0];
  x.y = X[1];
  x.z = X[2];
  p.x = X[3];
  p.y = X[4];
  p.x = X[5];
  l.x = X[6];
  l.y = X[7];
  l.z = X[8];
  q.w = X[9];
  q.x = X[10];
  q.y = X[11];
  q.z = X[12];
}

void xvToX(const Vector3d &x, const Vector3d &p, const Vector3d &l, const Quaternion &q, Vector &X){
  X[0] = x.x;
  X[1] = x.y;
  X[2] = x.z;
  X[3] = p.x;
  X[4] = p.y;
  X[5] = p.z;
  X[6] = l.x;
  X[7] = l.y;
  X[8] = l.z;
  X[9] = q.w;
  X[10] = q.x;
  X[11] = q.y;
  X[12] = q.z;
}

int isresting(const Vector3d &x, const Vector3d &v, const Plane &pl){

  if(Abs((x - O) * pl.n) < EPS && pl.h * Abs(v * pl.n) < EPS)
    return 1;
  else
    return 0;
}

int isstopped(const Vector3d &v, Plane &pl){

  if(pl.h * Abs(v * pl.tangent) < EPS)
    return 1;
  else
    return 0;
}

//
// Test for resting contact and/or stopped due to friction, and adjust position
// and velocity to exact conditions
//
void testresting(int &resting, int &stopped, Vector3d &x, Vector3d &v, const Plane &pl){
  resting = isresting(x, v, pl);
  if(resting){
    x = x - ((x - O) * pl.n) * pl.n;
    stopped = isstopped(v, pl);
    if(stopped)
      v.set(0.0, 0.0);
    else
      v = v - (v * pl.n) * pl.n;
  }
}

Matrix3x3 star(const Vector3d &omega) {
    Matrix3x3 w_star;

    w_star.set(0, -omega.z, omega.y, omega.z, 0, -omega.x, -omega.y, omega.x, 0);

    return w_star;
}
//
// System dynamics. Given current state and time, return rate of change of state
//
Vector dynamics(const Vector &X, double t, int resting, int stopped){
  Vector3d x, p, l;
  Quaternion q;
  Vector newXdot(13);
  Vector3d F;
  double fn, ft, vt;
  double friction;

  // compute total spring and gravity force
  XToxv(x, p, l, q, X);
  F = pl.m * g - sp.k * ((x - x0).norm() - sp.l0) * (x - x0).normalize();

  // if in resting contact compute the friction force and support force
  fn = F * pl.n;		  // force component normal to the surface
  if(resting && fn < 0){

    if(stopped){    // static friction
      ft = F * pl.tangent;   // force component tangent to the surface
      friction = -Min(Abs(pl.mus * fn), Abs(ft)) * Sign(ft);
    }
    else{	    // dynamic friction
      vt = v * pl.tangent;   // velocity component tangent to the surface
      friction = pl.mud * fn * Sign(vt);
    }

    // add the friction force
    F = F + friction * pl.tangent;

    // subtract support force normal to the resting surface
    F = F - fn * pl.n;
  }

  // build the state rate of change vector
  newXdot[0] = v.x;
  newXdot[1] = v.y;
  newXdot[2] = F.x / pl.m;
  newXdot[3] = F.y / pl.m;

  return newXdot;
}


//
// Run the simulation 1 time step, then loop infinitely via a timer callback
//
void simulate(int timerno){

  Vector Xnew(4);
  Vector2d xnew, vnew;
  Vector Xc(4);
  Vector2d xc, vc;
  Vector2d a;
  double fc;

  Vector dX(4);
  Vector2d dx, dv;

  // kill the simulation if stopped
  if(halted)
    return;

 // time step in use starts out being the base time step
  double h = p.h;
  do{
    // check for resting contact, and if resting set resting and stopped flags and
    // adjust position and velocity to exact resting contact
    testresting(resting, stopped, x, v);

    // compute the rate of change of state
    xvToX(x, v, X);
    Xdot = dynamics(X, t, resting, stopped);

    // clear resting contact effect if acceleration is away from the resting surface
    a.set(Xdot[2], Xdot[3]);
    if(a * p.n > 0){
      resting = 0;
      stopped = 0;
    }

    // now that we know everything, we can draw the current state
    drawState();
    //sysprint(x, v, resting, Xdot, t);

    // find out the change that would result from an Euler integration step
    dX = p.h * Xdot;
    XToxv(dx, dv, dX);

    // compute some useful stuff
    double dvt = dv * p.tangent;    // tangential change in velocity
    double vt = v * p.tangent;      // current tangential velocity
    double dxn = dx * p.n;	    // change in distance from surface
    double d = (x - O) * p.n;       // current distance from surface

    // if collision or slide to zero velocity in this timestep, then catch the event
    // using an Euler partial step, then complete the timestep in Euler
    // note: this way of testing avoids possible divide by zero
    if((resting && vt * dvt < 0 && Abs(vt) < Abs(dvt)) ||
       (!resting && d * dxn < 0 && Abs(d) < Abs(dxn))){

      // time to the event: resting slide-to-stop time fraction, !resting collision
      fc = resting? -vt / dvt: -d / dxn;

      // collision or stop, take an Euler step exactly to the critical point
      Xc = Euler(X, Xdot, fc * h);

      // cancel any normal component of the velocity
      XToxv(x, v, Xc);
      v = v - (v * p.n) * p.n;

      // state and time now at collision point, set h to time remaining in step
      xvToX(x, v, Xnew);
      t += fc * h;
      h = (1.0 - fc) * h;
    }
    else{
      if(h != p.h)      // partial step, so finish using Euler
	Xnew = Euler(X, Xdot, h);
      else		// full step, no collision or stop, so take a single full RK4 step
	Xnew = RK4(X, Xdot, t, h, resting, stopped);
      t += h;
      h = 0;
    }
  }while(Abs(h) > EPS);

  // update state and advance to next time step
  XToxv(x, v, Xnew);

  if(!singlestep)
    glutTimerFunc(50, simulate, 1);    // 0.1 second timer
}
