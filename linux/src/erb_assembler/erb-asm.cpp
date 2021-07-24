//Written and Maintained by Jason Mars (JmarsKoder@gmail.com)

#include "erb_assembler.h"

using namespace erb::assembler;

void info()
{
  cout << "Erbium Assembler Version 1.0\n"
       << "Written by JmK aka JmarsKoder aka Jason Mars\n" 
       << "http://jm.is-a-geek.org/" << endl;
}

void usage()
{
  cout << "Usage: erb-asm prog.erb <prog.ebx>  -Assembles Source File\n"
       << "       erb-asm -info                -Prints Info" << endl; 
  cout << endl;
  info();
}

int main(int argc, char *argv[])
{
  if(argc<2){usage(); return 0;}
  else if(!strcmp(argv[1],"-info"))
    {info(); return 0;}
  string in=k::file_to_string(argv[1]);
  if(in.size()<1){usage(); return 0;}
  string out_file="out.ebx";
  if(argc>2) out_file=argv[2];
  ofstream out(out_file.c_str(),ios::binary);
  //ofstream errs("erb_asm_errors.log");
  assemble(in,out,cerr);
  out.close();
  //errs.close();
  return 0;
};
