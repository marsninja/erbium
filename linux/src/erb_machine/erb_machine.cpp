/*---------------------------------------------------------------------------
Erbium Machine

Implementation of the abstract virtual machine
Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "erb_machine.h"

namespace erb { namespace machine {
//Erbium Machine Instruction Class-------------------------------------------
erb_inst::erb_inst():op(end),c_imm(0),v_imm(0){for(int i=0;i<4;i++)prm[i]=0;}
bool erb_inst::ascii_in(istream &in)
{
  unsigned opval;
  in >> opval >> prm[0] >> prm[1] >> prm[2] >> prm[3] >> c_imm >> v_imm;
  op=(erb_op)opval;
  return true;
}

void erb_inst::ascii_out(ostream &out)
{
  out << op << ' ' << prm[0] << ' ' << prm[1] << ' ' << prm[2] << ' '
      << prm[3] << ' ' << c_imm << ' ' << v_imm << endl;
}

bool erb_inst::bin_in(istream &in)
{
  in.read((char*)&op, sizeof(erb_op));
  in.read((char*)&(prm), sizeof(prm));
  in.read((char*)&c_imm, sizeof(long));
  in.read((char*)&v_imm, sizeof(double));
  return true;
}

void erb_inst::bin_out(ostream &out)
{
  out.write((char*)&op, sizeof(erb_op));
  out.write((char*)&(prm), sizeof(prm));
  out.write((char*)&c_imm, sizeof(long));
  out.write((char*)&v_imm, sizeof(double));
}
//------------------------------------------------------------------------END


//Erbium Memory Manager------------------------------------------------------
erb_memory::erb_memory(){clear();}

void erb_memory::clear()
{
  for(u_long i=0; i<mem.size(); i++)
    {mem[i].erb_cnt.clear(); mem[i].erb_val.clear(); mem[i].erb_elm.clear();} 
  mem.add(core());
  while(!ra.empty()) ra.pop();
}

void erb_memory::print(ostream &out)
{
  out << "Current State\n";
  out << "Windows: " << mem.size() << endl;
}
//------------------------------------------------------------------------END


//Erbium Program-------------------------------------------------------------
erb_program::erb_program():cur(0),head(0),tail(0){}

void erb_program::reset(){cur=head;}

bool erb_program::at_end(){return cur==tail;}

bool erb_program::ascii_in(istream &in)
{
  while(!in.eof())
  {
    erb_inst i;
    i.ascii_in(in);
    inst.add(i);
  }  
  if(inst.size()<1) return false;
  cur=head=&inst[0];
  tail=&inst[inst.size()-1];
  return true;
}

void erb_program::ascii_out(ostream &out)
{
  for(u_long i=0; i<inst.size(); i++)
    inst[i].ascii_out(out);
}

bool erb_program::bin_in(istream &in)
{
  while(!in.eof())
  {
    erb_inst i;
    i.bin_in(in);
    inst.add(i);
  }  
  if(inst.size()<1) return false;
  cur=head=&inst[0];
  tail=&inst[inst.size()-1];
  return true;
}

void erb_program::bin_out(ostream &out)
{
  for(u_long i=0; i<inst.size(); i++)
    inst[i].bin_out(out);
}
//------------------------------------------------------------------------END


//Erbium Machine-------------------------------------------------------------
void erb_machine::serror(err_type e)
{
  static char *er[]=
  {
    "Invalid Operation!",
    "Invalid Parameter Values!",
    "Operation Does Not Support Specified Type!",
    "Fatal Division by Zero!",
    "Instruction Pointer Lost, No 'end' Operator Found!",
    "This Jump Targets an Invalid Location!",
    "Invalid Range for Randomization!"
  }; 
  (*(prog.cur)).ascii_out(err_out);
  err_out << er[e] << endl; erred=true;
}  

erb_machine::erb_machine(bag<primitive> &pr, ostream &err):
  prim(pr),err_out(err),erred(false){}

void erb_machine::load_program(const erb_program &p)
{
  prog=p; 
  erred=false;
}

//void erb_machine::trace(const u_long &ins) //located in erb_proc.cpp
//void erb_machine::execute() //located in erb_proc.cpp
//void erb_machine::big_switch(const erb_inst &i) //located in erb_proc.cpp

void erb_machine::reset(){mem.clear(); prog.reset();}

bool erb_machine::errored(){return erred;}
//------------------------------------------------------------------------END
}}
