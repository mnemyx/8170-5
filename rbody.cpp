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

const int NUMBODIES = 3;

const int TimerDelay = 100; // 1/10 second delay between time steps

const double dt = 0.005;
static double t;

static bool Stopped;
static bool Step;

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
  Step = false;
}

//
// Display Callback Routine: clear the screen and draw the cat
//
void drawScreen(){  
  // clear the window to the background color
  glClear(GL_COLOR_BUFFER_BIT);

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

//
// Mouse button event handler, called when button pressed or released
//
void handleButtons(int button, int state, int x, int y){
  
  if(state != GLUT_UP)
    return;
    
  y = HEIGHT - y;
  
  if(button == GLUT_LEFT_BUTTON){
    Stopped = !Stopped;
    if(!Stopped)
      handleTimeStep(0);
  }
  else if(button == GLUT_RIGHT_BUTTON && Stopped){
    Step = true;
    Stopped = false;
    handleTimeStep(0);
  }
  else if(button == GLUT_MIDDLE_BUTTON){
    Initialize();
    glutPostRedisplay();
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
      
    default:		// not a valid key -- just ignore it
      return;
  }
  
  glutPostRedisplay();  
}

//
// Main program to initialize the rigid body system and the display
//
int main(int argc, char* argv[]){
  
  Initialize();
  
  // start up the glut utilities
  glutInit(&argc, argv);

  // create the graphics window, giving width, height, and title text
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Rigid Body Simulation");

  // drawing callback routine
  glutDisplayFunc(drawScreen);
  
  // mouse button callback routine
  glutMouseFunc(handleButtons);
  
  // keyboard callback routine
  glutKeyboardFunc(handleKeys);
  
  // Timer callback routine
  glutTimerFunc(TimerDelay, handleTimeStep, 0);

  // specify window clear (background) color to be black
  glClearColor(0, 0, 0, 1);
  
  // establish screen coordinate system
  glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WIDTH / 200.0, WIDTH / 200.0, -HEIGHT / 200.0, HEIGHT / 200.0);
  glMatrixMode(GL_MODELVIEW);
  
  glutMainLoop();
  return 0;
}

