//Written and Maintained by Jason Mars (JmarsKoder@gmail.com)

#include "erb_game.h"

using namespace erb;
using namespace erb::game;

static void init();
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
game_object prims;
matrix m;

int main(int argc, char *argv[])
{
  //open necessary files
  ifstream pfile("builders.prim", ios::in);
  if(!pfile.is_open())
  {
    cout << "builders.prim no present" << endl; 
    exit(0);
  }

  //load in the builders
  int i=0;
  while(!pfile.eof())
  {
    string s;
    pfile >> s; 
    if(pfile.eof()) break;
    brush b;
    b.ascii_in(pfile);
    prims.add(b);
    cout << "Loaded " << s << endl;
    i++; cout << i << endl;
  }

  prims.formulate();
  prims.ascii_out();


   
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB 
                        | GLUT_ALPHA | GLUT_DEPTH);
   glutInitWindowSize(800, 800);
   glutCreateWindow(argv[0]);
   init ();
   glutReshapeFunc (reshape);
   glutKeyboardFunc (keyboard);
   glutDisplayFunc (display);
   glutIdleFunc (display);
   glutMainLoop();
   return 0;

}

static void init(void)
{
   glCullFace (GL_BACK);
   glEnable (GL_CULL_FACE);
   glBlendFunc (GL_SRC_ALPHA_SATURATE, GL_ONE);
   glShadeModel(GL_SMOOTH);
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

bool look=0;
void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glEnable (GL_BLEND);
   glEnable (GL_POLYGON_SMOOTH);
   glDisable (GL_DEPTH_TEST);
   if(!look)
   {gluLookAt(0,20,-50,0,0,0,0,1,0);
     look=1;}
   prims.draw();
   //rotate(prims,vector(1,0,1),.01);

   glFlush ();
   glutSwapBuffers();
}


void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-1.0, 1,-1,1,1.5,1000);
   glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
      case 'q':
         exit(0);
         break;
      case 'w':
        prims.ascii_out();
        break;
      case 's':
        break;
      case 'd':
        m.scale(1,1,1.5);
        prims.apply(m);
        m.make_identity(); 
        break;
      case 'a':
        m.scale(1.5,1,1);
        prims.apply(m);
        m.make_identity(); 
        break;
      case 'r':
        break;
      default:
         break;
   }
}
