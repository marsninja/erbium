/**************************************************************************
 * JmarsKoder Presents 
 * JmarsKoder's Utils v. 1.0
 * 
 * written by Jason Mars (jmarskoder@gmail.com)
 * ***********************************************************************/

#include <string>
#include <iostream>
#include <fstream>
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
////-----------------------------------------------------------------------END


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
  virtual void ascii_out(ostream &out)=0;
  virtual void ascii_in(istream &in)=0;
  virtual void bin_out(ostream &out)=0;
  virtual void bin_in(istream &in)=0;
};
//-----------------------------------------------------------------------END
}
#endif
