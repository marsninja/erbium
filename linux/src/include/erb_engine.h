/*---------------------------------------------------------------------------
Erb Engine

Includes all of Erb engine
Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#ifndef ErbEngineHeader
#define ErbEngineHeader

#include "erb_math.h"

using namespace std;
using namespace k;
using k::u_long;

namespace erb {
  //bounding box with origin and dimentions
  struct box {vertex orig; vector dim; bool valid; box():valid(false){}}; 
  class polygon; //polygon
  class brush; //single convex polyhedra
  class construct; //a construct
  //rotate about any axis
  void rotate(polygon &p, vector axis, double ang);
  void rotate(brush &b, vector axis, double ang);
  void rotate(construct &c, vector axis, double ang);
  //test for bounding box collision
  bool collide(const box &a, const box &b);
  
//Polygon Class (holds verticies and plane)----------------------------------
class polygon
{
public:
  plane p;
  bag<vertex> vert;  
public:
  void add(const vertex &pt); //add a vertex to polygon
  bool form_plane(); //use first 3 vertexs to gen plane
  void apply(const matrix &m); //apply matrix to all verticies
  u_long num_verts(); //return number of verticies
  void sort_verts(); //sorts the points in counter clockwise orientation
  void invert();//reverses order of verticies
public:
  vertex &operator[](u_long i); //return vert[i]
  bool operator==(const polygon &p); //test for equality
public:
  bool ascii_in(istream &in=cin); //Standard I/O
  void ascii_out(ostream &out=cout); 
  bool bin_in(istream &in=cin); //Binary I/O
  void bin_out(ostream &out=cout); 
  void map_out(ostream &out=cout, const string &tex="sfloor1_2"); //for quake mapfile format
};
//------------------------------------------------------------------------END


//Brush Class (convex polyhedra)---------------------------------------------
class brush
{
public:
  box bound; //brushes bouding box
  bag<polygon> poly;
public:
  brush();
  void add(const polygon &p); //add a polgyon to brush
  bool formulate(); //gen legit planes and verts
  void build_box(); //build bounding box
  void apply(const matrix &m); //apply matrix to entire brush
  void apply_self(const matrix &m); //apply matrix to brush in local space
  void center(); //centers brush on the origin
  u_long num_polys(); //return number of polygons
public:
  polygon &operator[](u_long i); //return poly[i]
  bool operator==(const brush &b); //test for equality
public:
  bool ascii_in(istream &in=cin); //Standard I/O
  void ascii_out(ostream &out=cout); 
  bool bin_in(istream &in=cin); //Binary I/O
  void bin_out(ostream &out=cout); 
  void map_out(ostream &out=cout); //for quake mapfile format
};

class primitive //a brush that is normalized and centered
{
private:
  brush br;
public:
  void set(const brush &b); 
  const brush &get();
};
//------------------------------------------------------------------------END


//Construct Class (essentially a csg-tree)-----------------------------------
class construct
{
public:
  box bound; //bouding box
  bag<brush> br; //the polyhedra that forms the construct
public:
  construct();
  construct(const brush &b); //add a brush
  void add(const brush &b); //add a brush to the construct
  bool formulate(); //gen legit planes and verts
  void build_box(); //build bounding box
  void apply(const matrix &m); //apply matrix to construct
  void apply_self(const matrix &m); //applys to construct in local space
  void center(); //centers construct on the origin
  u_long num_brushes(); //return number of brushes
public:
  brush &operator[](u_long i); //return br[i]
  bool operator==(const construct &c); //test for equality
public:
  construct operator-(const construct &c); //Subtraction
  construct operator+(const construct &c); //Union
  construct operator*(const construct &c); //Intersection
  void operator-=(const construct &c);
  void operator+=(const construct &c);
  void operator*=(const construct &c);
public:
  bool ascii_in(istream &in=cin); //Standard I/O
  void ascii_out(ostream &out=cout); 
  bool bin_in(istream &in=cin); //Binary I/O
  void bin_out(ostream &out=cout); 
  void map_out(ostream &out=cout); //for quake mapfile format
};
//------------------------------------------------------------------------END
}
#endif
