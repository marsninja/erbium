/*---------------------------------------------------------------------------
 Erb Engine Math Library
 
 Includes all the 3d Math classes.
 Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
 ---------------------------------------------------------------------------*/

#ifndef ErbMathHeader
#define ErbMathHeader

#include "k-utils.h"

#define PI 3.1415926535897932385
#define PI_OVER_180 0.01745329251994
#define abs(x) ((x)<0?-(x):(x))
#define epsilon (numeric_limits<double>::epsilon()*10)
#define max_precision (numeric_limits<double>::digits10)
#define is_zero(x) (abs(x)<epsilon)
#define is_equal(x, y) (is_zero(x-y))

using namespace std;
using namespace k;

namespace erb {
class vector;
class matrix;
class plane;
//rotate about any axis
void rotate(vector &v, vector axis, double ang); 

//Vector Class (vector in 3d space)------------------------------------------
class vector
{
public:
  double x, y, z, w; //publicly accessable coords.
public:
  vector(double xv=0, double yv=0, double zv=0, double wv=1.0);
  vector(const vector &p);
  void set(double xv=0, double yv=0, double zv=0, double wv=1.0);
  void negate(); //negates x, y, and z 
  double length(); //lenth of vector
  void normalize(); //normalize vector
  vector cross(const vector &v) const; //cross product of vector
  double dot(const vector &v) const;
public:
  vector& operator+=(const vector &v); //vector addition
  vector& operator-=(const vector &v); //vector subtraction
  vector& operator*=(const vector &v); //vector mult
  vector operator+(const vector &v) const; //vector addition
  vector operator-(const vector &v) const; //vector subtraction
  vector operator*(const vector &v) const; //vector mult
  vector operator*(double m) const; //magnify vector
  vector operator%(const vector &v) const; //cross product of vector
  vector operator*(const matrix &m) const; //vector transform
  bool operator==(const vector &v) const; //test for equality
public:
  bool ascii_in(istream &in=cin); //Standard I/O
  void ascii_out(ostream &out=cout); 
};
//------------------------------------------------------------------------END


//Matrix Class (translations on vectors)--------------------------------------
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
  //calc plane from 3 vectors
  plane(const vector &p1, const vector &p2, const vector &p3); 
  plane(const vector &v, double dv); 
  void set(double av=0, double bv=0, double cv=1, double dv=0); //xy plane
  //calc plane from 3 vectors
  void set(const vector &p1, const vector &p2, const vector &p3); 
  void set(const vector &v, double dv); 
  double classify(vector &p) const; //tells what size of plane a vector lies
  double distance(vector &p) const; //absolute distance from plane
  void invert(); //effectively flips the plane
public:
  bool ascii_in(istream &in=cin); //Standard I/O
  void ascii_out(ostream &out=cout); 
};
//------------------------------------------------------------------------END
}
#endif
