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
struct sym_rec
{
  string name;
  u_long type;
  u_long loc;
  u_long scope;
  bool is_arr; //if it is an array
  bool is_gen; //if it is a construct generation module
  string lab; //label if it is a generatable
  bag<unsigned short> prm; //parameter list for generation modules
  sym_rec():name(""),type(0),loc(0),scope(0),is_arr(0),is_gen(0),lab(""){}
};

string next_label(); //returns next label to be used
void install(char *s, u_long ty); //add a var to sym_tab
void install_prm(char *s, u_long ty); //add a function param var to sym_tab
sym_rec &context_check(char *s, u_long ty); //check that a var is in sym_tab
sym_rec &context_check(char *s); //check that a var is in sym_tab
void dec_scope(); //decrement scope (frees memory)
u_long check_prim(char *s); //make sure a primitive is avail
void check_args(char *s); //Make sure the args passed matches declaration
void code_gen(erb_op op, u_long p1, u_long p2, u_long p3, u_long p4,
              u_long c_imm, double v_imm, string targ=""); //generates code
//------------------------------------------------------------------------END
}
#endif
