/*---------------------------------------------------------------------------
Erbium Engine

Includes all of Erbium engine
Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "erb_engine.h"
#include <cmath>

namespace erb {
//BBox Collide---------------------------------------------------------------
bool collide(const box &a, const box &b)
{
  double axmin=a.orig.x, axmax=a.orig.x+a.dim.y,
         aymin=a.orig.y, aymax=a.orig.y+a.dim.y,
         azmin=a.orig.z, azmax=a.orig.z+a.dim.z,
         bxmin=b.orig.x, bxmax=b.orig.x+b.dim.y,
         bymin=b.orig.y, bymax=b.orig.y+b.dim.y,
         bzmin=b.orig.z, bzmax=b.orig.z+b.dim.z;

  if(axmax <= bxmin+epsilon ||
     aymax <= bymin+epsilon ||
     azmax <= bzmin+epsilon ||
     bxmax <= axmin+epsilon ||
     bymax <= aymin+epsilon || 
     bzmax <= azmin+epsilon) return 0;
  
  return 1;

}
//------------------------------------------------------------------------END


//Rotate Func----------------------------------------------------------------
void rotate(polygon &p, vector axis, double ang)
{
  for(u_long i=0; i<p.num_verts(); i++)
    rotate(p[i], axis, ang);
}

void rotate(brush &b, vector axis, double ang)
{
  for(u_long i=0; i<b.num_polys(); i++)
    rotate(b[i], axis, ang);
}

void rotate(construct &c, vector axis, double ang)
{
  for(u_long i=0; i<c.num_brushes(); i++)
    rotate(c[i], axis, ang);
}
//---------------------------------------------------------------------------
  
//Polygon Class--------------------------------------------------------------
void polygon::add(const vertex &pt)
{
  vert.add(pt);
}

bool polygon::form_plane()
{
  if(vert.size()>2)
  {
    p.set(vert[0], vert[1], vert[2]);
    return true;
  }
  else return false;
}

void polygon::apply(const matrix &m)
{
  for(u_long i=0; i<vert.size(); i++)
    vert[i]=vert[i]*m;
}

u_long polygon::num_verts()
{
  return vert.size();
  
}

void polygon::sort_verts()
{
  u_long vsize=vert.size();
  if(vsize<3) return;
  vertex center; //First get center of polygon
  for(u_long i=0; i<vsize; i++) 
    center+=vert[i];
  center.x/=vsize; center.y/=vsize; center.z/=vsize;

  //now we find relative angles of all verts
  double ang[vsize]; ang[0]=0;
  for(u_long i=1; i<vsize; i++)
  {
    vector v1=vert[0]-center; v1.normalize();
    vector v2=vert[i]-center; v2.normalize();
    vector v1r(v1.z+epsilon/2, -epsilon, -(v1.x+epsilon/2)); //Epsilon used for smoothings
    double angle=(acos(v1.dot(v2))*180)/PI;
    if(v1r.dot(v2)<0&&(!is_zero(angle))) angle=360-angle;
    if(is_zero(angle)&&!(v1==v2)){angle=180.0;}
    ang[i]=angle;
  }

  //finaly we sort them
  bag<vertex> newb;
  newb.add(vert[0]);
  u_long index=0;
  double cutoff=0, min=360;
  for(u_long i=1; i<vsize; i++)
  {
    for(u_long j=1; j<vsize; j++)
    {
      if(ang[j]>cutoff&&ang[j]<=min){index=j; min=ang[j];}
    }
    newb.add(vert[index]);
    cutoff=min; min=360;
  }
  
  if(plane(newb[0], newb[1], newb[2]).norm==p.norm) 
    vert=newb;  
  else
  {
    vert.clear();
    for(u_long i=1; i<vsize+1; i++)
      vert.add(newb[vsize-i]); 
  }
}

void polygon::invert()
{
  u_long vsize=vert.size();
  bag<vertex> rev;
  for(u_long i=0; i<vsize; i++)
    rev.add(vert[vsize-(i+1)]);
  vert=rev;
}

vertex &polygon::operator[](u_long i)
{
  return vert[i];
}

bool polygon::operator==(const polygon &p)
{
  return vert==p.vert;
}

bool polygon::ascii_in(istream &in)
{
  u_long c;
  in >> c; 
  while(c--)
  {
    vertex v;
    if(!v.ascii_in(in))return false;
    add(v);
  }
  return true;
}

void polygon::ascii_out(ostream &out)
{
  u_long c=vert.size(), d=c-1; //D IMPORTANT KEEPS CORRECT VERT ORDER
  out << c << " " ; 
  while(c--)
  {
    vert[d-c].ascii_out(out);
    out << " ";
  }
}

bool polygon::bin_in(istream &in)
{
  u_long c;
  in.read((char*)&c, sizeof(u_long)); 
  while(c--)
  {
    vertex v;
    if(!v.bin_in(in))return false;
    add(v);
  }
  return true;
}

void polygon::bin_out(ostream &out)
{
  u_long c=vert.size(), d=c-1; //D IMPORTANT KEEPS CORRECT VERT ORDER
  out.write((char*)&c, sizeof(u_long)); 
  while(c--)
  {
    vert[d-c].bin_out(out);
  }
}

void polygon::map_out(ostream &out, const string &tex)
{
  u_long c=3, d=c-1; //D IMPORTANT KEEPS CORRECT VERT ORDER
  while(c--)
  {
    vert[d-c].ascii_out(out);
    out << " ";
  }
  out << tex << " 0 0 0 1.0 1.0";
}
//------------------------------------------------------------------------END


//Brush Class (convex polyhedra)---------------------------------------------
brush::brush(){bound.valid=false;}

void brush::add(const polygon &p)
{
  poly.add(p);
  bound.valid=false;
}

bool brush::formulate()
{
  for(u_long i=0; i<poly.size(); i++) //Fist set up all planes
  {
    if(!poly[i].form_plane()) return false;
    poly[i].vert.clear();
  }

  //Generate verticies from plane intersections
  for(u_long i=0; i<poly.size(); i++) 
  {
    for(u_long j=i; j<poly.size(); j++)
    {
      if(i==j) continue;

      //Firt get the line intersecton of 2 planes
      vertex origin; vector dir;
      dir=poly[i].p.norm%poly[j].p.norm; //Get direction vector for the line  
      double len=dir.length(); //get lenth of vector
      if(is_zero(len)) continue; //if lenght is zero planes are parallel
      //Choose the best axis to place origin point
      double abval, maxab=abs(dir.x);
      u_long axis=0; 
      if((abval=abs(dir.y))>maxab) {maxab=abval; axis=1;}
      if((abval=abs(dir.z))>maxab) {maxab=abval; axis=2;}
      switch(axis) //Set origin point
      {
        case 0: origin.set(0, 
                    (poly[i].p.norm.z*poly[j].p.d-
                     poly[j].p.norm.z*poly[i].p.d)/dir.x,  
                    (poly[j].p.norm.y*poly[i].p.d-
                     poly[i].p.norm.y*poly[j].p.d)/dir.x);  
                break;
        case 1: origin.set((poly[j].p.norm.z*poly[i].p.d-
                      poly[i].p.norm.z*poly[j].p.d)/dir.y,  
                    0, 
                    (poly[i].p.norm.x*poly[j].p.d-
                     poly[j].p.norm.x*poly[i].p.d)/dir.y);  
                break;
        case 2: origin.set((poly[i].p.norm.y*poly[j].p.d-
                      poly[j].p.norm.y*poly[i].p.d)/dir.z,  
                    (poly[j].p.norm.x*poly[i].p.d-
                     poly[i].p.norm.x*poly[j].p.d)/dir.z,
                    0);  
                break;
      }
      dir.normalize();

      //now we have to get the point of intersection between 
      //line and all planes
      for(u_long k=0; k<poly.size(); k++)
      {
        vertex ver;
        if(k==i||k==j)continue;
        double tmp=dir.dot(poly[k].p.norm); 
        if(is_zero(tmp)) continue; //plane is parallel to line;
        ver=dir*(-(poly[k].p.norm.dot(origin)+poly[k].p.d)/tmp);
        ver+=origin; 

        //cut out verticies that are cliped by planes 
        bool add=true;
        for(u_long l=0; l<poly.size(); l++)
          if(poly[l].p.classify(ver)>epsilon)
          {
            add=false; break;
          } 
        if(!add) continue;
        
        //add each vertex to our list
        poly[i].add(ver);
        poly[j].add(ver);
      } 
    }
    poly[i].vert.del_dups();
    poly[i].sort_verts();
    
    if(poly[i].num_verts()<3){poly.del(i); i--;} //delete invalid planes
  }
  //build bouding box
  build_box();
  return true;
}

void brush::build_box()
{
  bound.orig=bound.dim=poly[0][0];
  for(u_long i=0; i<poly.size(); i++)
    for(u_long j=0; j<poly[i].num_verts(); j++)
    {
      if(bound.orig.x>poly[i][j].x) bound.orig.x=poly[i][j].x;
      if(bound.orig.y>poly[i][j].y) bound.orig.y=poly[i][j].y;
      if(bound.orig.z>poly[i][j].z) bound.orig.z=poly[i][j].z;
      if(bound.dim.x<poly[i][j].x) bound.dim.x=poly[i][j].x;
      if(bound.dim.y<poly[i][j].y) bound.dim.y=poly[i][j].y;
      if(bound.dim.z<poly[i][j].z) bound.dim.z=poly[i][j].z;
    }
  bound.dim-=bound.orig;
  bound.valid=true;
}

void brush::apply(const matrix &m)
{
  for(u_long i=0; i<poly.size(); i++) 
    poly[i].apply(m);
  bound.valid=false;
}

void brush::apply_self(const matrix &m)
{
  if(!bound.valid) build_box();
  matrix trans, untrans;
  trans.translate(-(bound.orig.x+bound.dim.x/2), 
      -(bound.orig.y+bound.dim.y/2), -(bound.orig.z+bound.dim.z/2));
  untrans.translate(bound.orig.x+bound.dim.x/2, 
      bound.orig.y+bound.dim.y/2, bound.orig.z+bound.dim.z/2);
  for(u_long i=0; i<poly.size(); i++) 
  {
    poly[i].apply(trans);
    poly[i].apply(m);
    poly[i].apply(untrans);
  }
  bound.valid=false;
}

void brush::center()
{
  if(!bound.valid) build_box();
  matrix trans;
  trans.translate(-(bound.orig.x+bound.dim.x/2), 
      -(bound.orig.y+bound.dim.y/2), -(bound.orig.z+bound.dim.z/2));
  for(u_long i=0; i<poly.size(); i++)
    poly[i].apply(trans);
  bound.valid=false;
}

u_long brush::num_polys()
{
  return poly.size();
}

polygon &brush::operator[](u_long i)
{
  return poly[i];
}

bool brush::operator==(const brush &b)
{
  return poly==b.poly;
}

bool brush::ascii_in(istream &in)
{
  u_long c;
  in >> c; 
  while(c--)
  {
    polygon p;
    if(!p.ascii_in(in))return false;
    add(p);
  }
  formulate();
  return true;
}

void brush::ascii_out(ostream &out)
{
  u_long c=poly.size();
  out << c << endl; 
  while(c--)
  {
    poly[c].ascii_out(out);
    out << endl;
  }
}

bool brush::bin_in(istream &in)
{
  u_long c;
  in.read((char*)&c, sizeof(u_long)); 
  while(c--)
  {
    polygon p;
    if(!p.bin_in(in))return false;
    add(p);
  }
  formulate();
  return true;
}

void brush::bin_out(ostream &out)
{
  u_long c=poly.size();
  out.write((char*)&c, sizeof(u_long)); 
  while(c--)
  {
    poly[c].bin_out(out);
  }
}

void brush::map_out(ostream &out)
{
  bag<string> tex;
  tex.add("sfloor1_2");
  tex.add("sfloor4_2");
  tex.add("sfloor4_6");
  tex.add("afloor1_4");
  tex.add("afloor1_3");
  tex.add("azfloor1_1");
  tex.add("woodflr1_5");
  tex.add("floor01_5");
  //string pick=tex[rand()%tex.size()];
  string pick=tex[3];//tex[rand()%tex.size()];

  u_long c=poly.size();
  out << "{" << endl; 
  while(c--)
  {
    poly[c].map_out(out,pick);
    out << endl;
  }
  out << "}" << endl; 
}

void primitive::set(const brush &b)
{
  br=b;
  br.center();
  matrix m;
  m.scale(1/br.bound.dim.x, 1/br.bound.dim.y, 1/br.bound.dim.z);
  br.apply(m);
}

const brush &primitive::get(){return br;}
//------------------------------------------------------------------------END


//Construct Class (essentially a csg-tree)-----------------------------------
construct::construct(){bound.valid=false;}
construct::construct(const brush &b){br.add(b);}

void construct::add(const brush &b){bound.valid=false; br.add(b);}

bool construct::formulate()
{
  for(u_long i=0; i<br.size(); i++)
  {
    if(!br[i].formulate()){return false;}
    if(br[i].num_polys()<4){br.del(i); i--;}
  }
  build_box();
  return true;
}

void construct::build_box()
{
  if(br.size()<1)return;
  bound.orig=bound.dim=br[0].poly[0][0];
  for(u_long l=0; l<br.size(); l++)
    for(u_long i=0; i<br[l].poly.size(); i++)
      for(u_long j=0; j<br[l].poly[i].num_verts(); j++)
      {
        if(bound.orig.x>br[l].poly[i][j].x) bound.orig.x=br[l].poly[i][j].x;
        if(bound.orig.y>br[l].poly[i][j].y) bound.orig.y=br[l].poly[i][j].y;
        if(bound.orig.z>br[l].poly[i][j].z) bound.orig.z=br[l].poly[i][j].z;
        if(bound.dim.x<br[l].poly[i][j].x) bound.dim.x=br[l].poly[i][j].x;
        if(bound.dim.y<br[l].poly[i][j].y) bound.dim.y=br[l].poly[i][j].y;
        if(bound.dim.z<br[l].poly[i][j].z) bound.dim.z=br[l].poly[i][j].z;
      }
  bound.dim-=bound.orig;
  bound.valid=true;
}

void construct::apply(const matrix &m)
{
  for(u_long j=0; j<br.size(); j++)
    for(u_long i=0; i<br[j].poly.size(); i++) 
      br[j].poly[i].apply(m);
  bound.valid=false;
  //build_box();
}

void construct::apply_self(const matrix &m)
{
  if(!bound.valid) build_box();
  matrix trans, untrans;
  trans.translate(-(bound.orig.x+bound.dim.x/2), 
      -(bound.orig.y+bound.dim.y/2), -(bound.orig.z+bound.dim.z/2));
  untrans.translate(bound.orig.x+bound.dim.x/2, 
      bound.orig.y+bound.dim.y/2, bound.orig.z+bound.dim.z/2);
  for(u_long j=0; j<br.size(); j++)
    for(u_long i=0; i<br[j].poly.size(); i++) 
    {
      br[j].poly[i].apply(trans);
      br[j].poly[i].apply(m);
      br[j].poly[i].apply(untrans);
    }
  bound.valid=false;
  //build_box();
}

void construct::center()
{
  if(!bound.valid) build_box();
  matrix trans;
  trans.translate(-(bound.orig.x+bound.dim.x/2), 
      -(bound.orig.y+bound.dim.y/2), -(bound.orig.z+bound.dim.z/2));
  for(u_long j=0; j<br.size(); j++)
    for(u_long i=0; i<br[j].poly.size(); i++)
      br[j].poly[i].apply(trans);
  bound.valid=false;
  //build_box();
}

u_long construct::num_brushes(){return br.size();}

brush &construct::operator[](u_long i){return br[i];}

bool construct::operator==(const construct &c)
{
  return br==c.br;
}

construct construct::operator+(const construct &c)
{
  construct ret; ret.br=this->br;
  ret.br.append(c.br);
  ret.bound.valid=false;
  //ret.build_box();
  return ret;
}

construct construct::operator-(const construct &c)
{
  construct ret; ret.br=this->br;
  bag<brush> newb;
  for(u_long i=0; i<c.br.size(); i++)
  {
    for(u_long k=0; k<ret.br.size(); k++)
    {
        if(!ret.br[k].bound.valid){ret.br[k].build_box();}
        if(!c.br[i].bound.valid){c.br[i].build_box();}
        if(collide(c.br[i].bound, ret.br[k].bound))
        {
          for(u_long j=0; j<c.br[i].num_polys(); j++)
          {
            brush b=ret.br[k];
            polygon p=c.br[i][j]; p.invert();
            b.poly.add(p);
            newb.add(b);
          }
        }
        else {newb.add(ret.br[k]);}
        ret.br.del(k); k--;
    }
    ret.br=newb; newb.clear();
  }
  ret.formulate();
  return ret;
}

construct construct::operator*(const construct &c)
{
  construct ret; ret.br=this->br;
  for(u_long i=0; i<c.br.size(); i++)
    for(u_long j=0; j<c.br[i].num_polys(); j++)
      for(u_long k=0; k<ret.br.size(); k++)
        ret.br[k].poly.add(c.br[i][j]);
  ret.formulate();
  return ret;
}

void construct::operator+=(const construct &c){*this=*this+c;}
void construct::operator-=(const construct &c){*this=*this-c;}
void construct::operator*=(const construct &c){*this=*this*c;}

bool construct::ascii_in(istream &in)
{
  u_long c;
  in >> c; 
  while(c--)
  {
    brush b;
    if(!b.ascii_in(in))return false;
    add(b);
  }
  return true;
}

void construct::ascii_out(ostream &out)
{
  u_long c=br.size();
  out << c << endl; 
  while(c--)
  {
    br[c].ascii_out(out);
    out << endl;
  }
}

bool construct::bin_in(istream &in)
{
  u_long c;
  in.read((char*)&c, sizeof(u_long)); 
  while(c--)
  {
    brush b;
    if(!b.bin_in(in))return false;
    add(b);
  }
  return true;
}

void construct::bin_out(ostream &out)
{
  u_long c=br.size();
  out.write((char*)&c, sizeof(u_long)); 
  while(c--)
  {
    br[c].bin_out(out);
  }
}

void construct::map_out(ostream &out)
{
  u_long c=br.size();
  out << "{\n\"classname\" \"worldspawn\"\n" << endl; 
  while(c--)
  {
    br[c].map_out(out);
    out << endl;
  }
  out << "}";
}
//------------------------------------------------------------------------END
}
