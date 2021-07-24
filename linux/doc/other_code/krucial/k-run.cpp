#include "k-utils.h"
#include "k_vm.h"

using namespace k::krucial;

void info()
{
  cout << "Krucial Virtual Machine 1.0\n"
       << "Written by JmK aka JmarsKoder aka Jason Mars\n" 
       << "http://www.x-i-c.com/" << endl;
}

void usage()
{
  cout << "Usage: k-run prog.kx  -Executes Program\n"
       << "       k-run -info    -Prints Info" << endl; 
  cout << endl;
  info();
}

int main(int argc, char *argv[])
{
  if(argc<2){usage(); return 0;}
  if(!strcmp(argv[1],"-info")) {info(); return 0;}
  ifstream in(argv[1],ios::binary);
  if(!in.is_open()){usage(); return 0;}
  ofstream errs("kvm_errors.log");
  script scrpt; scrpt.load(in);
  k_vm proc(errs); proc.execute(scrpt);
  if(proc.errored()){cout << "There Were Errors, See 'kvm_errors.log'";}
  else{errs << "No Errors!";}
  in.close();
  errs.close();
  return 0;
};
