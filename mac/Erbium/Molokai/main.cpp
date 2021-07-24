/*---------------------------------------------------------------------------
Molokai - the erbium high level language

Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "molokai.h"
#include <sstream>

using namespace molokai;

void info()
{
  cout << "Molokai for Erbium Version 1.0\n"
       << "Written by JmK aka JmarsKoder aka Jason Mars\n" 
       << "http://jm.is-a-geek.org/" << endl;
}

void usage()
{
  cout << "Usage: molokai prog.mol <prog.ebr>  -Compile Source File\n"
       << "       molokai -info                -Prints Info" << endl; 
  cout << endl;
  info();
}

//Molokai--------------------------------------------------------------------
int main(int argc, char *argv[])
{
  //check command line
  if(argc<2){usage(); return 0;}
  else if(!strcmp(argv[1],"-info"))
    {info(); return 0;}
  
  cout << mol_compile(argv[1]);
  
  return 0;
}
//------------------------------------------------------------------------END
