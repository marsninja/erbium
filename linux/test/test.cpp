#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include "erb_engine.h"

using namespace erb;

void do_stuff();
static void redraw();

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutCreateWindow("Erbium Engine Test");

	glutDisplayFunc(redraw);	

	glEnable( GL_DEPTH_TEST );
	glMatrixMode(GL_PROJECTION);						//hello
	gluPerspective(45, //view angle
			1.0,	//aspect ratio
			10.0, //near clip
			1500.0);//far clip
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	do_stuff();

	glutMainLoop();

	return 0; 
}

construct c;
matrix m;

void do_stuff()
{
	std::ifstream in("eh.map", ios::in);
	u_long bnum;
	matrix m;
	in >> bnum;
	m.translate(0,0,-600);
	for(u_long i=0; i<bnum; i++)
	{
		brush bv;
		if(!bv.ascii_in(in)){cout << "Error" << endl;}
		bv.apply(m); 
    c.add(bv);
	}
	//c.add(b[0]); //c-=b[1];
	//c.ascii_out();
}

static void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	for(u_long l=0; l<c.br.size(); l++)
	{
		for(u_long i=0; i<c.br[l].num_polys(); i++)
		{
			glBegin(GL_POLYGON);
			glNormal3f(c.br[l][i].p.norm.x, c.br[l][i].p.norm.y, c.br[l][i].p.norm.z);

			for(long j=(long)(c.br[l][i].num_verts()-1); j>=0; j--)
			{
				//cout << j << ": "; b2[i][j].ascii_out(cout); cout << endl;

				glVertex3f(c.br[l][i][j].x, c.br[l][i][j].y, c.br[l][i][j].z);
			}
			glEnd();
		}
	}
	//rotate(b2, vector(1,0,0), -.01);
	matrix r;
	r.rotate_x(.05);
	r.rotate_y(.05);
	r.rotate_z(.05);
	for(u_long l=0; l<c.br.size(); l++)
	{
		c.apply_self(r);
	}
	glutSwapBuffers();
	glutPostRedisplay();
}
