//
// Code to test and visualize 2D rigid body code
//
// NSF CCLI Project    Donald H. House         6/20/08
//

#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "RBSystem.h"
#include "RBody.h"

using namespace std;

const int WIDTH = 800;    // initial window dimensions
const int HEIGHT = 600;

const int NEAR = 10;
const int FAR = 1000;
const int DEPTH = -500;

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

/// variables above are for camera position and shading ///

const float WHITE[] = {1, 1, 1, 1};
const float BRIGHT_PALEBLUE[] = {0.5, 0.5, 1, 1};

/// above are colors ///

const int NUMBODIES = 3;

const int TimerDelay = 100; // 1/10 second delay between time steps

const double dt = .05;
static double t;

static bool Stopped;
static bool Step;
static bool Started;

static char *Filename = NULL;

static RBody Cube;
static RBSystem *RBSys = NULL;

/************************* END OF GLOBAL VARIABLES *******************************/

//
// Read in parameter files
//
void loadParams(char *file) {
    ifstream indata;
    char check[100];
    int numof;
    double vars[21];

    indata.open(file);
    if(!indata) {
        cerr << "Could not open the parameter file." << endl;
        exit(1);
    }

    Filename = file;

    while(indata >> check) {
        if(strcmp(check, "environment") == 0) {
        cout << " LOADING ENVIRONMENT DATA " << endl;
            for(int i=0; i<7; i++)
                indata >> vars[i];

            RBSys->setEnv(Vector(vars[0], vars[1], vars[2]), Vector(vars[3], vars[4], vars[5]), vars[6]);

        } else if (strcmp(check, "spring") == 0) {
        cout << " LOADING SPRING DATA " << endl;
            for(int i = 0; i < 8; i++){
                indata >> vars[i];
                cout << "vars[" << i << "]: " <<  vars[i] << endl;
                }
            RBSys->setSpring(vars[0], vars[1], Vector(vars[2], vars[3], vars[4]), vars[5], vars[6], vars[7]);

        } else if (strcmp(check, "rbodies") == 0) {
        cout << "LOADING RBODY DATA " << endl;
            indata >> numof;
            double m[numof], w[numof], h[numof], d[numof], d1[numof], d2[numof], d3[numof];
            int type[numof];
            Quaternion q[numof];
            Vector3d x0[numof], v0[numof], o0[numof];
            Vector4d c[numof];

            for(int k = 0; k < numof; k++) {
                for(int i = 0; i < 25; i++) {
                    indata >> vars[i];

                //cout << "vars[" << i << "]: " <<  vars[i] << endl;
                }
                m[k] = vars[0];
                w[k] = vars[1];
                h[k] = vars[2];
                d[k] = vars[3];
                type[k] = vars[4];
                d1[k] = vars[5];
                d2[k] = vars[6];
                d3[k] = vars[7];
                x0[k].set(vars[8], vars[9], vars[10]);
                q[k].set(vars[11], vars[12], vars[13], vars[14]);
                v0[k].set(vars[15], vars[16], vars[17]);
                o0[k].set(vars[18], vars[19], vars[20]);
                c[k].set(vars[21], vars[22], vars[23], vars[24]);
            }

            RBSys = new RBSystem(numof);
            //cout << " HIHIHIHIH " << endl;
            RBSys->setParams(m, w, h, d, type, d1, d2, d3, c);
            RBSys->initializeState(x0, q, v0, o0);

        }
    }

    RBSys->printsys();

    indata.close();
}

void Initialize(char *file){

  loadParams(file);

  //Cube.setParams(1.0, 25.0, 25.0, 25.0, 0, 0.0, 0.0, 0.0);
    //Cube.print();
  t = 0;

  Stopped = true;
  Started = true;
  Step = false;
}

//
// Display Callback Routine: clear the screen and draw the cat
//
void drawScreen(){
  const float light_position1[] = {1, 1, 1, 1};
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
  //glEnable(GL_LIGHT1);

  // establish camera coordinates
  glRotatef(Tilt, 1, 0, 0);	    // tilt - rotate camera about x axis
  glRotatef(Pan, 0, 1, 0);	    // pan - rotate camera about y axis
  glTranslatef(0, 0, Approach);     // approach - translate camera along z axis

  // rotate the model
  glRotatef(ThetaY, 0, 1, 0);       // rotate model about x axis
  glRotatef(ThetaX, 1, 0, 0);       // rotate model about y axis

  // draw the rigid bodies
  RBSys->drawSys();
  //Cube.drawbody();
  glutSwapBuffers();
}

void getShading() {
    float ambient_color[4];
    float diffuse_color[4];
    float specular_color[4];
    int shininess;


    // set up material colors to current hue.
    for(int i = 0; i < 3; i++)
      ambient_color[i] = diffuse_color[i] = specular_color[i] = 0;
    ambient_color[3] = diffuse_color[3] = specular_color[3] = 1;
    shininess = 1;


    for(int i = 0; i < 3; i++){
      ambient_color[i] = AMBIENT_FRACTION * BRIGHT_PALEBLUE[i];
      diffuse_color[i] = DIFFUSE_FRACTION * BRIGHT_PALEBLUE[i];
      specular_color[i] = SPECULAR_FRACTION * WHITE[i];
      shininess = 60;
    }

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

//
// Motion event handler, runs on a timer
//
void handleTimeStep(int n){
    if(Stopped)		// freeze if stopped
        return;

    RBSys->takeTimestep(t, dt);

    //cout << "taking time step" << endl;
    //RBSys->printsys(); cout << endl;

    t += dt;

    drawScreen();
    glutPostRedisplay();		// make sure it gets displayed

    if(Step){
        Stopped = true;
        Step = false;
    } else {
        Stopped = false;
        glutTimerFunc(TimerDelay, handleTimeStep, 0); // and move again after another delay
    }
}

// Init Camera
 void InitCamera() {
  glDisable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  //glDisable(GL_BLEND);

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

  Initialize(Filename); // reload parameters in case changed

  glutIdleFunc(NULL);
  t = 0;
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

  if(argc != 2){
    cerr << "usage: rbody paramfile\n";
    exit(1);
  }

  Initialize(argv[1]);

  // start up the glut utilities
  glutInit(&argc, argv);

  // create the graphics window, giving width, height, and title text
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Rigid Body Simulation");

  InitCamera();

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

