/**************************************************************************
 * JmarsKoder Presents 
 * JmarsKoder's Utils v. 1.0
 * 
 * written by Jason Mars (jmarskoder@gmail.com)
 * ***********************************************************************/

#include "k-utils.h"

namespace k {
//All Things Global---------------------------------------------------------
bool is_zero(const double &d){return !(d>=0.1e-13||d<=0.1e-13);}
//-----------------------------------------------------------------------END


//File and String Manipulation----------------------------------------------
string file_to_string(const string &fn)
{
  ifstream file(fn.c_str(), ios::in);
  if(!file.is_open()){return "";}
  string tmp; string res; 
  while(!file.eof())
  {
    getline(file,tmp,'\n');
    res+=tmp+'\n';
  }
  file.close();
  return res;
}

void string_to_file(const string &fn, const string &str)
{
  fstream file(fn.c_str(), ios::out);
  file << str;
  file.close();
}

string char_ptr_to_string(char *s)
{
  string tmp=s;
  delete s;
  return tmp;
}

void error(const string &s, const u_long &ln)
{
  if(ln)
    {cerr << "Error Line " << ln <<": "<< s << endl;}
  else
    {cerr << "Error: " << s << endl;}
}
//------------------------------------------------------------------------END
}
