/*---------------------------------------------------------------------------
Molokai - the erbium high level language

Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "molokai.h"
#include <sstream>
 
extern u_long yyline;
extern void yyparse();
extern FILE *yyin;

namespace molokai { 
//Molokai--------------------------------------------------------------------
compiler_state state;
  
compiler_state::compiler_state(){used[0]=used[1]=used[2]=1;in_loop=0;} //FIXME: Hack 2 0th element reserved for return vals

void compiler_state::clear()
{
  *this=compiler_state();
}
  
void compiler_state::inc(sym_type t)
{
  code_gen(machine::push,0,t,used[t],0,0,0); //FIXME: Hack 1
  used[t]++;
}
void compiler_state::dec(sym_type t)
{
  used[t]--;
  code_gen(machine::pop,0,t,used[t],0,0,0); //FIXME: Hack 1 (pushing and poping for expressions)
}
u_long compiler_state::top(sym_type t){return used[t]-1;}

void compiler_state::add_prim(const string &s)
{
  prim_name.add(s);
}

string compiler_state::next_label()
{
  ostringstream s;
  s << "label" << ++label_count;
  return s.str();
} 

void compiler_state::add_symbol(const char *s, u_long ty, symbol::symkind k)
{
  for(u_long i=0; i<sym.size(); i++)
    if(s==sym[i].name)
    {
      cerr << "Error Line " << yyline << ": " << s 
           << " is defined more than once!" << endl;
      return;
    }
  symbol sy; sy.name=s; sy.kind=k;
  sy.type=ty; sy.loc=used[ty]; used[ty]++; 
  code_gen(machine::push,0,ty,used[ty]-1,0,0,0);

  //fix for call to fix_pop_order
  if(k==symbol::prm){prm_push.add(src_out.top()); src_out.pop();}
  //these pushes are emitted in fix_pop_order
  
  if(sy.kind==symbol::arr)
  {
    sy.mem_loc=used[0]; used[0]++;
    code_gen(machine::push,0,0,used[0]-1,0,0,0);
  }
  if(sy.kind==symbol::prm){sym[last_proc].prmlist.add(ty);}
  if(sy.kind==symbol::proc){last_proc=sym.size();}
  sy.scope=cur_scope;
  sym.add(sy);
}

symbol &compiler_state::last_symbol()
{
  return sym.top();
}

symbol &compiler_state::check_for(const char *s, u_long ty, symbol::symkind k)
{
  for(u_long i=0; i<sym.size(); i++)
    if(s==sym[i].name)
    {
      if((sym[i].kind==k||k==symbol::any)&&(sym[i].type==ty||ty==5||
          (ty==6&&sym[i].type<2)))
      return sym[i];
    }
  cerr << "Error Line " << yyline << ": " 
       << s << " is not defined (or used) correctly!" << endl;
  return sym[0];
}

u_long compiler_state::check_prim(char *s)
{
  for(u_long i=0; i<prim_name.size(); i++)
    if(s==prim_name[i])
    {
      return i;
    }
  cerr << "Error Line " << yyline << ": " << s 
       << " is not a valid builder!" << endl;
  return 0;
}

void compiler_state::inc_scope(){cur_scope++;}

void compiler_state::dec_scope()
{
  cur_scope--;
  for(u_long i=0; i<sym.size(); i++)
  {
    if(sym[i].scope>cur_scope)
    {
      code_gen(machine::pop,0,sym[i].type,used[sym[i].type]-1,0,0,0); //restore state
      prev_context.add(src_out.top()); src_out.pop();
      used[sym[i].type]--; //free mem locations
      if(sym[i].kind==symbol::arr) //if is array type free pointer
      {
        code_gen(machine::pop,0,0,used[0]-1,0,0,0); //restore state
        prev_context.add(src_out.top()); src_out.pop();
        used[0]--;
      } 
      sym.del(i); i--;
    }
  }
}

void compiler_state::restore_context()
{
  //while(prev_context.size()){src_out.add(prev_context.top()); prev_context.pop();}
  bag<string> reverse;
  while(prev_context.size()){reverse.add(prev_context.top()); prev_context.pop();}
  while(reverse.size()){src_out.add(reverse.top()); reverse.pop();} //correct pop order
}

bag<string> &compiler_state::src()
{
  return src_out;
}

void compiler_state::check_args(string id)
{
  for(u_long i=0; i<sym.size(); i++)
    if(id==sym[i].name)
    {
      if(sym[i].prmlist.size()==args.size())
      {
        for(u_long j=0; j<args.size(); j++)
        {
          if(sym[i].prmlist[j]<2&&args[j]<2) continue;
          if(sym[i].prmlist[j]!=args[j])
            cerr << "Error Line " << yyline << ": Call to " 
                 << id << ", invalid parameter list!" << endl;
        }
      }
      else
        cerr << "Error Line " << yyline << ": Call to " 
             << id << ", invalid parameter list!" << endl;
    }
  args.clear();
}

void compiler_state::prop_assign_gen(u_long loc)
{
  switch(prop_inst_stack.top())
  {
    case machine::cgetx:
      {code_gen(machine::csetx, loc, loc, used[1]-1, 0, 0, 0);}
      break;
    case machine::cgety:
      {code_gen(machine::csety, loc, loc, used[1]-1, 0, 0, 0);}
      break;
    case machine::cgetz:
      {code_gen(machine::csetz, loc, loc, used[1]-1, 0, 0, 0);}
      break;
    case machine::cgetw:
      {code_gen(machine::csetw, loc, loc, used[1]-1, 0, 0, 0);}
      break;
    case machine::cgetl:
      {code_gen(machine::csetl, loc, loc, used[1]-1, 0, 0, 0);}
      break;
    case machine::cgeth:
      {code_gen(machine::cseth, loc, loc, used[1]-1, 0, 0, 0);}
      break;
    default: break;
  }
  prop_inst_stack.pop();
}

string compiler_state::backpatch_hop_from_here()
{
  code_gen(machine::jump,0,0,0,0,0,0);
  backpatch.add(src_out.size()-1);
  string lab=next_label();
  src_out.add(lab+':');
  return lab;
}

string compiler_state::backpatch_hop_to_here()
{
  string lab=next_label();
  src_out[backpatch.top()]+=lab;
  src_out.add(lab+':');
  backpatch.pop();
  return lab;
}

void compiler_state::fix_pop_order_for_args()
{
  bag<string> reorder, reverse;
  string cast="";
  for(u_long i=0; i<sym[last_proc].prmlist.size(); i++)
  {
    string s=src_out.top(); src_out.pop();
    if(s[0]=='c'){cast=s; i--; continue;} //Maintains cast command in order with pop
    reorder.add(s);
    if(cast!=""){reorder.add(cast); cast="";}
  }
  while(prm_push.size()){reverse.add(prm_push.top()); prm_push.pop();}
  while(reverse.size()){src_out.add(reverse.top()); reverse.pop();}
  for(u_long i=0; i<reorder.size(); i++)
  {
    src_out.add(reorder[i]); 
  }
}
//------------------------------------------------------------------------END
  

  
//Compile Function-----------------------------------------------------------
string mol_compile(char *fn)
{
  compiler_state &cmplr=state;
  cmplr.clear();
  
  //open necessary files
  yyin=fopen(fn, "r");
  ifstream pfile("builders.prim", ios::in);
  
  //load in the builders
  construct prims; 
  while(!pfile.eof())
  {
    string s;
    pfile >> s; 
    if(pfile.eof()) break;
    cmplr.add_prim(s); //add name to table
    brush b;
    b.ascii_in(pfile);
    prims.add(b);
  }
  
  //emit data section to output code
  cmplr.src().add("data\n");
  ostringstream data;
  prims.ascii_out(data);
  cmplr.src().add(data.str());
  
  //emit code section to output code
  cmplr.src().add("code\n");
  cmplr.code_gen(machine::timeseed,0,0,0,0,0,0);
  cmplr.code_gen(machine::expand,0,128,0,0,0,0);
  cmplr.code_gen(machine::expand,1,128,0,0,0,0);
  cmplr.code_gen(machine::expand,2,128,0,0,0,0);
  yyparse(); 
  fclose(yyin);
  
  string ret;
  for(u_long i=0; i<cmplr.src().size(); i++)
  {
    ret+=cmplr.src()[i]+"\n";
  }

  return ret;
}
//------------------------------------------------------------------------END

 
}

using namespace molokai;

