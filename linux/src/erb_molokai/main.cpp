/*---------------------------------------------------------------------------
Molokai - the erbium high level language

Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "molokai.h"
#include <sstream>

extern void yyparse();
extern FILE *yyin;
namespace molokai
{
  extern molokai::compiler_state state;
}

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
  
  //open necessary files
  yyin=fopen(argv[1], "r");
  ifstream pfile("builders.prim", ios::in);

  //load in the builders
  construct prims; 
  while(!pfile.eof())
  {
    string s;
    pfile >> s; 
    if(pfile.eof()) break;
    molokai::state.add_prim(s); //add name to table
    brush b;
    b.ascii_in(pfile);
    prims.add(b);
  }

  //emit data section to output code
  molokai::state.src().add("data\n");
  ostringstream data;
  prims.ascii_out(data);
  molokai::state.src().add(data.str());

  //emit code section to output code
  molokai::state.src().add("code\n");
  molokai::state.code_gen(machine::timeseed,0,0,0,0,0,0);
  molokai::state.code_gen(machine::expand,0,128,0,0,0,0);
  molokai::state.code_gen(machine::expand,1,128,0,0,0,0);
  molokai::state.code_gen(machine::expand,2,128,0,0,0,0);
  yyparse(); 
  fclose(yyin);
  for(u_long i=0; i<molokai::state.src().size(); i++)
  {
    cout << molokai::state.src()[i] << endl;
  }
  return 0;
}
//------------------------------------------------------------------------END
