#include "k_low.h"

namespace k { namespace krucial {
//Label------------------------------------------------------------------END
label::label():name(""),ptr(0){}
//--------------------------------------------------------------------------


//Assembler--------------------------------------------------------------END
void k_low::process_params(const u_long &num, u_long start)
{
  for(u_long i=0; i<num; i++)
  {
    scan.get_token();
	if(scan.token_type()==str_tok)
	{
	  if(scan.token_text()=="int") dummy.prm[start+i]=0;
	  else if(scan.token_text()=="cnt") dummy.prm[start+i]=1;
	  else if(scan.token_text()=="num") dummy.prm[start+i]=2;
	  else if(scan.token_text()=="str") dummy.prm[start+i]=3;
	  else {serror(bad_param); return;}
	}
    else if(scan.token_type()!=num_tok){serror(bad_param); return;}
    else dummy.prm[start+i]=atoi(scan.token_text().c_str());
  }
}

void k_low::get_immediate(const u_long &typ)
{
  scan.get_token();
  if(typ==3)
  {
    if(scan.token_type()!=quote_tok){serror(bad_val);}
    else {dummy.val.k_str=scan.token_text();}
  }
  else
  {
    int m=1; //Modifier in case its a negative number
    if(scan.token_type()==oper_tok&&scan.token_text()=="-") 
      {m=-1; scan.get_token();}
    if(scan.token_type()!=num_tok){serror(bad_val);}
    switch(typ)
    {
      case 0: dummy.val.k_int=m*atoi(scan.token_text().c_str()); break;
      case 1: dummy.val.k_cnt=atoi(scan.token_text().c_str()); break;
      case 2: dummy.val.k_num=m*atof(scan.token_text().c_str()); break;
    }
  }
}

bool k_low::find_label(const string &s)
{
  for(u_long i=0; i<lab.size(); i++)
  {
    if(lab[i].name==s){return true;}
  }
  return false;
}

u_long k_low::label_value(const string &s)
{
  for(u_long i=0; i<lab.size(); i++)
  {
    if(lab[i].name==s){return lab[i].ptr;}
  }
  serror(no_lab);
  return inst_num;
}

void k_low::snag_labels()
{
  scan.location(0); u_long cntr=0;
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
        lab.push_back(tmp);
      }
      else {serror(dup_lab);}
      scan.get_token(); 
    } 
    else if(scan.token_text()=="expand") //expand <t1> <units>
      {process_params(2); cntr++;}
    else if(scan.token_text()=="deflate") //deflate <t1> <units>
      {process_params(2); cntr++;}
    else if(scan.token_text()=="set") //set <t1> <v1> <v2>
      {process_params(3); cntr++;}
    else if(scan.token_text()=="seti") //seti <t1> <v1> <value>
      {process_params(2); get_immediate(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="cast") //cast <t1> <v1> <t2> <v2>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="push") //push <t1> <v1>
      {process_params(2); cntr++;}
    else if(scan.token_text()=="pushi") //pushi <t1> <value>
      {process_params(1); get_immediate(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="pop") //pop <t1> <v1>
      {process_params(2); cntr++;}
    else if(scan.token_text()=="add") //add <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="addi") //addi <t1> <v1> <v2> <value>
      {process_params(3); get_immediate(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="sub") //sub <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="subi") //subi <t1> <v1> <v2> <value>
      {process_params(3); get_immediate(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="mult") //mult <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="multi") //multi <t1> <v1> <v2> <value>
      {process_params(3); get_immediate(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="div") //div <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="divi") //divi <t1> <v1> <v2> <value>
      {process_params(3); get_immediate(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="mod") //mod <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="modi") //modi <t1> <v1> <v2> <value>
      {process_params(3); get_immediate(dummy.prm[0]); cntr++;}
    else if(scan.token_text()=="bcpl") //bcpl <t1> <v1> <v2>
      {process_params(3); cntr++;}
	else if(scan.token_text()=="and") //and <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="or") //or <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="not") //not <t1> <v1> <v2>
      {process_params(3); cntr++;}
    else if(scan.token_text()=="jump") //jump <label>
      {scan.get_token(); cntr++;}
    else if(scan.token_text()=="jeq") //jeq <t1> <v1> <v2> <label>
      {process_params(3); scan.get_token(); cntr++;}
    else if(scan.token_text()=="jne") //jne <t1> <v1> <v2> <label>
      {process_params(3); scan.get_token(); cntr++;}
    else if(scan.token_text()=="jsub") //jsub <label>
      {scan.get_token(); cntr++;}
    else if(scan.token_text()=="ret") //ret
      {cntr++;}
    else if(scan.token_text()=="slt") //slt <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="sgt") //sgt <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="slte") //slte <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="sgte") //sgte <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="seq") //seq <t1> <v1> <v2> <v3>
      {process_params(4); cntr++;}
    else if(scan.token_text()=="end") //end
      {cntr++;} 
    else if(scan.token_text()=="syscall") //syscall ...
    {
      process_params(1);
      switch(dummy.prm[0])
      {
        case 0: process_params(2,1); break; //Print: 0 <t1> <v1>  
        case 1: process_params(2,1); break;//Input: 1 <t1> <v1>
      } cntr++;
    }
  }
  scan.location(0);
}

void k_low::serror(err_type e) //Output Error
{
  static char *er[]=
  {
    "Syntax Error",
    "Invalid Parameter Values!",
    "No Label Found!",
    "Duplicate, Label Already Exists!",
    "Incorrect Immediate Value Type!",
    "This Is Not A Valid Label!"
  };
  err_out << "Line " << scan.current_line() << ": " 
          << er[e] << endl;
  erred=true;
}

k_low::k_low(ostream &err):err_out(err),inst_num(0),scan(err){}

void k_low::assemble(const string &txt, ostream &out)
{
  erred=false; scan.setup(txt);
  snag_labels();
  while(scan.get_token())
  { 
    if(scan.token_type()==comment_tok){continue;}
    if(scan.token_type()!=str_tok){serror(serr); continue;}
    dummy.clear();
    if(scan.look_ahead().tok==":") //Labels
      {scan.get_token();} 
    else if(scan.token_text()=="expand") //expand <t1> <units>
    {
      dummy.opc=expand; process_params(2);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="deflate") //deflate <t1>
    {
      dummy.opc=deflate; process_params(2);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="set") //set <t1> <v1> <v2>
    {
      dummy.opc=set; process_params(3);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="seti") //seti <t1> <v1> <value>
    {
      dummy.opc=seti; process_params(2);
      get_immediate(dummy.prm[0]);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="cast") //cast <t1> <v1> <t2> <v2>
    {
      dummy.opc=cast; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="push") //push <t1> <v1>
    {
      dummy.opc=push; process_params(2);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="pushi") //pushi <t1> <value>
    {
      dummy.opc=pushi; process_params(1);
      get_immediate(dummy.prm[0]);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="pop") //pop <t1> <v1>
    {
      dummy.opc=pop; process_params(2);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="add") //add <t1> <v1> <v2> <v3>
    {
      dummy.opc=add; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="addi") //addi <t1> <v1> <v2> <value>
    {
      dummy.opc=addi; process_params(3);
      get_immediate(dummy.prm[0]);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="sub") //sub <t1> <v1> <v2> <v3>
    {
      dummy.opc=sub; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="subi") //subi <t1> <v1> <v2> <value>
    {
      dummy.opc=subi; process_params(3);
      get_immediate(dummy.prm[0]);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="mult") //mult <t1> <v1> <v2> <v3>
    {
      dummy.opc=mult; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="multi") //multi <t1> <v1> <v2> <value>
    {
      dummy.opc=multi; process_params(3);
      get_immediate(dummy.prm[0]);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="div") //div <t1> <v1> <v2> <v3>
    {
      dummy.opc=div; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="divi") //divi <t1> <v1> <v2> <value>
    {
      dummy.opc=divi; process_params(3);
      get_immediate(dummy.prm[0]);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="mod") //mod <t1> <v1> <v2> <v3>
    {
      dummy.opc=mod; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="modi") //modi <t1> <v1> <v2> <value>
    {
      dummy.opc=modi; process_params(3);
      get_immediate(dummy.prm[0]);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="and") //and <t1> <v1> <v2> <v3>
    {
      dummy.opc=land; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="or") //or <t1> <v1> <v2> <v3>
    {
      dummy.opc=lor; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="not") //not <t1> <v1> <v2>
    {
      dummy.opc=lnot; process_params(3);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="jump") //jump <label>
    {
      dummy.opc=jump; scan.get_token();
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[0]=label_value(scan.token_text());}
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="jeq") //jeq <t1> <v1> <v2> <label>
    {
      dummy.opc=jeq; process_params(3); scan.get_token();
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[3]=label_value(scan.token_text());}
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="jne") //jne <t1> <v1> <v2> <label>
    {
      dummy.opc=jne; process_params(3); scan.get_token();
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[3]=label_value(scan.token_text());}
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="jsub") //jsub <label>
    {
      dummy.opc=jsub; scan.get_token();
      if(scan.token_type()!=str_tok){serror(bad_lab); continue;}
      else{dummy.prm[0]=label_value(scan.token_text());}
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="ret") //ret
    {
      dummy.opc=ret;
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="slt") //slt <t1> <v1> <v2> <v3>
    {
      dummy.opc=slt; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="sgt") //sgt <t1> <v1> <v2> <v3>
    {
      dummy.opc=sgt; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="slte") //slte <t1> <v1> <v2> <v3>
    {
      dummy.opc=slte; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="sgte") //sgte <t1> <v1> <v2> <v3>
    {
      dummy.opc=sgte; process_params(4);
      dummy.dump(out); inst_num++;
    }
    else if(scan.token_text()=="seq") //seq <t1> <v1> <v2> <v3>
    {
      dummy.opc=seq; process_params(4);
      dummy.dump(out); inst_num++;
    } 
    else if(scan.token_text()=="end") //end
    { 
      dummy.opc=end;
      dummy.dump(out); inst_num++;
    } 
    else if(scan.token_text()=="syscall") //syscall ...
    {
      dummy.opc=syscall; process_params(1);
      switch(dummy.prm[0])
      {
        case 0: process_params(2,1); //Print: 0 <t1> <v1>  
                break; 
        case 1: process_params(2,1); //Input: 1 <t1> <v1>
                break;
      }
      dummy.dump(out); inst_num++;
    }
  }
}

const string &k_low::text(){return scan.text();}

bool k_low::errored(){return erred;} //Returns true if error occured
//--------------------------------------------------------------------------
}}
