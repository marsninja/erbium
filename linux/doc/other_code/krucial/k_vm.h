#include "k-utils.h"
#include <stack>
#include <vector>

#ifndef Krucial_VM_Header
#define Krucial_VM_Header
namespace k { namespace krucial {
//Op Codes------------------------------------------------------------------
enum op_code
{
  expand, //expand <t1> <units>; 0=long, 1=count, 2=number, 3=string
  deflate, //deflate <t1> <units>; 0=long, 1=count, 2=number, 3=string
  set, //set <t1> <v1> <v2>; v1 and v2 is index location of variable 
  seti, //seti <t1> <v1> <value>; value=constant or string;
  cast, //cast <t1> <v1> <t2> <v2>; cast var of type 2 to var of type 1
  push, //push <t1> <v1>; pushes a var unto the stack
  pushi, //pushi <t1> <value>; pushes a value onto the stack
  pop, //pop <t1> <v1>; pops a value of the stack
 
  add, //add <t1> <v1> <v2> <v3>; v1=v2+v3;
  addi, //addi <t1> <v1> <v2> <value>; v1=v2+value;
  sub, //sub <t1> <v1> <v2> <v3>; t only 0,1,2 not 3; v1=v2-v3;
  subi, //subi <t1> <v1> <v2> <value>; t only 0,1,2 not 3; v1=v2-value;
  mult, //mult <t1> <v1> <v2> <v3>; t only 0,1,2 not 3; v1=v2*v3;
  multi, //multi <t1> <v1> <v2> <value>; t only 0,1,2 not 3; v1=v2*value;
  div, //div <t1> <v1> <v2> <v3>; t only 0,1,2 not 3; v1=v2/v3;
  divi, //divi <t1> <v1> <v2> <value>; t only 0,1,2 not 3; v1=v2/value;
  mod, //mod <t1> <v1> <v2> <v3>; t only 0,1 not 2,3; v1=v2%v3;
  modi, //modi <t1> <v1> <v2> <value>; t only 0,1 not 2,3; v1=v2%value;

  land, //and <t1> <v1> <v2> <v3>; v1=v2&&v3;
  lor, //or <t1> <v1> <v2> <v3>; v1=v2||v3;
  lnot, //not <t1> <v1> <v2>; v1=!v2;

  jump, //jump <loc>; loc is the instruction to jump too
  jeq, //jeq <t1> <v1> <v2> <loc>; jump if v1 equal v2
  jne, //jne <t1> <v1> <v2> <loc>; jump if v1 not equal v2
  jsub, //jsub <loc>; jump + save location for return 
  ret, //ret; returns from a subroutine 

  slt, //slt <t1> <v1> <v2> <v3>; v1=v2<v3;
  sgt, //sgt <t1> <v1> <v2> <v3>; v1=v2>v3;
  slte, //slte <t1> <v1> <v2> <v3>; v1=v2<=v3;
  sgte, //sgte <t1> <v1> <v2> <v3>; v1=v2>=v3;
  seq, //seq <t1> <v1> <v2> <v3>; v1=v2==v3;

  end, //end; terminates program
  syscall //syscall ...; system functions built into the vm
          //Print: 0 <t1> <v1>
          //Input: 1 <t1> <v1>
};
//-----------------------------------------------------------------------END


//Memory--------------------------------------------------------------------
class instruction;
class k_memory
{
private:
  stack< instruction* > ra; //return address for (jsub, and ret)
private:
  long k_reg_int[32]; //Registers.... Kinda
  u_long k_reg_cnt[32];
  double k_reg_num[32];
  string k_reg_str[32];
private:
  vector<long*> k_int; //Memory
  vector<u_long*> k_cnt;
  vector<double*> k_num;
  vector<string*> k_str;
private: 
  stack<long> k_s_int; //For pushing and poping instructions
  stack<u_long> k_s_cnt;
  stack<double> k_s_num;
  stack<string> k_s_str;
public:
  k_memory();
  ~k_memory();
  void clear();
public:
  void push_ra(instruction* &l);
  instruction* pop_ra();
public:
  void expand_int(const u_long &l);
  void expand_cnt(const u_long &l);
  void expand_num(const u_long &l);
  void expand_str(const u_long &l);
  void deflate_int(u_long l);
  void deflate_cnt(u_long l);
  void deflate_num(u_long l);
  void deflate_str(u_long l);
  long &integer(const u_long &id);
  u_long &count(const u_long &id);
  double &number(const u_long &id);
  string &c_string(const u_long &id);
  u_long integer_size();
  u_long count_size();
  u_long number_size();
  u_long c_string_size();
public:
  void push_int(const long &val);
  void push_cnt(const u_long &val);
  void push_num(const double &val);
  void push_str(const string &val);
  long pop_int();
  u_long pop_cnt();
  double pop_num();
  string pop_str();
  stack<long> &int_see_stack();
  stack<u_long> &cnt_see_stack();
  stack<double> &num_see_stack();
  stack<string> &str_see_stack();
public:
  string print();
};
//-----------------------------------------------------------------------END


//Instruction---------------------------------------------------------------
class instruction
{
public:
  instruction();
  void clear();
  void load(istream &input);
  void dump(ostream &output);
  string print(); //For Debug Purposes
public:
  op_code opc;
  u_long prm[4];
  struct value //immediate values
  {
    long k_int;
    u_long k_cnt;
    double k_num;
    string k_str;
  } val;
};
//-----------------------------------------------------------------------END


//Script--------------------------------------------------------------------
class script
{
private://private:
  u_long sz; //size
  instruction *inst, *head, *tail;
  instruction* it;
public:
  script();
  ~script();
  instruction* &begin();
  instruction* &end();
  void load(istream &input);
  void dump(ostream &output);
  u_long size();
};
//-----------------------------------------------------------------------END


//Virtual Machine-----------------------------------------------------------
class k_vm
{
private:
  k_memory mem; //VM Memory
  instruction* cur_inst; //current instruction
  script scr; //current script
private:
  ostream &err_out; //For Error Reporting
  bool erred;
  enum err_type {inval_op, inval_prm, bad_typ_op, div_zero, no_end};
  void serror(err_type e); //Output Error
public:
  k_vm(ostream &err=cout);
  void load_script(script &scrpt);
  script &get_script();
  void point_to(const u_long &ins);
  instruction* &current_instruction();
  void trace(const u_long &ins=1);
  void execute();
  void execute(script &scrpt);
  void big_switch(const instruction &i);
  k_memory &mem_stamp();
  void reset();
  bool at_end();
  bool errored(); //Returns true if error occured
};
//-----------------------------------------------------------------------END
}}
#endif
