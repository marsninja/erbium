#include "k_vm.h"
#include <sstream>

namespace k { namespace krucial { 
//Memory--------------------------------------------------------------------
k_memory::k_memory(){clear();}
k_memory::~k_memory(){clear();}

void k_memory::clear()
{
  while(!ra.empty()){ra.pop();}
  for(u_long i=0; i<32; i++)
  {
    k_reg_int[i]=0; 
    k_reg_cnt[i]=0;
    k_reg_num[i]=0;
    k_reg_str[i]="";
  }
  for(u_long i=0; i<k_int.size(); i++) 
    {if(k_int[i]!=0){delete k_int[i]; k_int[i]=0;}}
  for(u_long i=0; i<k_cnt.size(); i++)
    {if(k_cnt[i]!=0){delete k_cnt[i]; k_cnt[i]=0;}}
  for(u_long i=0; i<k_num.size(); i++)
    {if(k_num[i]!=0){delete k_num[i]; k_num[i]=0;}}
  for(u_long i=0; i<k_str.size(); i++)
    {if(k_str[i]!=0){delete k_str[i]; k_str[i]=0;}}
  k_int.clear(); k_cnt.clear();
  k_num.clear(); k_str.clear();
  while(!k_s_int.empty())k_s_int.pop();
  while(!k_s_cnt.empty())k_s_cnt.pop();
  while(!k_s_num.empty())k_s_num.pop();
  while(!k_s_str.empty())k_s_str.pop();
}

void k_memory::push_ra(instruction* &l){ra.push(l);}

instruction* k_memory::pop_ra()
  {instruction* tmp=ra.top(); ra.pop(); return tmp;}

void k_memory::expand_int(const u_long &l)
  {for(u_long i=0; i<l; i++) k_int.push_back(new long);}

void k_memory::expand_cnt(const u_long &l)
  {for(u_long i=0; i<l; i++) k_cnt.push_back(new u_long);}

void k_memory::expand_num(const u_long &l)
  {for(u_long i=0; i<l; i++) k_num.push_back(new double);}

void k_memory::expand_str(const u_long &l)
  {for(u_long i=0; i<l; i++) k_str.push_back(new string);}

void k_memory::deflate_int(u_long l)
{
  if(l>=k_int.size()) l=k_int.size(); 
  for(u_long i=0; i<l; i++) k_int.pop_back();
}

void k_memory::deflate_cnt(u_long l)
{
  if(l>=k_cnt.size()) l=k_cnt.size(); 
  for(u_long i=0; i<l; i++) k_cnt.pop_back();
}

void k_memory::deflate_num(u_long l)
{
  if(l>=k_num.size()) l=k_num.size();
  for(u_long i=0; i<l; i++) k_num.pop_back();
}

void k_memory::deflate_str(u_long l)
{
  if(l>=k_str.size()) l=k_str.size(); 
  for(u_long i=0; i<l; i++) k_str.pop_back();
}

long &k_memory::integer(const u_long &id)
{
  if(id<32){return k_reg_int[id];}
  else
  {
    if((id-32)<k_int.size()){return *(k_int[id-32]);}
    else return k_reg_int[0];
  }
}

u_long &k_memory::count(const u_long &id)
{
  if(id<32){return k_reg_cnt[id];}
  else
  {
    if((id-32)<k_cnt.size()){return *(k_cnt[id-32]);}
    else return k_reg_cnt[0];
  }
}

double &k_memory::number(const u_long &id)
{
  if(id<32){return k_reg_num[id];}
  else
  {
    if((id-32)<k_num.size()){return *(k_num[id-32]);}
    else return k_reg_num[0];
  }
}

string &k_memory::c_string(const u_long &id)
{
  if(id<32){return k_reg_str[id];}
  else
  {
    if((id-32)<k_str.size()){return *(k_str[id-32]);}
    return k_reg_str[0];
  }
}

u_long k_memory::integer_size(){return (u_long)k_int.size()+32;}
u_long k_memory::count_size(){return (u_long)k_cnt.size()+32;}
u_long k_memory::number_size(){return (u_long)k_num.size()+32;}
u_long k_memory::c_string_size(){return (u_long)k_str.size()+32;}

void k_memory::push_int(const long &val){k_s_int.push(val);}
void k_memory::push_cnt(const u_long &val){k_s_cnt.push(val);}
void k_memory::push_num(const double &val){k_s_num.push(val);}
void k_memory::push_str(const string &val){k_s_str.push(val);}

long k_memory::pop_int()
{
  if(k_s_int.empty()){return 0;}
  long tmp=k_s_int.top(); 
  k_s_int.pop();
  return tmp;
}

u_long k_memory::pop_cnt()
{
  if(k_s_cnt.empty()){return 0;}
  u_long tmp=k_s_cnt.top();
  k_s_cnt.pop();
  return tmp;
}

double k_memory::pop_num()
{
  if(k_s_num.empty()){return 0.0;}
  double tmp=k_s_num.top();
  k_s_num.pop();
  return tmp;
}

string k_memory::pop_str()
{
  if(k_s_str.empty()){return "";}
  string tmp=k_s_str.top();
  k_s_str.pop();
  return tmp;
}

stack<long> &k_memory::int_see_stack(){return k_s_int;}
stack<u_long> &k_memory::cnt_see_stack(){return k_s_cnt;}
stack<double> &k_memory::num_see_stack(){return k_s_num;}
stack<string> &k_memory::str_see_stack(){return k_s_str;}

string k_memory::print()
{
  ostringstream out;
  out << "Current State\n";
  out << "\nStack Tops\n-\n";
  out << "\tint\tcount\tnumber\tstring\n";
  if(ra.size()>0){out << ra.top();} else out << 0; out << '\t';
  if(k_s_int.size()>0){out << k_s_int.top();} else out << 0; out << '\t';
  if(k_s_cnt.size()>0){out << k_s_cnt.top();} else out << 0; out << '\t';
  if(k_s_num.size()>0){out << k_s_num.top();} else out << 0; out << '\t';
  if(k_s_str.size()>0){out << k_s_str.top();} else out << ""; out << endl;
  out << "\nRegisters\n-\n";
  out << "reg\tint\tcount\tnumber\tstring\n";
  for(u_long i=0; i<32; i++)
    out << i << '\t' << k_reg_int[i] << '\t' << k_reg_cnt[i] << '\t'
        << k_reg_num[i] << '\t' << k_reg_str[i] << endl;
  return out.str();
}
//-----------------------------------------------------------------------END


//Instruction---------------------------------------------------------------
instruction::instruction(){clear();}

void instruction::clear()
{
  opc=end; for(u_long i=0; i<4; i++)prm[i]=0;
  val.k_int=0; val.k_cnt=0;
  val.k_num=0; val.k_str.clear();
}

void instruction::load(istream &input)
{
  u_long strsize=0;
  input.read((char*)&opc, sizeof(unsigned int));
  input.read((char*)&(prm), sizeof(prm));
  input.read((char*)&(val.k_int), sizeof(long));
  input.read((char*)&(val.k_cnt), sizeof(u_long));
  input.read((char*)&(val.k_num), sizeof(double));
  input.read((char*)&(strsize), sizeof(u_long)); 
  if(strsize!=0)
  {
    char *tmp=new char[strsize+1]; 
    input.read(tmp, strsize); tmp[strsize]='\0'; val.k_str=tmp;
    delete[]tmp; 
  }
}

void instruction::dump(ostream &output)
{ 
  u_long strsize=(u_long)val.k_str.size()+1;
  output.write((char*)&opc, sizeof(unsigned int));
  output.write((char*)&(prm), sizeof(prm));
  output.write((char*)&(val.k_int), sizeof(long));
  output.write((char*)&(val.k_cnt), sizeof(u_long));
  output.write((char*)&(val.k_num), sizeof(double));
  output.write((char*)&(strsize), sizeof(u_long));
  if(strsize!=0)
  {
    output.write(val.k_str.c_str(), strsize); 
  }
}
 
string instruction::print()
{
  ostringstream out;
  out << op_code(opc) << " " << prm[0] << " " << prm[1] << " "
      << prm[2] << " " << prm[3] << " " << val.k_int
      << " " << val.k_cnt << " " << val.k_num << " \""
      << val.k_str << "\"" << '\0';
  return out.str();
}
//-----------------------------------------------------------------------END


//Script--------------------------------------------------------------------
script::script():sz(0),inst(NULL){}
script::~script(){/*if(inst!=NULL){delete [] inst;}*/}
instruction* &script::begin(){return head;}
instruction* &script::end(){return tail;}

void script::load(istream &input)
{
  deque<instruction> in;
  instruction temp;
  while(!input.eof())
  {
    temp.load(input);
    in.push_back(temp);
  }
  inst=new instruction[in.size()+1];
  for(u_long i=0; i<in.size(); i++)
  {
    inst[i]=in[i];
  }
  sz=in.size();
  head=&inst[0]; tail=&inst[in.size()];
}

void script::dump(ostream &output)
{
  for(u_long i=0; i<size(); i++) {inst[i].dump(output);}
} 

u_long script::size(){return sz;}
//-----------------------------------------------------------------------END


//Virtual Machine-----------------------------------------------------------
void k_vm::serror(err_type e)
{
  static char *er[]=
  {
    "Invalid Operation!",
    "Invalid Parameter Values!",
    "Operation Not Supported By Data Type!",
    "Fatal Division by Zero!",
    "Instruction Pointer Lost, No 'end' Operator Found!"
  }; 
  err_out << er[e] << endl; erred=true;
}

k_vm::k_vm(ostream &err):
  erred(false),err_out(err){}

void k_vm::load_script(script &scrpt)
{
  scr=scrpt;
  cur_inst=scr.begin();
  erred=false;
}

script &k_vm::get_script(){return scr;}

void k_vm::point_to(const u_long &ins){cur_inst=scr.begin()+ins;}
instruction* &k_vm::current_instruction(){return cur_inst;}

//void k_vm::trace(const u_long &ins) //located in k_proc.cpp
//void k_vm::execute()  //located in k_proc.cpp
//void k_vm::execute(script &scrpt) //located in k_proc.cpp
//void k_vm::big_switch(const instruction &i) //located in k_proc.cpp

void k_vm::reset(){cur_inst=scr.begin(); mem.clear();}
k_memory &k_vm::mem_stamp(){return mem;}
bool k_vm::at_end(){return cur_inst->opc==end;}
bool k_vm::errored(){return erred;}
//-----------------------------------------------------------------------END
}}
