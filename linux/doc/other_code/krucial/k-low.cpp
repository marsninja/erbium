#include "k-utils.h"
#include "k_low.h"

using namespace k::krucial;

void info()
{
  cout << "Krucial Low Level Translator 1.0\n"
       << "Written by JmK aka JmarsKoder aka Jason Mars\n" 
       << "http://www.x-i-c.com/" << endl;
}

void usage()
{
  cout << "Usage: k-low prog.klo prog.kx  -Assembles Source File\n"
       << "       k-low -info             -Prints Info" << endl; 
  cout << endl;
  info();
}

int main(int argc, char *argv[])
{
  if(argc<2){usage(); return 0;}
  else if(!strcmp(argv[1],"-info"))
    {info(); return 0;}
  string in=k::file_to_string(argv[1]);
  if(in.size()<1){return 0;}
  string out_file="out.kx";
  if(argc>2) out_file=argv[2];
  ofstream out(out_file.c_str(),ios::binary);
  ofstream errs("klow_errors.log");
  k_low a(errs); a.assemble(in,out);
  if(a.errored()){cout << "There Were Errors, See 'klow_errors.log'";}
  else{errs << "No Errors!";}
  out.close();
  errs.close();
  return 0;
};
