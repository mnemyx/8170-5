//
// Code to test and visualize 2D rigid body code
//
// NSF CCLI Project    Donald H. House         6/20/08
//

#include <iostream>       // definitions for C++ input/output
#include <cstdlib>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "RBSystem.h"
#include "Matrix.h"
#include "Geometry.h"
#include "Plane.h"
#include "Witness.h"

using namespace std;

const int WIDTH = 800;    // initial window dimensions
const int HEIGHT = 600;

const int NEAR = 10;
const int FAR = 1000;
const int DEPTH = -1000;

const int NONE = -1;

const double ROTFACTOR = 0.2;
const double XLATEFACTOR = 0.5;

const int DRAWWIDTH = 640;
const int DRAWHEIGHT = 480;

const double AMBIENT_FRACTION = 0.1;
const double DIFFUSE_FRACTION = 0.2;
const double SPECULAR_FRACTION = 0.2;

static double Pan;
static double Tilt;
static double Approach;

// model orientation
static double ThetaX;
static double ThetaY;

// global variables to track mouse and shift key
static int MouseX;
static int MouseY;
static int Button = NONE;

static double WinWidth = WIDTH;
static double WinHeight = HEIGHT;
static int MiddleButton = false;

/// variables above are for camera position and shading ///

const float WHITE[] = {1, 1, 1, 1};

/// above are colors ///

const int NUMBODIES = 3;

const int TimerDelay = 100; // 1/10 second delay between time steps

const double dt = 0.005;
static double t;

static bool Stopped;
static bool Step;
static bool Started;

RBSystem rbsystem(NUMBODIES);

void Initialize(){
  double mass[] = {1.0, 1.0, 1.0};
  double moi[] = {1.0, 1.0, 1.0};
  int type[] = {RECT_PRISM, RECT_PRISM, RECT_PRISM};
  double dim1[] = {1.0, 2.0, 1.75};
  double dim2[] = {0.2, 1.5, 1.0};

  rbsystem.setParams(mass, moi, type, dim1, dim2);

  Vector2d x0[NUMBODIES], v0[NUMBODIES];
  double theta0[NUMBODIES], omega0[NUMBODIES];

  x0[0].set(-2, 2);
  theta0[0] = DegToRad(15);
  v0[0].set(2, -2);
  omega0[0] = DegToRad(60);

  x0[1].set(1, -2);
  theta0[1] = DegToRad(-15);
  v0[1].set(-1, 1);
  omega0[1] = DegToRad(-45);

  x0[2].set(1.5, 1.5);
  theta0[2] = DegToRad(0);
  v0[2].set(-1, -1);
  omega0[2] = DegToRad(80);

  t = 0;
  rbsystem.initializeState(x0, theta0, v0, omega0);

  Stopped = true;
  Started = true;
  Step = false;
}

//
// Display Callback Routine: clear the screen and draw the cat
//
void drawScreen(){
  const float light_position1[] = {-1, 1, -1, 1};
  const float light_position2[] = {1, 1, 1, 1};

  // clear the window to the background color
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);  // solid - clear depth buffer
  // establish shading model, flat or smooth
  glShadeModel(GL_SMOOTH);

  // light is positioned in camera space so it does not move with object
  glLoadIdentity();
  glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
  glLightfv(GL_LIGHT0, GL_AMBIENT, WHITE);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
  glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);

  glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
  glLightfv(GL_LIGHT1, GL_AMBIENT, WHITE);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, WHITE);
  glLightfv(GL_LIGHT1, GL_SPECULAR, WHITE);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  // establish camera coordinates
  glRotatef(Tilt, 1, 0, 0);	    // tilt - rotate camera about x axis
  glRotatef(Pan, 0, 1, 0);	    // pan - rotate camera about y axis
  glTranslatef(0, 0, Approach);     // approach - translate camera along z axis

  // rotate the model
  glRotatef(ThetaY, 0, 1, 0);       // rotate model about x axis
  glRotatef(ThetaX, 1, 0, 0);       // rotate model about y axis

  // draw the rigid bodies
  rbsystem.Draw();

  glutSwapBuffers();
}

//
// Motion event handler, runs on a timer
//
void handleTimeStep(int n){
  if(Stopped)		// freeze if stopped
    return;

  rbsystem.TakeTimestep(t, dt);

  glutPostRedisplay();		// make sure it gets displayed

  if(Step){
    Stopped = true;
    Step = false;
  }
  else
    glutTimerFunc(TimerDelay, handleTimeStep, 0); // and move again after another delay
}

// Init Camera
void InitCamera() {
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  Pan = 0;
  Tilt = 0;
  Approach = DEPTH;

  ThetaX = 0;
  ThetaY = 0;
}

//
// Routine to restart simulation
//
void RestartSim(){

  Initialize(); // reload parameters in case changed

  glutIdleFunc(NULL);

  drawScreen();
}

// handles updating the projection
void updateProjection(){

  // initialize the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  double scale = fabs((double)NEAR / (double)DEPTH);
  double xmax = scale * DRAWWIDTH / 2;
  double ymax = scale * DRAWHEIGHT / 2;
  glFrustum(-xmax, xmax, -ymax, ymax, NEAR, FAR);

  glMatrixMode(GL_MODELVIEW);
}

//
// handles reshape callback
//
void doReshape(int w, int h){

  glViewport(0, 0, w, h);
  WinWidth = w;
  WinHeight = h;

  updateProjection();
}

//
// Mouse button event handler, called when button pressed or released
//
void handleButtons(int button, int state, int x, int y){
  if(state == GLUT_UP)
      Button = NONE;		// no button pressed
    else {
      MouseY = -y;			// invert y window coordinate to correspond with OpenGL
      MouseX = x;

      Button = button;		// store which button pressed
    }
}

//
// Mouse button event handler, called when button pressed or released
//
void handleKeys(unsigned char key, int x, int y){
  switch(key){
    case 'q':		// q - quit
    case 'Q':
    case 27:		// esc - quit
      exit(0);

  case 's':
    case 'S':
      Step = !Step;
      if(Step) cout << "Stepped Mode";
      else cout << "Continous Mode";
      break;

    case 'd':
    case 'D':
        if(Started) {
            Started = false;
            Stopped = false;
            drawScreen();
            handleTimeStep(0);
        } else if(Stopped) {
            Stopped = false;
            handleTimeStep(0);
        } else {
            Stopped = true;
            glutIdleFunc(NULL);
        }
        break;

    case 'r':
    case 'R':
        RestartSim();
      break;

    default:		// not a valid key -- just ignore it
      return;
  }

  glutPostRedisplay();
}

//
//  Watch mouse motion
//
void handleMotion(int x, int y){
    int delta;

    y = -y;
    int dy = y - MouseY;
    int dx = x - MouseX;

    switch(Button){
      case GLUT_LEFT_BUTTON:
        ThetaX -= ROTFACTOR * dy;
        ThetaY += ROTFACTOR * dx;
        glutPostRedisplay();
        break;
      case GLUT_MIDDLE_BUTTON:
        Pan -= ROTFACTOR * dx;
        Tilt += ROTFACTOR * dy;
        glutPostRedisplay();
        break;
      case GLUT_RIGHT_BUTTON:
        delta = (fabs(dx) > fabs(dy)? dx: dy);
        Approach += XLATEFACTOR * delta;
        glutPostRedisplay();
        break;
    }

    MouseX = x;
    MouseY = y;
}
//
// Main program to initialize the rigid body system and the display
//
int main(int argc, char* argv[]){

  Initialize();

  // start up the glut utilities
  glutInit(&argc, argv);

  InitCamera();

  // create the graphics window, giving width, height, and title text
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Rigid Body Simulation");

  // drawing callback routine
  glutDisplayFunc(drawScreen);

  // reshape callback routing
  glutReshapeFunc(doReshape);

  // mouse button callback routine
  glutMouseFunc(handleButtons);

  // keyboard callback routine
  glutKeyboardFunc(handleKeys);

  // motion callback routin
  glutMotionFunc(handleMotion);

  // Timer callback routine
  glutTimerFunc(TimerDelay, handleTimeStep, 0);

  // specify window clear (background) color to be black
  glClearColor(0, 0, 0, 1);

  glutMainLoop();
  return 0;
}

