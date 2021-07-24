/**************************************************************************
 * JmarsKoder Presents 
 * JmarsKoder's Utils v. 1.0
 * 
 * written by Jason Mars (jmarskoder@gmail.com)
 * ***********************************************************************/

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#ifndef K_Utils_Header
#define K_Utils_Header
namespace k {
//All Things Global---------------------------------------------------------
typedef unsigned long u_long;
template<class t> void k_swap(t &v1, t &v2)
{
  t tmp;
  tmp=v1;
  v1=v2;
  v2=tmp;
}
bool is_zero(const double &d);
//-------------------------------------------------------------------------END


//File and String Manipulation----------------------------------------------
string file_to_string(const string &fn);
void string_to_file(const string &fn, const string &str);
string char_ptr_to_string(char *s); //Deals with dangling pointers
void error(const string &s, const u_long &ln=0); //Prints error
//-----------------------------------------------------------------------END


//I/O-----------------------------------------------------------------------
class k_io
{
public:
	virtual ~k_io() {} // silence compiler warnings
  virtual void ascii_out(ostream &out)=0;
  virtual void ascii_in(istream &in)=0;
};
//-----------------------------------------------------------------------END


//Fast Vector---------------------------------------------------------------
template <class T>
class bag
{
private:
  u_long siz, max_siz; //max size doubles when size equals max-size
  T *b;
private:
  void resize() //double the bag size
  {
    max_siz*=2;
    T *n=new T[max_siz];
    for(u_long i=0; i<siz; i++)
    {
      n[i]=b[i];
    }
    delete[]b;
    b=n;
  }
public:
  void resize(const u_long &target) //resized to hold target num of objects
  {
    if(target<max_siz) return;
    max_siz=4;
    while(target>(max_siz*2)) max_siz*=2; //clever code: resize doubles max
    resize();
    siz=target;
  }
public:
  bag(const bag<T> &bv):siz(0),max_siz(4),b(0){set(bv);}
  bag():siz(0),max_siz(4),b(new T[max_siz]){} //initializes to max_siz 8
  ~bag(){delete[]b;} //remove from memory
  const u_long &size() const {return siz;} //returns size
  T &operator[](u_long i) const {return b[i];} //index into f_vector
  T &top() const {return b[siz-1];}
  void pop() {del(siz-1);}
  //add object ot vector
  void add(T ob){b[siz]=ob; siz++; if(siz==max_siz)resize();} 
  //delete object from vector
  void del(u_long i)
  {
    u_long tsiz=siz;
    for(;i<tsiz;i++){b[i]=b[i+1];}
    siz--;
  }
  void del_dups()
  {
    for(u_long i=0; i<siz; i++)
      for(u_long j=i+1; j<siz; j++)
        if(b[i]==b[j]){del(j); j--;}
  }
  void clear(){siz=0; max_siz=4; delete[]b; b=new T[max_siz];}
  void set(const bag<T> &bv) 
  {
    if(b==bv.b) return; //prevents value stomping if sets the same element
    while(max_siz<=bv.siz){max_siz*=2;}
    if(b) delete [] b;
    b=new T[max_siz];
    siz=bv.siz;
    for(u_long i=0; i<siz; i++)
    {
      b[i]=bv[i];
    }
  }
  void append(const bag<T> &bv)
  {
    for(u_long i=0; i<bv.siz; i++)
      add(bv[i]);
  }
  void operator=(const bag<T> &bv){set(bv);}
  bool operator==(const bag<T> &bv)
  {
    for(u_long i=0; i<siz; i++)
    {
      if(!(b[i]==bv.b[i])) return false;
    }
    return true;
  }
};
//-----------------------------------------------------------------------END
}
#endif
