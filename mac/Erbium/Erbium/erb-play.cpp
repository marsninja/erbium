//Written and Maintained by Jason Mars (JmarsKoder@gmail.com)

#include "erb_game.h"
#include <cmath>

using namespace erb;
using namespace erb::game;

static void init();
void setup();
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void key_up(unsigned char key, int x, int y);
void proc_g();
game_object prims, crims;
matrix m;
vector loc; double rot_y=0,forward_dir=0;
bool key_pressed[256];

int main(int argc, char *argv[])
{
    setup();
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA
                         | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow(argv[0]);
    init ();
    /*gluPerspective(45, //view angle
     1.0,	//aspect ratio
     10.0, //near clip
     1500.0);//far clip*/
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glutKeyboardUpFunc(key_up);              // Called When A Special Key Released (NEW)
    
    glutDisplayFunc (display);
    glutIdleFunc (display);
    glutMainLoop();
    return 0;
    
}

static void init(void)
{
    //glEnable (GL_CULL_FACE);
    //glCullFace (GL_BACK);
    glBlendFunc (GL_SRC_ALPHA_SATURATE, GL_ONE);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);							   // The Type Of Depth Testing To Do
    glEnable(GL_DEPTH_TEST);						   // Enables Depth Testing
    glEnable (GL_BLEND);
    glEnable (GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

void setup()
{
    //open necessary files
    /*ifstream pfile("builders.prim", ios::in);
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
     crims.add(b);
     cout << "Loaded " << s << endl;
     i++; cout << i << endl;
     }
     
     prims.formulate();
     crims.formulate();
     prims.ascii_out();
     m.rotate_x(20);m.translate(200,0,0);
     crims.apply(m);
     m.make_identity();*/
    string cgen=assembler::assemble(molokai::mol_compile("beauty.mol"));
    cout << cgen;
    istringstream code (cgen);
    erb_machine proc=erb_setup_machine(code);
    proc.execute();
    
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //glDisable (GL_DEPTH_TEST);
    glLoadIdentity();									// Reset The View
    
    
    proc_g();
    //prims.draw();
    //crims.draw();
    for(int i=0; i<renderlist.size();i++)
    {
        draw(renderlist[i]);
    }
    
    //rotate(prims,vector(1,0,1),.04);
    
    glFlush ();
    glutSwapBuffers();
    glutPostRedisplay();
}


void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1,-1,1,1.5,100000);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    key_pressed[key]=true;
}

void key_up(unsigned char key, int x, int y)
{
    key_pressed[key]=false;
}

void proc_g()
{
    if(key_pressed['x'])
        exit(0);
    if(key_pressed['w'])
    {
        loc.x += (double)sin(forward_dir*PI_OVER_180) * 0.5f;
        loc.z += (double)cos(forward_dir*PI_OVER_180) * 0.5f;
    }
    if(key_pressed['s'])
    {
        loc.x -= (double)sin(forward_dir*PI_OVER_180) * 0.5f;
        loc.z -= (double)cos(forward_dir*PI_OVER_180) * 0.5f;
    }
    if(key_pressed['q']) //Strafing
    {
        loc.z -= (double)sin(forward_dir*PI_OVER_180) * 0.5f;
        loc.x += (double)cos(forward_dir*PI_OVER_180) * 0.5f;
    }
    if(key_pressed['e'])
    {
        loc.z += (double)sin(forward_dir*PI_OVER_180) * 0.5f;
        loc.x -= (double)cos(forward_dir*PI_OVER_180) * 0.5f;
    }
    if(key_pressed['r']) //Fly
    {
        loc.y -= 0.05;
    }
    if(key_pressed['c'])
    {
        loc.y += 0.05;
    }
    
    if(key_pressed['a']) forward_dir+=.15;
    if(key_pressed['d']) forward_dir-=.15;
    
    
    
    rot_y=360.0-forward_dir;
    glRotated(rot_y, 0.0, 1.0, 0.0);
    glTranslatef(loc.x, loc.y, loc.z);
    
    /*case 'd':
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
     break;*/
}