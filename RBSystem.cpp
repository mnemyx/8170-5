/*
 *
 *  RBSystem.h
 *  GBG - Proj #5 - Rigid Bodies
 *
 */

#include "RBSystem.h"

// help function
Matrix3x3 star(const Vector3d &omega) {
    Matrix3x3 w_star;

    w_star.set(0, -omega.z, omega.y, omega.z, 0, -omega.x, -omega.y, omega.x, 0);

    return w_star;
}

RBSystem::RBSystem(int nbods) {
    nbodies = nbods;
    rblist = new RBody[nbodies];

    for(int i = 0; i < nbodies; i++)
        rblist[i].setRBI(i);

    Y.setSize(nbodies * STATE_SIZE);
    Ydot.setSize(nbodies * STATE_SIZE);
}

RBSystem::~RBSystem() {
    delete []rblist;
}

void RBSystem::setParams(double m[], double width[], double height[], double depth[], int type[], double d1[], double d2[], double d3[], Vector4d c[]) {
    //cout << "nbodies -- setParams(): " << nbodies << endl;
    for(int i = 0; i < nbodies; i++) {
        //cout << "width[" << i << "]: " << width[i] << endl;
        //cout << "height[" << i << "]: " << height[i] << endl;
        //cout << "depth[" << i << "]: " << depth[i] << endl;
        rblist[i].setParams(m[i], width[i], height[i], depth[i], type[i], d1[i], d2[i], d3[i]);
        rblist[i].setColor(c[i]);
    }
}

void RBSystem::setEnv(Vector3d g, Vector3d w, double v) {
    Env.G = g;
    Env.W = w;
    Env.Vis = v;
}

void RBSystem::setSpring(double k, double d, Vector3d p0, double l0, int rbinum, int ivert) {
    Spring.SetStrut(k, d, l0, p0);
    springmeta[0] = rbinum;
    springmeta[1] = ivert;
}

void RBSystem::initializeState(Vector3d x0[], Quaternion q[], Vector3d v0[], Vector3d omega0[]) {
    for(int i = 0; i < nbodies; i++) {
        rblist[i].initICs(x0[i], q[i], v0[i], omega0[i]);
        //rblist[i].print();
    }
}


/*
void RBSystem::stateToArr(int indx) {
    int i = indx * STATE_SIZE;

    Y[i++] = rblist[indx].getX().x;
    Y[i++] = rblist[indx].getX().y;
    Y[i++] = rblist[indx].getX().z;

    Y[i++] = rblist[indx].getQ().w;
    Y[i++] = rblist[indx].getQ().x;
    Y[i++] = rblist[indx].getQ().y;
    Y[i++] = rblist[indx].getQ().z;

    Y[i++] = rblist[indx].getP().x;
    Y[i++] = rblist[indx].getP().y;
    Y[i++] = rblist[indx].getP().z;

    Y[i++] = rblist[indx].getL().x;
    Y[i++] = rblist[indx].getL().y;
    Y[i++] = rblist[indx].getL().z;
}


void RBSystem::arrToState(int indx) {
    int i = indx * STATE_SIZE;
    Vector3d x, p, l;
    Quaternion q;

    x.set(Y[i++], Y[i++], Y[i++]);
    q.set(Y[i++], Y[i++], Y[i++], Y[i++]);
    p.set(Y[i++], Y[i++], Y[i++]);
    l.set(Y[i++], Y[i++], Y[i++]);

    rblist[indx].setICs(x, q, p, l);
}

void RBSystem::arrToBodies() {
    for(int i = 0; i < nbodies; i++)
        arrToState(i);
}

void RBSystem::bodiesToArr() {
    for(int i = 0; i < nbodies; i++)
        stateToArr(i);
}

void RBSystem::computeFT(double t, double, dt, int rindx) {
    Vector3d F;
}

void RBSystem::ddtStateToArr(int indx) {
    int i = indx * STATE_SIZE;

    Ydot[i++] = rblist[indx].getv().x;
    Ydot[i++] = rblist[indx].getv().y;
    Ydot[i++] = rblist[indx].getv().z;

    Quaternion w.set(0, rblist[indx].getw());
    Quaternion qdot = .5 * w * rblist[indx].getQ();

    Ydot[i++] = qdot.w;
    Ydot[i++] = qdot.x;
    Ydot[i++] = qdot.y;
    Ydot[i++] = qdot.z;

    Ydot[i++] = rblist[indx].getf().x;
    Ydot[i++] = rblist[indx].getf().y;
    Ydot[i++] = rblist[indx].getf().z;

    Ydot[i++] = rblist[indx].gett().x;
    Ydot[i++] = rblist[indx].gett().y;
    Ydot[i++] = rblist[indx].gett().z;
}

void RBSystem::dydt(double t, double dt, const Vector &X, const Vector &Xdot) {
    arrToBodies();

    for (int i = 0; i < nbodies; i++) {
        computeFT(t, dt, i);
        ddtStateToArr(i)
    }
}*/

void XtoState(Vector3d &x, Quaternion &q, Vector3d &p, Vector3d &l, const StateVector X) {
    x.set(X[0],X[1],X[2]);
    q.set(X[3],X[4],X[5],X[6]);
    p.set(X[7],X[8],X[9]);
    l.set(X[10],X[11], X[12]);
}

void StatetoX(const Vector3d x, const Quaternion q, const Vector3d p, const Vector3d l, StateVector &X) {
    X[0] = x.x;
    X[1] = x.y;
    X[2] = x.z;
    X[3] = q.q.w;
    X[4] = q.q.x;
    X[5] = q.q.y;
    X[6] = q.q.z;
    X[7] = p.x;
    X[8] = p.y;
    X[9] = p.z;
    X[10] = l.x;
    X[11] = l.y;
    X[12] = l.z;
}

StateVector dynamics(const StateVector &X, double t, double dt, int nbodies, const RBody &rb, const Strut &sp, int spi, const Environment &Env) {
    Vector3d x, p, l;
    Quaternion q;
    StateVector newXdot(nbodies * STATE_SIZE);
    Vector3d F, T, V, Q;
    Vector3d fg, fs, fd;
    Vector3d ts, tg, td;
    Vector3d w;
    Matrix3x3 r, iinv;
    Quaternion wq, qdot;

    XtoState(x, q, p, l, X);

    //cout << "x: " << x << endl;
    //cout << "q: " << q << endl;
    //cout << "p: " << p << endl;
    //cout << "l: " << l << endl;

    // calc velocity
    V = p / rb.getM();
    //cout << "M: " << rb.getM() << endl;
    //cout << "V: " << V << endl;
    // calc rate of change of q:
    r = q.normalize().rotation();
    iinv = r * rb.getIbodyinv() * r.transpose();

    //cout << "iinv: \n";
    //iinv.print();

    w = iinv * l;
    wq.set(w);
    //cout << endl << "w: " << w << endl;
    //cout << endl << "wq: " << wq << endl;

    Q = 0.5 * wq * q;
    //cout << "Q: " << Q << endl;
    // calc force
    //fg = rb.getM() * Env.G;

    if (Env.W.x == 0 && Env.W.y == 0 && Env.W.z == 0)
        fg = (Env.G - Env.Vis * V ) * rb.getM();
    else
        fg = (Env.G + Env.Vis * (Env.W - V)) * rb.getM();


    Vector3d p1 = rb.getvertex(spi);

    //fs = - (sp.GetK() / rb.getM()) * ((p1 - sp.GetP0()).norm() - sp.GetL0()) * (p1 - sp.GetP0()).normalize();
    //fd = - (sp.GetD() / rb.getM()) * ((V * (p1 - sp.GetP0()).normalize()) * (p1 - sp.GetP0()).normalize());
    //cout << "fs: " << fs << endl;

    fs = -sp.GetK() * ((p1 - sp.GetP0()).norm() - sp.GetL0()) * (p1 - sp.GetP0()).normalize();

    fd = -sp.GetD() * ((V * (p1 - sp.GetP0()).normalize()) * (p1 - sp.GetP0()).normalize());

    F = fg + fs + fd;


    // calc torque
    //    ts = (p1 - x).norm() % (fs);
    //    td = (p1 - x).norm() % (fd);

    ts = (p1 - x) % fs;

    td = (p1 - x) % fd;

    T = ts + td;

    StatetoX(V, Q, F, T, newXdot);

    return newXdot;
}

StateVector RK4(const StateVector &X, const StateVector &Xdot, double t, double dt, int nbodies, const RBody &rb, const Strut &sp, int spi, const Environment &env) {
    StateVector K1(nbodies * STATE_SIZE), K2(nbodies * STATE_SIZE);
    StateVector K3(nbodies * STATE_SIZE), K4(nbodies * STATE_SIZE);

    K1 = dt * Xdot;
    K2 = dt * dynamics(X + 0.5 * K1, t + 0.5 * dt, dt, nbodies, rb, sp, spi, env);
    K3 = dt * dynamics(X + 0.5 * K2, t + 0.5 * dt, dt,  nbodies, rb, sp, spi, env);
    K4 = dt * dynamics(X + K3, t + dt, dt, nbodies, rb, sp, spi, env);

    return X + (K1 + 2 * K2 + 2 * K3 + K4) / 6.0;
}

void RBSystem::takeTimestep(double t, double dt) {
    StateVector Xnew;
    Quaternion q;
    Vector3d x, p, l;

    // compute the rate of change of state
    for(int i = 0; i < nbodies; i ++) {
        StatetoX(rblist[i].getX(), rblist[i].getQ(),
                 rblist[i].getP(), rblist[i].getL(), Y);
        //cout << "Y: \n" << endl;
        //Y.print();
        Ydot = dynamics(Y, t, dt, nbodies, rblist[i], Spring, springmeta[1], Env);

        //cout << "Ydot: \n" << endl;
        //Ydot.print();

        Xnew = RK4(Y, Ydot, t, dt, nbodies, rblist[i], Spring, springmeta[1], Env);
        XtoState(x, q, p, l, Xnew);

        rblist[i].setICs(x, q, p, l);
    }

    printsys();
}

void RBSystem::drawSys(){
    // draw strut/spring
    for(int i = 0; i < nbodies; i++)
        rblist[i].drawbody();

    cout << "Spring.GetP0(): " << Spring.GetP0() << endl;
    cout << "rblist[springmeta[0]].getvertex(springmeta[1]): " << rblist[springmeta[0]].getvertex(springmeta[1]) << endl;
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
        glLineWidth(1);
        glColor3f(1,1,1);
        glVertex3f(Spring.GetP0().x, Spring.GetP0().y, Spring.GetP0().z);
        glVertex3f(rblist[springmeta[0]].getvertex(springmeta[1]).x, rblist[springmeta[0]].getvertex(springmeta[1]).y, rblist[springmeta[0]].getvertex(springmeta[1]).z);
    glEnd();
    glDisable(GL_LINE_SMOOTH);
}

void RBSystem::printsys() {
    cout << "# OF RIGID BODIES: " << nbodies << endl << "     ";
    for (int i = 0; i < nbodies; i++)
        rblist[i].print();
    cout << endl << "Y: " << endl << "     ";
        Y.print();
    cout << endl << "Ydot: " << endl << "     ";
        Ydot.print();
    cout << endl << "Spring (attached to RBI: " << springmeta[0] << ", vertex index: " << springmeta[1] << "): " << endl << "     ";
        Spring.PrintStrut();
    cout << endl << "Environment: " << endl << "     ";
        cout << "G: " << Env.G << endl << "     ";
        cout << "W: " << Env.W << endl << "     ";
        cout << "Viscosity: " << Env.Vis << endl;

}
