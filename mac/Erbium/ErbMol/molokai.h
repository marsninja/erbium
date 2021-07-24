/*---------------------------------------------------------------------------
Molokai - the erbium high level language

Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#ifndef MolokaiHeader
#define MolokaiHeader

#include "k-utils.h"
#include "erb_machine.h"

using namespace std;
using namespace k;
using namespace erb;
using namespace erb::machine;
using k::u_long;

namespace molokai {
//Molokai--------------------------------------------------------------------
struct symbol
{
  string name;
  u_long type;
  enum symkind { var, proc, arr, prm, any } kind; //Describes what kind of symbol
  
  u_long loc; //associated with 'register'
  u_long mem_loc; //used for arrays to point to location in memory uses count
  u_long scope; //static scoping
  string lab; //label if it is a procedure
  bag<unsigned short> prmlist; //parameter list for procedures

  symbol():name(""),type(0),kind(var),loc(0),scope(0),lab(""){}
};

class compiler_state
{
private:
  bag<symbol> sym;
  u_long used[3], cur_scope; //remembers 'regs' and scoping
  u_long label_count; //keeps track of labels
  bag<string> prim_name; //names of all loaded primatives
private:
  u_long bp_count; //bp_count used if same label needs patched many times
  u_long last_proc; //saves the last procedure being parsed
  bag<string> prm_push; //buffers the pushes for context saving
  bag<string> prev_context; //buffers the pops for context restoring
public:
  bag<unsigned short> args; //list of arg types for a procedure call
  bag<string> src_out; //output assembly file
  enum sym_type {count=0, number=1, construct=2}; 
  bag<machine::erb_op> prop_inst_stack; //holds the property value
  bag<u_long> backpatch; //stack of all backpatch locations
  bag<sym_type> last_type; //saves the last type specified
  bag<string> last_id; //saves the last id being parsed, @ signifies array
  bag<string> loop_lab; //the looping label to jump 2
  u_long in_loop; //are we in the body of a loop or not
  bag<u_long> num_breaks; //how many breaks to backpatch
  bag<u_long> break_loc; //the break locations to backpatch
public:
  compiler_state();
  void clear();
public:
  void inc(sym_type t); //allots a 'register' to be used
  void dec(sym_type t); //frees a 'register' to be used
  u_long top(sym_type t); //last used 'register'
public:
  void add_prim(const string &s); //Adds a primitave to the compilers knowlege
  string next_label(); //returns a fresh label to be used
  void add_symbol(const char *s, u_long ty, symbol::symkind k); //add a symbol
  symbol &last_symbol(); //return the last symbol that was added
  symbol &check_for(const char *s, u_long ty=5, symbol::symkind k=symbol::any);
  u_long check_prim(char *s); //returns index of prim
  void inc_scope(); //static scoping step in
  void dec_scope(); //static scoping step out
  void restore_context(); //brings the context back
  bag<string> &src(); //print string to assembly file
public:
  void check_if_numeric(symbol &loc);
  void check_args(string id); //Makes sure that argument list matches decl
  void prop_assign_gen(u_long loc);
  string backpatch_hop_from_here(); //returns name of label
  string backpatch_hop_to_here(); //returns name of label
  void fix_pop_order_for_args(); //makes sure call values match parameters
public:
  void code_gen(erb_op op, u_long p1, u_long p2, u_long p3, u_long p4,
              u_long c_imm, double v_imm, string targ=""); //generates code
};
//------------------------------------------------------------------------END
  
  
//Compile Function-----------------------------------------------------------
string mol_compile(char *fn);
//------------------------------------------------------------------------END
}
#endif
