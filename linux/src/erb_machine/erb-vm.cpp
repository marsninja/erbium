//Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
#include "erb_machine.h"

using namespace erb::machine;

void info()
{
  cout << "Erbium Execution Machine Version 1.0\n"
       << "Written by JmK aka JmarsKoder aka Jason Mars\n" 
       << "http://jm.is-a-geek.org/" << endl;
}

void usage()
{
  cout << "Usage: erb-vm prog.ebx  -Executes Program\n"
       << "       erb-vm -info     -Prints Info" << endl; 
  cout << endl;
  info();
}

int main(int argc, char *argv[])
{
  if(argc<2){usage(); return 0;}
  if(!strcmp(argv[1],"-info")) {info(); return 0;}
  ifstream in(argv[1],ios::binary);
  if(!in.is_open()){usage(); return 0;}
  //ofstream errs("erb_vm_errors.log");
  
  bag<erb::primitive> p; 
  erb::construct c; c.bin_in(in);
  for(u_long i=0; i<c.num_brushes(); i++)
  {
    erb::primitive t; t.set(c[i]);
    p.add(t);
  }
    
  erb_program prog; prog.bin_in(in);
  erb_machine proc(p, cerr); 
  proc.load_program(prog); proc.execute();
  in.close();
  //errs.close();
  return 0;
};
