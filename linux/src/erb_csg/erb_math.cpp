/*---------------------------------------------------------------------------
Erb Engine Math Library

Includes all the 3d Math classes.
Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "erb_math.h"
#include <iomanip>
#include <cmath>

namespace erb {
//Util Functions-------------------------------------------------------------
void rotate(vertex &v, vector axis, double ang)
{
  if((v.x==0)&&(v.y==0)&&(v.z==0)) return;
    
  vector w;
  double c, s, t;
      
  axis.normalize();
      
  //calculate parameters of the rotation matrix
  c = cos(ang*(PI/180));
  s = sin(ang*(PI/180));
  t = 1 - c;
      
  //multiply v with rotation matrix
  w.x = (t * axis.x * axis.x +          c) * v.x
      + (t * axis.x * axis.y + s * axis.z) * v.y
      + (t * axis.x * axis.z - s * axis.y) * v.z;
            
  w.y = (t * axis.x * axis.y - s * axis.z) * v.x 
      + (t * axis.y * axis.y +          c) * v.y 
      + (t * axis.y * axis.z + s * axis.x) * v.z;
             
  w.z = (t * axis.x * axis.z + s * axis.y) * v.x 
      + (t * axis.y * axis.z - s * axis.x) * v.y 
      + (t * axis.z * axis.z +          c) * v.z;
      
  w.normalize();
  w = w * ((vector)v).length();
          
  v=w;
}
//------------------------------------------------------------------------END

//Point Class----------------------------------------------------------------
vertex::vertex(double xv, double yv, double zv, double wv):
  x(xv),y(yv),z(zv),w(wv){}

void vertex::set(double xv, double yv, double zv, double wv)
{
  x=xv; y=yv; z=zv; w=wv;
}

void vertex::negate()
{
  x=-x; y=-y; z=-z; //w=-w;
}

void vertex::operator+=(const vertex &v)
{
  x+=v.x; y+=v.y; z+=v.z; //w+=v.w;
}

void vertex::operator-=(const vertex &v)
{
  x-=v.x; y-=v.y; z-=v.z; //w-=v.w;
}

vertex vertex::operator+(const vertex &v) const
{
  return vertex(x+v.x, y+v.y, z+v.z); //, w+v.w);
}

vertex vertex::operator-(const vertex &v) const
{
  return vertex(x-v.x, y-v.y, z-v.z); //, w-v.w);
}

vertex vertex::operator*(const matrix &m) const
{
  return vertex(
      x*m.mat[0][0] + y*m.mat[1][0] + z*m.mat[2][0] + w*m.mat[3][0],
      x*m.mat[0][1] + y*m.mat[1][1] + z*m.mat[2][1] + w*m.mat[3][1],
      x*m.mat[0][2] + y*m.mat[1][2] + z*m.mat[2][2] + w*m.mat[3][2],
      x*m.mat[0][3] + y*m.mat[1][3] + z*m.mat[2][3] + w*m.mat[3][3]
      );
}

bool vertex::operator==(const vertex &v) const
{
  return (is_equal(x, v.x)&&is_equal(y, v.y)&&is_equal(z, v.z));
}

bool vertex::ascii_in(istream &in)
{
  char c;
  in >> c;
  if(c!='(') return false;
  in >> x >> y >> z >> c;
  if(c!=')') return false;
  return true; 
}

void vertex::ascii_out(ostream &out)
{
  out << setprecision(15) << "( " << x << " " << y << " " << z << " )";
}

bool vertex::bin_in(istream &in)
{
  in.read((char*)&x, sizeof(double));
  in.read((char*)&y, sizeof(double));
  in.read((char*)&z, sizeof(double));
  return true; 
}
void vertex::bin_out(ostream &out)
{
  out.write((char*)&x, sizeof(double));
  out.write((char*)&y, sizeof(double));
  out.write((char*)&z, sizeof(double));
}
//------------------------------------------------------------------------END

//Vector Class---------------------------------------------------------------
vector::vector(double xv, double yv, double zv, double wv):
  vertex(xv,yv,zv,wv){}
  
vector::vector(const vertex &p):
  vertex(p){}
  
double vector::length()
{
  return sqrt(x*x+y*y+z*z);
}

void vector::normalize()
{
  double len=length();
  if(len!=0) {x=x/len; y=y/len; z=z/len;}
}

vector vector::cross(const vector &v) const
{
  return vector(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x, 1.0);
}

double vector::dot(const vector &v) const
{
  return x*v.x+y*v.y+z*v.z;
}

void vector::operator*=(const vector &v)
{
  x*=v.x; y*=v.y; z*=v.z; //w*=v.w;
}

vector vector::operator*(const vector &v) const
{
  return vector(x*v.x, y*v.y, z*v.z); //, w*v.w);
}

vector vector::operator*(double m) const
{
  return vector(x*m, y*m, z*m);
}

vector vector::operator%(const vector &v) const
{
  return cross(v);
}
//------------------------------------------------------------------------END


//Matrix Class---------------------------------------------------------------
matrix::matrix(){make_identity();}

void matrix::make_identity()
{
  memset(&mat, 0, sizeof(mat));
  mat[0][0]=mat[1][1]=mat[2][2]=mat[3][3]=1.0;
}

void matrix::rotate_x(double ang)
{
  matrix m;
  double c=cos(ang*(PI/180));
  double s=sin(ang*(PI/180));
  m.mat[1][1]=c;  m.mat[1][2]=s;
  m.mat[2][1]=-s; m.mat[2][2]=c;
  *this*=m;
}

void matrix::rotate_y(double ang)
{
  matrix m;
  double c=cos(ang*(PI/180));
  double s=sin(ang*(PI/180));
  m.mat[0][0]=c;  m.mat[0][2]=-s; 
  m.mat[2][0]=s;  m.mat[2][2]=c;
  *this*=m;
}
void matrix::rotate_z(double ang)
{
  matrix m;
  double c=cos(ang*(PI/180));
  double s=sin(ang*(PI/180));
  m.mat[0][0]=c;  m.mat[0][1]=s;
  m.mat[1][0]=-s; m.mat[1][1]=c;
  *this*=m;
}

void matrix::scale(double x, double y, double z)
{
  matrix m;
  m.mat[0][0]=x; m.mat[1][1]=y; m.mat[2][2]=z;
  *this*=m;
}

void matrix::translate(double x, double y, double z)
{
  matrix m;
  m.mat[3][0]=x; m.mat[3][1]=y; m.mat[3][2]=z;
  *this*=m;
}

void matrix::operator*=(const matrix &m)
{
  matrix m2;
  double ab;
  for(int i=0; i<4; i++)
    for(int j=0; j<4; j++)
    {
      ab=0;
      for(int k=0; k<4; k++)
        ab+=mat[i][k]*m.mat[k][j];
      m2.mat[i][j]=ab;
    }
  *this=m2;
}

matrix matrix::operator*(const matrix &m) const
{
  matrix m2;
  double ab;
  for(int i=0; i<4; i++)
    for(int j=0; j<4; j++)
    {
      ab=0;
      for(int k=0; k<4; k++)
        ab+=mat[i][k]*m.mat[k][j];
      m2.mat[i][j]=ab;
    }
  return m2;
}

vector matrix::operator*(const vector &v) const
{
  return vector(
      v.x*mat[0][0] + v.y*mat[1][0] + v.z*mat[2][0] + v.w*mat[3][0],
      v.x*mat[0][1] + v.y*mat[1][1] + v.z*mat[2][1] + v.w*mat[3][1],
      v.x*mat[0][2] + v.y*mat[1][2] + v.z*mat[2][2] + v.w*mat[3][2],
      v.x*mat[0][3] + v.y*mat[1][3] + v.z*mat[2][3] + v.w*mat[3][3]
      );
}
//------------------------------------------------------------------------END


//Plane Class (planar geometry)----------------------------------------------
plane::plane(double av, double bv, double cv, double dv):
  norm(av, bv, cv), d(dv){}

plane::plane(const vertex &p1, const vertex &p2, const vertex &p3)
{
  set(p1,p2,p3);
}

plane::plane(const vector &v, double dv):norm(v),d(dv){}

void plane::set(double av, double bv, double cv, double dv)
{
  norm.set(av, bv, cv); d=dv;
}

void plane::set(const vertex &p1, const vertex &p2, const vertex &p3)
{
  norm=(vector)(p1-p2)%(vector)(p3-p2); 
  norm.normalize();
  d=-(norm.dot((vector)p2));
}

void plane::set(const vector &v, double dv)
{
  norm=v; d=dv;
}

double plane::classify(vertex &p) const
{
  return (norm.dot((vector)p))+d; 
}

double plane::distance(vertex &p) const
{
  return abs(norm.dot((vector)p)+d); 
}

void plane::invert()
{
  norm.negate();
  d=-d;
}
//------------------------------------------------------------------------END
}
