/*---------------------------------------------------------------------------
Erbium  Assembler

Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "erb_assembler.h"
#include <sstream>

namespace erb { namespace assembler {
using namespace erb::machine;
  bag<label> lab;
  scanner scan;
  ostream *err_out=&cerr; //For Error Reporting
  erb::machine::erb_inst dummy;

//Erbium Assembler-----------------------------------------------------------
void process_prms(const u_long &num, u_long start)
{
  for(u_long i=0; i<num; i++)
  {
    scan.get_token();
	  if(scan.token_type()==str_tok)
	  {
	    if(scan.token_text()=="cnt") dummy.prm[start+i]=0;
	    else if(scan.token_text()=="val") dummy.prm[start+i]=1;
	    else if(scan.token_text()=="elm") dummy.prm[start+i]=2;
	    else {serror(bad_prm); return;}
  	}
    else if(scan.token_type()!=num_tok){serror(bad_prm); return;}
    else dummy.prm[start+i]=atoi(scan.token_text().c_str());
  }
}

void get_imm(const u_long &typ)
{
  scan.get_token();
  int m=1; //Modifier in case its a negative number
  if(scan.token_type()==oper_tok&&scan.token_text()=="-") 
    {m=-1; scan.get_token();}
  if(scan.token_type()!=num_tok){serror(bad_val);}
  switch(typ)
  {
    case 0: 
      char *a; 
      dummy.c_imm=m*strtoul(scan.token_text().c_str(), &a, 0); 
      break;
    case 1: 
      dummy.v_imm=m*strtod(scan.token_text().c_str(), &a); 
      break;
    default: serror(bad_prm); break;
  }
}

bool find_label(const string &s)
{
  for(u_long i=0; i<lab.size(); i++)
  {
    if(lab[i].name==s){return true;}
  }
  return false;
}

u_long label_value(const string &s)
{
  for(u_long i=0; i<lab.size(); i++)
  {
    if(lab[i].name==s){return lab[i].ptr;}
  }
  serror(no_lab);
  return 0;
}

void snag_labels()
{
  scan.location(0); u_long cntr=0;
  while(scan.get_token())
  {
    if(scan.token_text()=="code") break;
  }
  if(scan.token_text()!="code"){serror(bad_data);}
  while(scan.get_token())
  { 
    if(scan.token_type()==comment_tok){continue;}
    if(scan.token_type()!=str_tok){serror(serr); continue;}
    if(scan.look_ahead().tok==":") //Labels
    { 
      if(!find_label(scan.token_text()))
      {
        label tmp; 
        tmp.name=scan.token_text(); tmp.ptr=cntr;
        lab.add(tmp);
      }
      else {serror(dup_lab);}
      scan.get_token(); 
    }
    else if(scan.token_text()=="expand")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="deflate")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="pload")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="spit")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="spitmap")
      {process_prms(1); cntr++;}
    else if(scan.token_text()=="cuni")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="cinter")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="csub")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="crotx")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="croty")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="crotz")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="cmovex")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="cmovey")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="cmovez")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="cscalex")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="cscaley")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="cscalez")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="csetx")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="csety")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="csetz")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="csetw")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="csetl")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="cseth")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="cgetx")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="cgety")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="cgetz")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="cgetw")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="cgetl")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="cgeth")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="ccol")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="alloc")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="load")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="store")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="push")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="pop")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="copy")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="cast")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="copydown")
      {process_prms(2); cntr++;}
    else if(scan.token_text()=="seti")
      {process_prms(2); get_imm(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="add")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="addi")
      {process_prms(3); get_imm(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="sub")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="subi")
      {process_prms(3); get_imm(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="mult")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="multi")
      {process_prms(3); get_imm(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="div")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="divi")
      {process_prms(3); get_imm(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="mod")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="modi")
      {process_prms(3); get_imm(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="seq")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="sne")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="slt")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="sgt")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="slte")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="sgte")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="land")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="lor")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="lnot")
      {process_prms(3); cntr++;}
    else if(scan.token_text()=="seed")
      {process_prms(1); cntr++;}
    else if(scan.token_text()=="timeseed")
      {cntr++;}
    else if(scan.token_text()=="gen")
      {process_prms(4); cntr++;}
    else if(scan.token_text()=="jump")
      {scan.get_token(); cntr++;}
    else if(scan.token_text()=="jchance")
      {process_prms(1); scan.get_token(); cntr++;}
    else if(scan.token_text()=="jeq")
      {process_prms(3); scan.get_token(); cntr++;}
    else if(scan.token_text()=="jne")
      {process_prms(3); scan.get_token(); cntr++;}
    else if(scan.token_text()=="jlt")
      {process_prms(3); scan.get_token(); cntr++;}
    else if(scan.token_text()=="jgt")
      {process_prms(3); scan.get_token(); cntr++;}
    else if(scan.token_text()=="jlte")
      {process_prms(3); scan.get_token(); cntr++;}
    else if(scan.token_text()=="jgte")
      {process_prms(3); scan.get_token(); cntr++;}
    else if(scan.token_text()=="jsub")
      {scan.get_token(); cntr++;}
    else if(scan.token_text()=="ret") 
      {cntr++;}
    else if(scan.token_text()=="end")
      {cntr++;} 
  }
  scan.location(0);
  scan.set_line(1);
}

void serror(err_type e) //Output Error
{
  static char *er[]=
  {
    "Syntax Error",
    "Error Processing Data Section",
    "Invalid Parameter Values!",
    "No Label Found!",
    "Duplicate, Label Already Exists!",
    "Incorrect Immediate Value Type!",
    "This Is Not A Valid Label!"
  };
  *err_out << "Line " << scan.current_line() << ": " 
           << er[e] << endl;
}

void assemble(const string &txt, ostream &out, ostream &er)
{
  err_out=&er;
  scan.setup(txt);
  snag_labels();
  scan.get_token();
  if(scan.token_text()!="data"){serror(bad_data);}
  else
  {
    string s;
    while(scan.get_token())
    {
      if(scan.token_text()=="code") break;
      s+=scan.token_text();
      if(scan.token_text()!="+"&&scan.token_text()!="-") s+=' '; //for case: ( 0 3 -2 )
    }
    if(scan.token_text()!="code"){serror(bad_data);}
    istringstream in; in.str(s);
    construct c;
    if(!c.ascii_in(in)){serror(bad_data);} 
    c.bin_out(out);
  }
  while(scan.get_token())
  {
    if(scan.token_type()==comment_tok){continue;}
    if(scan.token_type()!=str_tok){serror(serr); continue;}
    if(scan.look_ahead().tok==":") //Labels
      {scan.get_token();} 
    else if(scan.token_text()=="expand")
    {
      dummy.op=expand; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="deflate")
    {
      dummy.op=deflate; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="pload")
    {
      dummy.op=pload; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="spit")
    {
      dummy.op=spit; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="spitmap")
    {
      dummy.op=spitmap; process_prms(1); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cuni")
    {
      dummy.op=cuni; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cinter")
    {
      dummy.op=cinter; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="csub")
    {
      dummy.op=csub; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="crotx")
    {
      dummy.op=crotx; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="croty")
    {
      dummy.op=croty; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="crotz")
    {
      dummy.op=crotz; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cmovex")
    {
      dummy.op=cmovex; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cmovey")
    {
      dummy.op=cmovey; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cmovez")
    {
      dummy.op=cmovez; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cscalex")
    {
      dummy.op=cscalex; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cscaley")
    {
      dummy.op=cscaley; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cscalez")
    {
      dummy.op=cscalez; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="csetx")
    {
      dummy.op=csetx; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="csety")
    {
      dummy.op=csety; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="csetz")
    {
      dummy.op=csetz; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="csetw")
    {
      dummy.op=csetw; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="csetl")
    {
      dummy.op=csetl; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cseth")
    {
      dummy.op=cseth; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cgetx")
    {
      dummy.op=cgetx; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cgety")
    {
      dummy.op=cgety; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cgetz")
    {
      dummy.op=cgetz; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cgetw")
    {
      dummy.op=cgetw; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cgetl")
    {
      dummy.op=cgetl; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cgeth")
    {
      dummy.op=cgeth; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="ccol")
    {
      dummy.op=ccol; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="alloc")
    {
      dummy.op=alloc; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="load")
    {
      dummy.op=load; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="store")
    {
      dummy.op=store; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="push")
    {
      dummy.op=push; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="pop")
    {
      dummy.op=pop; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="copy")
    {
      dummy.op=copy; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="cast")
    {
      dummy.op=cast; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="copydown")
    {
      dummy.op=copydown; process_prms(2); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="seti")
    {
      dummy.op=seti; process_prms(2); 
      get_imm(dummy.prm[0]); dummy.bin_out(out);
    }
    else if(scan.token_text()=="add")
    {
      dummy.op=add; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="addi")
    {
      dummy.op=addi; process_prms(3); 
      get_imm(dummy.prm[0]); dummy.bin_out(out);
    }
    else if(scan.token_text()=="sub")
    {
      dummy.op=sub; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="subi")
    {
      dummy.op=subi; process_prms(3); 
      get_imm(dummy.prm[0]); dummy.bin_out(out);
    }
    else if(scan.token_text()=="mult")
    {
      dummy.op=mult; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="multi")
    {
      dummy.op=multi; process_prms(3); 
      get_imm(dummy.prm[0]); dummy.bin_out(out);
    }
    else if(scan.token_text()=="div")
    {
      dummy.op=div; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="divi")
    {
      dummy.op=divi; process_prms(3); 
      get_imm(dummy.prm[0]); dummy.bin_out(out);
    }
    else if(scan.token_text()=="mod")
    {
      dummy.op=mod; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="modi")
    {
      dummy.op=modi; process_prms(3); 
      get_imm(dummy.prm[0]); dummy.bin_out(out);
    }
    else if(scan.token_text()=="seq")
    {
      dummy.op=seq; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="sne")
    {
      dummy.op=sne; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="slt")
    {
      dummy.op=slt; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="sgt")
    {
      dummy.op=sgt; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="slte")
    {
      dummy.op=slte; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="sgte")
    {
      dummy.op=sgte; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="land")
    {
      dummy.op=land; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="lor")
    {
      dummy.op=lor; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="lnot")
    {
      dummy.op=lnot; process_prms(3); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="seed")
    {
      dummy.op=seed; process_prms(1); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="timeseed")
    {
      dummy.op=timeseed; 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="gen")
    {
      dummy.op=gen; process_prms(4); 
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="jump")
    {
      dummy.op=jump; scan.get_token(); 
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[0]=label_value(scan.token_text());}
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="jchance")
    {
      dummy.op=jchance; process_prms(1); scan.get_token(); 
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[0]=label_value(scan.token_text());}
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="jeq")
    {
      dummy.op=jeq; process_prms(3); scan.get_token(); 
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[3]=label_value(scan.token_text());}
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="jne")
    {
      dummy.op=jne; process_prms(3); scan.get_token(); 
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[3]=label_value(scan.token_text());}
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="jlt")
    {
      dummy.op=jlt; process_prms(3); scan.get_token(); 
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[3]=label_value(scan.token_text());}
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="jgt")
    {
      dummy.op=jgt; process_prms(3); scan.get_token(); 
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[3]=label_value(scan.token_text());}
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="jlte")
    {
      dummy.op=jlte; process_prms(3); scan.get_token(); 
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[3]=label_value(scan.token_text());}
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="jgte")
    {
      dummy.op=jgte; process_prms(3); scan.get_token(); 
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[3]=label_value(scan.token_text());}
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="jsub")
    {
      dummy.op=jsub; scan.get_token(); 
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[0]=label_value(scan.token_text());}
      dummy.bin_out(out);
    }
    else if(scan.token_text()=="ret") 
      {dummy.op=ret; dummy.bin_out(out);}
    else if(scan.token_text()=="end")
      {dummy.op=end; dummy.bin_out(out);}
  }
}
//------------------------------------------------------------------------END
}}
