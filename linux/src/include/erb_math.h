/*---------------------------------------------------------------------------
Erb Engine Math Library

Includes all the 3d Math classes.
Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#ifndef ErbMathHeader
#define ErbMathHeader

#include "k-utils.h"

#define PI 3.1415926535897932385f
#define abs(x) ((x)<0?-(x):(x))
#define epsilon 0.0000001f
#define is_zero(x) (abs(x)<epsilon)
#define is_equal(x, y) (is_zero(x-y))

using namespace std;
using namespace k;

namespace erb {
  class vertex;
  class vector;
  class matrix;
  class plane;
  //rotate about any axis
  void rotate(vertex &v, vector axis, double ang); 
  
//Point Class (vertex in 3d space)--------------------------------------------
class vertex
{
public:
  double x, y, z, w; //publicly accessable coords.
public:
  vertex(double xv=0, double yv=0, double zv=0, double wv=1.0);
  void set(double xv=0, double yv=0, double zv=0, double wv=1.0);
  void negate(); //negates x, y, and z 
public:
  void operator+=(const vertex &v); //vector addition
  void operator-=(const vertex &v); //vector subtraction
  vertex operator+(const vertex &v) const; //vector addition
  vertex operator-(const vertex &v) const; //vector subtraction
  vertex operator*(const matrix &m) const; //vector transform
  bool operator==(const vertex &v) const; //test for equality
public:
  bool ascii_in(istream &in=cin); //Standard I/O
  void ascii_out(ostream &out=cout); 
  bool bin_in(istream &in=cin); //Binary I/O
  void bin_out(ostream &out=cout); 
};
//------------------------------------------------------------------------END


//Vector Class (vecotr in 3d space)------------------------------------------
class vector : public vertex
{
public:
  vector(double xv=0, double yv=0, double zv=0, double wv=1.0);
  vector(const vertex &p);
  double length(); //lenth of vector
  void normalize(); //normalize vector
  vector cross(const vector &v) const; //cross product of vector
  double dot(const vector &v) const;
public:
  void operator*=(const vector &v); //vector mult
  vector operator*(const vector &v) const; //vector mult
  vector operator*(double m) const; //magnify vector
  vector operator%(const vector &v) const; //cross product of vector
};
//------------------------------------------------------------------------END


//Matrix Class (translations on vertexs)--------------------------------------
class matrix
{
public:
  double mat[4][4];
public:
  matrix();
  void make_identity(); //convert to identity matrix
  //sets up a roation matrix of rad around vector dir
  void rotate_x(double ang);
  void rotate_y(double ang);
  void rotate_z(double ang);
  void scale(double x, double y, double z);
  void translate(double x, double y, double z); 
public:
  void operator*=(const matrix &m); //multiiply matricies
  matrix operator*(const matrix &m) const; //multiiply matricies
  vector operator*(const vector &v) const; //matrix*vector operation
};
//------------------------------------------------------------------------END


//Plane Class (planar geometry)----------------------------------------------
class plane
{
public:
  vector norm; //a, b, c
  double d;
public:
  plane(double av=0, double bv=0, double cv=1, double dv=0); //xy plane
  //calc plane from 3 vertexs
  plane(const vertex &p1, const vertex &p2, const vertex &p3); 
  plane(const vector &v, double dv); 
  void set(double av=0, double bv=0, double cv=1, double dv=0); //xy plane
  //calc plane from 3 vertexs
  void set(const vertex &p1, const vertex &p2, const vertex &p3); 
  void set(const vector &v, double dv); 
  double classify(vertex &p) const; //tells what size of plane a vertex lies
  double distance(vertex &p) const; //absolute distance from plane
  void invert(); //effectively flips the plane
};
//------------------------------------------------------------------------END
}
#endif
