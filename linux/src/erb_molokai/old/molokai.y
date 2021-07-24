%{
#include "molokai.h"

using namespace molokai;

int yyerror(char *s);
void check_if_numeric(sym_rec &loc);
void prop_assign_gen(u_long loc);
void sprop_assign_gen(u_long loc1, u_long loc2);
string backpatch_hop_from_here(); //returns name of label
string backpatch_hop_to_here(); //returns name of label
void fix_pop_order_for_args(); //makes sure call values match parameters
extern int yylex();
extern u_long yyline;
namespace molokai 
{ 
  extern u_long used[], cur_scope; 
  extern bag<u_long> backpatch;
  extern u_long bp_count;
  extern bag<machine::erb_op> prop_inst_stack;
  extern bag<sym_rec> sym_tab;
  extern bag<string> src_out;
  extern bag<unsigned short> args;
  extern u_long last_func;
}
%}

%union
{
  char id[256];
  u_long cnt;
  double num;
  u_long line;
}

%start program
%token NUM_TYPE COUNT_TYPE CONSTRUCT_TYPE PRINT PRINTMAP GEN
%token LOAD X Y Z W L H DIM POS ROTATE MOVE SCALE
%token MAYBE OTHERWISE 
%token UPTO LTE GTE EQ NE LOR LAND 
%token <cnt> COUNT <num> NUMBER <id> ID 

%left LAND LOR
%left '<' '>' LTE GTE EQ NE
%left UPTO
%left '-' '+'
%left '*' '/'
%left '!'

%%

program : objects
          {code_gen(machine::end,0,0,0,0,0,0);}

objects : object
        | objects object 

object  : object_head params_head objects '}' formula 
          {
            u_long last_ob=used[2]-1; //save loc of object before dec_scope
            dec_scope();
            u_long loc=sym_tab.top().loc;
            code_gen(machine::copy,2,loc,last_ob,0,0,0); 
            code_gen(machine::copydown,2,loc,0,0,0,0); 
            code_gen(machine::ret,0,0,0,0,0,0); 
            used[2]--;

            backpatch_hop_to_here();
          } 
        | LOAD ID ';' 
          {
            u_long ploc=check_prim($2);
            install($2, 2);
            u_long loc=sym_tab.top().loc;
            code_gen(machine::pload,ploc,loc,0,0,0,0);
          }
        | part
        | declaration ';'
        | statement 

object_head : ID '('
              {
                string s="\n//Generating Obeject "; s+=$1;
                src_out.add(s);
            
                install($1, 2); last_func=sym_tab.size()-1;
                context_check($1).is_gen=true; //is generatable
                cur_scope++; //new scope

                context_check($1).lab=backpatch_hop_from_here();
              }

params_head : param_list ')' '{'
              {
                fix_pop_order_for_args();
              }

param_list : param
           | param ',' param_list

param : COUNT_TYPE ID
        {
          install_prm($2, 0);
          code_gen(machine::pop,1,used[1],0,0,0,0);
          code_gen(machine::cast,0,sym_tab.top().loc,used[1],0,0,0);
        }
      | NUM_TYPE ID
        {
          install_prm($2, 1);
          code_gen(machine::pop,1,sym_tab.top().loc,0,0,0,0);
        }
      | CONSTRUCT_TYPE ID
        {
          install_prm($2, 2);
          code_gen(machine::pop,2,sym_tab.top().loc,0,0,0,0);
        }
      |

statement : assignment ';'
          | scope 
          | maybe 
          | action ';' 

assignment : basic_assign
           | csg_assign
           | property_assign

scope : scopehead objects '}'
        {dec_scope();}

scopehead : '{'
            {cur_scope++;}

maybe : maybehead maybetail OTHERWISE statement
        {
          backpatch_hop_to_here();
        }

             
maybehead : MAYBE '(' expression 
            {
              code_gen(machine::seti,1,used[1]+1,0,0,0,0);
              code_gen(machine::seti,1,used[1]+2,0,0,0,1);
              code_gen(machine::gen,1,used[1],used[1]+1,used[1]+2,0,0);
              code_gen(machine::jgt,1,used[1],used[1]-1,0,0,0);
              backpatch.add(src_out.size()-1);
              used[1]--;
            }

maybetail : ')' statement  
        {
           string lab=next_label();
           src_out[backpatch.top()]+=lab;
           backpatch.pop();
          
           code_gen(machine::jump,0,0,0,0,0,0);
           backpatch.add(src_out.size()-1);
           src_out.add(lab+':');
        }

formula : '[' csg_expression ']' 

part   : part_head part_body

part_head : ID ID ':' 
            {
              u_long loc=context_check($1, 2).loc; 
              install($2, 2);
  
              string s="\n//Generating Part "; s+=$2;
              src_out.add(s);
              
              u_long dest=sym_tab.top().loc;
              code_gen(machine::copy,2,dest,loc,0,0,0);
              cur_scope++;
            } 

part_body : '{' objects '}' 
            {dec_scope();}

declaration : NUM_TYPE ID
              {install($2, 1);}
            | NUM_TYPE ID '[' expression ']'
              {
                install($2, 1);
                sym_tab.top().is_arr=true;
                code_gen(machine::cast,0,used[0],used[1]-1,0,0,0);
                code_gen(machine::alloc,1,sym_tab.top().loc,used[0],0,0,0);
                used[1]--;
              }
            | NUM_TYPE ID '=' expression
              {
                //Fluke the last value in expression in the right place
                used[1]--;
                install($2, 1);
              } 
            | COUNT_TYPE ID 
              {install($2, 0);}
            | COUNT_TYPE ID '[' expression ']'
              {
                install($2, 0);
                sym_tab.top().is_arr=true;
                code_gen(machine::cast,0,used[0],used[1]-1,0,0,0);
                code_gen(machine::alloc,0,sym_tab.top().loc,used[0],0,0,0);
                used[1]--;
              }
            | COUNT_TYPE ID '=' expression
              {
                install($2, 0);
                sym_rec loc=context_check($2);
                check_if_numeric(loc);
                code_gen(machine::cast,loc.type,loc.loc,used[1]-1,0,0,0);
                used[1]--;
              } 
            | CONSTRUCT_TYPE ID 
              {install($2, 2);}
            | CONSTRUCT_TYPE ID '[' expression ']'
              {
                install($2, 2);
                sym_tab.top().is_arr=true;
                code_gen(machine::cast,0,used[0],used[1]-1,0,0,0);
                code_gen(machine::alloc,2,sym_tab.top().loc,used[0],0,0,0);
                used[1]--;
              }
            | CONSTRUCT_TYPE ID '=' '[' csg_expression ']'
              {
                used[2]--;
                install($2, 2);
              } 
            | ID ID
              {
                install($2, 2);
                u_long loc1=context_check($1, 2).loc;
                u_long loc2=context_check($2, 2).loc;
                code_gen(machine::copy,2,loc2,loc1,0,0,0);
              }

action : ID '.' PRINT 
         {
           sym_rec targ=context_check($1, 2);
           code_gen(machine::spit,targ.type,targ.loc,0,0,0,0);
         }
       | ID '.' PRINTMAP 
         {
           sym_rec targ=context_check($1, 2);
           code_gen(machine::spitmap,targ.loc,0,0,0,0,0);
         }
       | ID '.' GEN '(' value_list ')'
         {
           check_args($1);
           sym_rec targ=context_check($1, 2);
           if(targ.is_gen)
             code_gen(machine::jsub,0,0,0,0,0,0,targ.lab);
         }
       | ID '.' GEN '(' value_list ')' ':' ID
         {
           check_args($1);
           sym_rec targ=context_check($1, 2);
           if(targ.is_gen)
             code_gen(machine::jsub,0,0,0,0,0,0,targ.lab);
           u_long loc=context_check($8, 2).loc;
           code_gen(machine::copy,2,loc,targ.loc,0,0,0);
         }
       | scale 
       | rotate 
       | move 

value_list : value
           | value ',' value_list

value : expression
        {
          args.add(1);
          code_gen(machine::push,1,used[1]-1,0,0,0,0);
          used[1]--;
        }
      | '[' csg_expression ']'
        {
          args.add(2);
          code_gen(machine::push,2,used[2]-1,0,0,0,0);
          used[2]--;
        }
      |

scale : ID '.' SCALE '.' axis '(' expression ')'
        {
          u_long loc=context_check($1, 2).loc;
          erb_op op=prop_inst_stack[prop_inst_stack.size()-1];
          code_gen((erb_op)(op+6), loc, loc, used[1]-1, 0, 0, 0); 
          prop_inst_stack.del(prop_inst_stack.size()-1);
          used[1]--;
        }
      | ID '.' SCALE '(' expression ',' expression ',' expression ')'
        {
          u_long loc=context_check($1, 2).loc;
          code_gen(machine::cscalex, loc, loc, used[1]-3, 0, 0, 0); 
          code_gen(machine::cscaley, loc, loc, used[1]-2, 0, 0, 0); 
          code_gen(machine::cscalez, loc, loc, used[1]-1, 0, 0, 0); 
          used[1]-=3;
        }
          
rotate : ID '.' ROTATE '.' axis '(' expression ')'
        {
          u_long loc=context_check($1, 2).loc;
          erb_op op=prop_inst_stack[prop_inst_stack.size()-1];
          code_gen((erb_op)(op), loc, loc, used[1]-1, 0, 0, 0); 
          prop_inst_stack.del(prop_inst_stack.size()-1);
          used[1]--;
        }
      | ID '.' ROTATE '(' expression ',' expression ',' expression ')'
        {
          u_long loc=context_check($1, 2).loc;
          code_gen(machine::crotx, loc, loc, used[1]-3, 0, 0, 0); 
          code_gen(machine::croty, loc, loc, used[1]-2, 0, 0, 0); 
          code_gen(machine::crotz, loc, loc, used[1]-1, 0, 0, 0); 
          used[1]-=3;
        }

move: ID '.' MOVE '.' axis '(' expression ')'
        {
          u_long loc=context_check($1, 2).loc;
          erb_op op=prop_inst_stack[prop_inst_stack.size()-1];
          code_gen((erb_op)(op+3), loc, loc, used[1]-1, 0, 0, 0); 
          prop_inst_stack.del(prop_inst_stack.size()-1);
          used[1]--;
        }
      | ID '.' MOVE '(' expression ',' expression ',' expression ')'
        {
          u_long loc=context_check($1, 2).loc;
          code_gen(machine::cmovex, loc, loc, used[1]-3, 0, 0, 0); 
          code_gen(machine::cmovey, loc, loc, used[1]-2, 0, 0, 0); 
          code_gen(machine::cmovez, loc, loc, used[1]-1, 0, 0, 0); 
          used[1]-=3;
        }

axis : X
       {prop_inst_stack.add(machine::crotx);}
     | Y
       {prop_inst_stack.add(machine::croty);}
     | Z
       {prop_inst_stack.add(machine::crotz);}
      


basic_assign : ID '=' expression 
               {
                 sym_rec loc=context_check($1); 
                 check_if_numeric(loc);
                 if(loc.type==0)
                   code_gen(machine::cast,loc.type,loc.loc,used[1]-1,0,0,0);
                 if(loc.type==1)
                   code_gen(machine::copy,loc.type,loc.loc,used[1]-1,0,0,0);
                 used[1]--;
               } 

csg_assign : ID '=' '[' csg_expression ']' 
             {
               u_long loc=context_check($1, 2).loc;
               code_gen(machine::copy,2,loc,used[2]-1,0,0,0);
               used[2]--;
             } 

property_assign: ID '.' property '=' expression 
                 {prop_assign_gen(context_check($1, 2).loc);} 
               | property '=' expression 
                 {if(cur_scope) prop_assign_gen(sym_tab.top().loc);} 
               | ID '.' special_prop '=' ID '.' special_prop 
                 {
                   u_long loc1=context_check($1, 2).loc;
                   u_long loc2=context_check($5, 2).loc;
                   sprop_assign_gen(loc1, loc2);
                 }
               | ID '.' special_prop '=' special_prop 
                 {
                   if(!cur_scope) return 0;
                   u_long loc1=context_check($1, 2).loc;
                   u_long loc2=sym_tab.top().loc;
                   sprop_assign_gen(loc1, loc2);
                 }
               | special_prop '=' ID '.' special_prop
                 {
                   if(!cur_scope) return 0;
                   u_long loc1=sym_tab.top().loc;
                   u_long loc2=context_check($3, 2).loc;
                   sprop_assign_gen(loc1, loc2);
                 }
               | special_prop '=' special_prop 
                 {
                   if(!cur_scope) return 0;
                   u_long loc1=sym_tab.top().loc;
                   u_long loc2=sym_tab.top().loc;
                   sprop_assign_gen(loc1, loc2);
                 }


csg_expression : ID
                 {
                   used[2]++;
                   u_long loc=context_check($1, 2).loc;
                   code_gen(machine::copy,2,used[2]-1,loc,0,0,0);
                 } 
               | csg_expression '-' csg_expression
                 {
                   code_gen(machine::csub,used[2]-2,used[2]-2,used[2]-1,0,0,0); 
                   used[2]--;
                 } 
               | csg_expression '+' csg_expression
                 {
                   code_gen(machine::cuni,used[2]-2,used[2]-2,used[2]-1,0,0,0); 
                   used[2]--;
                 } 
               | csg_expression '*' csg_expression 
                 {
                   code_gen(machine::cinter,used[2]-2,used[2]-2,used[2]-1,0,0,0); 
                   used[2]--;
                 } 
               | '(' csg_expression ')'  
           
expression : NUMBER
             {used[1]++; code_gen(machine::seti,1,used[1]-1,0,0,0,$1);} 
           | ID
             {
               used[1]++;
               sym_rec loc=context_check($1);
               check_if_numeric(loc);
               if(loc.type==0)
                 code_gen(machine::cast,loc.type,used[1]-1,loc.loc,0,0,0);
               if(loc.type==1)
                 code_gen(machine::copy,loc.type,used[1]-1,loc.loc,0,0,0);
             }
           | ID '.' property
             {
               used[1]++;
               u_long loc=context_check($1, 2).loc;
               erb_op op=prop_inst_stack[prop_inst_stack.size()-1];
               code_gen(op, used[1]-1, loc, 0, 0, 0, 0); 
               prop_inst_stack.del(prop_inst_stack.size()-1);
             } 
           | property
             {
               used[1]++;
               u_long loc=sym_tab.top().loc;
               erb_op op=prop_inst_stack[prop_inst_stack.size()-1];
               code_gen(op, used[1]-1, loc, 0, 0, 0, 0); 
               prop_inst_stack.del(prop_inst_stack.size()-1);
             } 
           | COUNT
             {used[1]++; code_gen(machine::seti,1,used[1]-1,0,0,0,$1);} 
           | expression '<' expression
             {code_gen(machine::slt,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression '>' expression
             {code_gen(machine::sgt,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression LTE expression
             {code_gen(machine::slte,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression GTE expression
             {code_gen(machine::sgte,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression EQ expression
             {code_gen(machine::seq,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression NE expression
             {code_gen(machine::sne,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression LAND expression
             {code_gen(machine::land,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression LOR expression
             {code_gen(machine::lor,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | '!' expression
             {code_gen(machine::lnot,1,used[1]-1,used[1]-1,0,0,0,0); used[1]--;} 
           | expression UPTO expression
             {code_gen(machine::gen,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression '-' expression
             {code_gen(machine::sub,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression '+' expression
             {code_gen(machine::add,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression '*' expression
             {code_gen(machine::mult,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | expression '/' expression
             {code_gen(machine::div,1,used[1]-2,used[1]-2,used[1]-1,0,0); used[1]--;} 
           | '(' expression ')' 

property : X
           {prop_inst_stack.add(machine::cgetx);}
         | Y
           {prop_inst_stack.add(machine::cgety);}
         | Z
           {prop_inst_stack.add(machine::cgetz);}
         | W
           {prop_inst_stack.add(machine::cgetw);}
         | L
           {prop_inst_stack.add(machine::cgetl);}
         | H
           {prop_inst_stack.add(machine::cgeth);}

special_prop :
             | POS
               {prop_inst_stack.add(machine::cgetx);} //dummy indicator for pos
             | DIM 
               {prop_inst_stack.add(machine::cgetw);} //dummy indicator for dim

         
%%

void check_if_numeric(sym_rec &loc)
{
  if(loc.type==2)
    cout << "Error Line " << yyline << ": " << loc.name 
         << " is of an invalid type." << endl;
}

void prop_assign_gen(u_long loc)
{  
  switch(prop_inst_stack[prop_inst_stack.size()-1])
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
  used[1]--;
  prop_inst_stack.del(prop_inst_stack.size()-1);
}

void sprop_assign_gen(u_long loc1, u_long loc2)
{
  switch(prop_inst_stack[prop_inst_stack.size()-1])
  {
    case machine::cgetx:
    {
      code_gen(machine::cgetx, used[1], loc2, 0, 0, 0, 0); 
      code_gen(machine::csetx, loc1, loc1, used[1], 0, 0, 0);
      code_gen(machine::cgety, used[1], loc2, 0, 0, 0, 0); 
      code_gen(machine::csety, loc1, loc1, used[1], 0, 0, 0);
      code_gen(machine::cgetz, used[1], loc2, 0, 0, 0, 0); 
      code_gen(machine::csetz, loc1, loc1, used[1], 0, 0, 0);
    } break;
    case machine::cgetw:
    {
      code_gen(machine::cgetw, used[1], loc2, 0, 0, 0, 0); 
      code_gen(machine::csetw, loc1, loc1, used[1], 0, 0, 0);
      code_gen(machine::cgetl, used[1], loc2, 0, 0, 0, 0);
      code_gen(machine::csetl, loc1, loc1, used[1], 0, 0, 0);
      code_gen(machine::cgeth, used[1], loc2, 0, 0, 0, 0); 
      code_gen(machine::cseth, loc1, loc1, used[1], 0, 0, 0);
    } break;
    default: break;
  }
  prop_inst_stack.del(prop_inst_stack.size()-1);
}

string backpatch_hop_from_here()
{
  code_gen(machine::jump,0,0,0,0,0,0);
  backpatch.add(src_out.size()-1);
  string lab=next_label();
  src_out.add(lab+':');
  return lab;
}

string backpatch_hop_to_here()
{
  string lab=next_label();
  src_out[backpatch[backpatch.size()-1]]+=lab;
  src_out.add(lab+':');
  backpatch.del(backpatch.size()-1);
  return lab;
}

void fix_pop_order_for_args()
{
  bag<string> reorder;
  string cast="";
  for(u_long i=0; i<sym_tab[last_func].prm.size(); i++)
  {
    string s=src_out.top(); src_out.pop();
    if(s[0]=='c'){cast=s; continue;} //Maintains cast command in order with pop
    reorder.add(s);
    if(cast!=""){reorder.add(cast); cast="";}
  }
  for(u_long i=0; i<sym_tab[last_func].prm.size(); i++)
  {
    src_out.add(reorder[i]); 
  }
}

int yyerror(char *s)
{
  printf("Error Line %lu: %s", yyline, s);
  return 0;
}
