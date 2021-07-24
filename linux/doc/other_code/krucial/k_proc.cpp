#include "k_vm.h"

namespace k { namespace krucial {
extern void k_sys_call(const instruction &i, k_memory &mem);
//Virtual Machine (Processor)-----------------------------------------------
void k_vm::trace(const u_long &ins)
{
  for(u_long i=0; i<ins; i++)
  {
    if(cur_inst==scr.end()){serror(no_end); break;}
    if((*cur_inst).opc==end){break;}
    big_switch(*(cur_inst)); cur_inst++;
  }
}

void k_vm::execute()
{
  while((*cur_inst).opc!=end)
  {
     if(cur_inst==scr.end()){serror(no_end); break;}
     big_switch(*cur_inst); cur_inst++;
  }
}
  
void k_vm::execute(script &scrpt)
{
  load_script(scrpt);
  execute();
}

void k_vm::big_switch(const instruction &i)
{
  switch(i.opc)
  {
    case expand: //expand <t1> <units>; (t1) 0=long, 1=count, 2=number, 3=string
      switch(i.prm[0])
      {
        case 0: mem.expand_int(i.prm[1]); break;
        case 1: mem.expand_cnt(i.prm[1]); break;
        case 2: mem.expand_num(i.prm[1]); break;
        case 3: mem.expand_str(i.prm[1]); break;
        default: serror(inval_prm);
      } break;
    case deflate: //deflate <t1> <units>; (t1) 0=long, 1=count, 2=number, 3=string
      switch(i.prm[0])
      {
        case 0: mem.deflate_int(i.prm[1]); break;
        case 1: mem.deflate_cnt(i.prm[1]); break;
        case 2: mem.deflate_num(i.prm[1]); break;
        case 3: mem.deflate_str(i.prm[1]); break;
        default: serror(inval_prm);
      } break;
    case set: //set <t1> <v1> <v2>; v1 and v2 is index location of variable 
      switch(i.prm[0])
      {
        case 0: mem.integer(i.prm[1])=mem.integer(i.prm[2]); break;
        case 1: mem.count(i.prm[1])=mem.count(i.prm[2]); break;
        case 2: mem.number(i.prm[1])=mem.number(i.prm[2]); break;
        case 3: mem.c_string(i.prm[1])=mem.c_string(i.prm[2]); break;
        default: serror(inval_prm);
      } break;
    case seti: //seti <t1> <v1> <value>; value=constant or string;
      switch(i.prm[0])
      {
        case 0: mem.integer(i.prm[1])=i.val.k_int; break;
        case 1: mem.count(i.prm[1])=i.val.k_cnt; break;
        case 2: mem.number(i.prm[1])=i.val.k_num; break;
        case 3: mem.c_string(i.prm[1])=i.val.k_str; break;
        default: serror(inval_prm);
      } break;
    case cast: //cast <t1> <v1> <t2> <v2>; cast var of type 2 to type 1
      switch(i.prm[0])
      {
        case 0:
          switch(i.prm[2])
          {
            case 0: mem.integer(i.prm[1])=
                    mem.integer(i.prm[3]); break;
            case 1: mem.integer(i.prm[1])=
                    (long)mem.count(i.prm[3]); break;
            case 2: mem.integer(i.prm[1])=
                    (long)mem.number(i.prm[3]); break;
            case 3: mem.integer(i.prm[1])=
                    (long)mem.c_string(i.prm[3])[0]; break;
            default: serror(inval_prm);
          } break;
        case 1: 
          switch(i.prm[2])
          {
            case 0: mem.count(i.prm[1])=
                    (u_long)mem.integer(i.prm[3]); break;
            case 1: mem.count(i.prm[1])=
                    mem.count(i.prm[3]); break;
            case 2: mem.count(i.prm[1])=
                    (u_long)mem.number(i.prm[3]); break;
            case 3: mem.count(i.prm[1])=
                    (u_long)mem.c_string(i.prm[3])[0]; break;
            default: serror(inval_prm);
          } break;
        case 2: 
          switch(i.prm[2])
          {
            case 0: mem.number(i.prm[1])=
                    (double)mem.integer(i.prm[3]); break;
            case 1: mem.number(i.prm[1])=
                    (double)mem.count(i.prm[3]); break;
            case 2: mem.number(i.prm[1])=
                    mem.number(i.prm[3]); break;
            case 3: mem.number(i.prm[1])=
                    (double)mem.c_string(i.prm[3])[0]; break;
            default: serror(inval_prm);
          } break;
        case 3:
          switch(i.prm[2])
          {
            case 0: mem.c_string(i.prm[1])=
                    (char)mem.integer(i.prm[3]); break;
            case 1: mem.c_string(i.prm[1])=
                    (char)mem.count(i.prm[3]); break;
            case 2: mem.c_string(i.prm[1])=
                    (char)mem.number(i.prm[3]); break;
            case 3: mem.c_string(i.prm[1])=
                    mem.c_string(i.prm[3]); break;
            default: serror(inval_prm);
          } break;
        default: serror(inval_prm);
      } break;
    case push: //push <t1> <v1>; pushes a var unto the stack
      switch(i.prm[0])
      {
        case 0: mem.push_int(mem.integer(i.prm[1])); break;
        case 1: mem.push_cnt(mem.count(i.prm[1])); break;
        case 2: mem.push_num(mem.number(i.prm[1])); break;
        case 3: mem.push_str(mem.c_string(i.prm[1])); break;
        default: serror(inval_prm);
      } break;
    case pushi: //pushi <t1> <value>; pushes a value onto the stack
      switch(i.prm[0]) 
      {
        case 0: mem.push_int(i.val.k_int); break;
        case 1: mem.push_cnt(i.val.k_cnt); break;
        case 2: mem.push_num(i.val.k_num); break;
        case 3: mem.push_str(i.val.k_str); break;
        default: serror(inval_prm);
      } break;
    case pop: //pop <t1> <v1>; pops a value of the stack
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=mem.pop_int(); break;
        case 1: mem.count(i.prm[1])=mem.pop_cnt(); break;
        case 2: mem.number(i.prm[1])=mem.pop_num(); break;
        case 3: mem.c_string(i.prm[1])=mem.pop_str(); break;
        default: serror(inval_prm);
      } break;
    case add: //add <t1> <v1> <v2> <v3>; v1=v2+v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])+mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])+mem.count(i.prm[3]); break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])+mem.number(i.prm[3]); break;
        case 3: mem.c_string(i.prm[1])=
                mem.c_string(i.prm[2])+mem.c_string(i.prm[3]); break;
        default: serror(inval_prm);
      } break;
    case addi: //addi <t1> <v1> <v2> <value>; v1=v2+value;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])+i.val.k_int; break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])+i.val.k_cnt; break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])+i.val.k_num; break;
        case 3: mem.c_string(i.prm[1])=
                mem.c_string(i.prm[2])+i.val.k_str; break;
        default: serror(inval_prm);
      } break;
    case sub: //sub <t1> <v1> <v2> <v3>; t only 0,1,2 not 3; v1=v2-v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])-mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])-mem.count(i.prm[3]); break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])-mem.number(i.prm[3]); break;
        case 3: serror(bad_typ_op); break;
        default: serror(inval_prm);
      } break;
    case subi: //subi <t1> <v1> <v2> <value>; t only 0,1,2 not 3; v1=v2-value;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])-i.val.k_int; break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])-i.val.k_cnt; break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])-i.val.k_num; break;
        case 3: serror(bad_typ_op); break;
        default: serror(inval_prm);
      } break;
    case mult: //mult <t1> <v1> <v2> <v3>; v1=v2*v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])*mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])*mem.count(i.prm[3]); break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])*mem.number(i.prm[3]); break;
        case 3: serror(bad_typ_op); break;
        default: serror(inval_prm);
      } break;
    case multi: //multi <t1> <v1> <v2> <value>; v1=v2*value;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])*i.val.k_int; break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])*i.val.k_cnt; break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])*i.val.k_num; break;
        case 3: serror(bad_typ_op); break;
        default: serror(inval_prm);
      } break;
    case div: //div <t1> <v1> <v2> <v3>; v1=v2/v3;
      switch(i.prm[0]) 
      {
        case 0: if(mem.integer(i.prm[3])==0)
                  {serror(div_zero); break;}
                mem.integer(i.prm[1])=
                mem.integer(i.prm[2])/mem.integer(i.prm[3]); break;
        case 1: if(mem.count(i.prm[3])==0)
                  {serror(div_zero); break;}
                mem.count(i.prm[1])=
                mem.count(i.prm[2])/mem.count(i.prm[3]); break;
        case 2: if(mem.number(i.prm[3])==0)
                  {serror(div_zero); break;}
                mem.number(i.prm[1])=
                mem.number(i.prm[2])/mem.number(i.prm[3]); break;
        case 3: serror(bad_typ_op); break;
        default: serror(inval_prm);
      } break;
    case divi: //divi <t1> <v1> <v2> <value>; v1=v2/value;
      switch(i.prm[0]) 
      {
        case 0: if(i.val.k_int==0)
                  {serror(div_zero); break;}
                mem.integer(i.prm[1])=
                mem.integer(i.prm[2])/i.val.k_int; break;
        case 1: if(i.val.k_cnt==0)
                  {serror(div_zero); break;}
                mem.count(i.prm[1])=
                mem.count(i.prm[2])/i.val.k_cnt; break;
        case 2: if(i.val.k_num==0)
                  {serror(div_zero); break;}
                mem.number(i.prm[1])=
                mem.number(i.prm[2])/i.val.k_num; break;
        case 3: serror(bad_typ_op); break;
        default: serror(inval_prm);
      } break;
    case mod: //mod <t1> <v1> <v2> <v3>; v1=v2%v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])%mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])%mem.count(i.prm[3]); break;
        case 2: serror(bad_typ_op); break;
        case 3: serror(bad_typ_op); break;
        default: serror(inval_prm);
      } break;
    case modi: //modi <t1> <v1> <v2> <value>; v1=v2%value;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])%i.val.k_int; break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])%i.val.k_cnt; break;
        case 2: serror(bad_typ_op); break;
        case 3: serror(bad_typ_op); break;
        default: serror(inval_prm);
      } break;
    case land: //and <t1> <v1> <v2> <v3>; v1=v2&&v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])&&mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])&&mem.count(i.prm[3]); break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])&&mem.number(i.prm[3]); break;
        case 3: if(mem.c_string(i.prm[2])=="t"&&
                   mem.c_string(i.prm[3])=="t")
                  mem.c_string(i.prm[1])="t";
                  else mem.c_string(i.prm[1])='f';
                break;
        default: serror(inval_prm);
      } break;
    case lor: //or <t1> <v1> <v2> <v3>; v1=v2||v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])||mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])||mem.count(i.prm[3]); break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])||mem.number(i.prm[3]); break;
        case 3: if(mem.c_string(i.prm[2])=="t"||
                   mem.c_string(i.prm[3])=="t")
                  mem.c_string(i.prm[1])='t';
                  else mem.c_string(i.prm[1])='f';             
                break;
        default: serror(inval_prm);
      } break;
    case lnot: //not <t1> <v1> <v2>; v1=!v2;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=!mem.integer(i.prm[2]); break;
        case 1: mem.count(i.prm[1])=!mem.count(i.prm[2]); break;
        case 2: mem.number(i.prm[1])=!mem.number(i.prm[2]); break;
        case 3: mem.c_string(i.prm[1])='f';
                if(mem.c_string(i.prm[2])=="f")
                  mem.c_string(i.prm[1])="t";
                break;
        default: serror(inval_prm);
      } break;
    case jump: //jump <loc>; loc is the instruction to jump too
      cur_inst=scr.begin()+i.prm[0]-1; break;
    case jeq: //jeq <t1> <v1> <v2> <loc>; jump if v1 equal v2
      switch(i.prm[0]) 
      {
        case 0: if(mem.integer(i.prm[1])==mem.integer(i.prm[2])) 
                  cur_inst=scr.begin()+i.prm[3]-1; break;
        case 1: if(mem.count(i.prm[1])==mem.count(i.prm[2])) 
                  cur_inst=scr.begin()+i.prm[3]-1; break;
        case 2: if(mem.number(i.prm[1])==mem.number(i.prm[2])) 
                  cur_inst=scr.begin()+i.prm[3]-1; break;
        case 3: if(mem.c_string(i.prm[1])==mem.c_string(i.prm[2])) 
                  cur_inst=scr.begin()+i.prm[3]-1; break;
        default: serror(inval_prm);
      } break;
    case jne: //jne <t1> <v1> <v2> <loc>; jump if v1 not equal v2
      switch(i.prm[0]) 
      {
        case 0: if(mem.integer(i.prm[1])!=mem.integer(i.prm[2])) 
                  cur_inst=scr.begin()+i.prm[3]-1; break;
        case 1: if(mem.count(i.prm[1])!=mem.count(i.prm[2])) 
                  cur_inst=scr.begin()+i.prm[3]-1; break;
        case 2: if(mem.number(i.prm[1])!=mem.number(i.prm[2])) 
                  cur_inst=scr.begin()+i.prm[3]-1; break;
        case 3: if(mem.c_string(i.prm[1])!=mem.c_string(i.prm[2])) 
                  cur_inst=scr.begin()+i.prm[3]-1; break;
        default: serror(inval_prm);
      } break;
    case jsub: //jsub <loc>; jump + save location for return
      mem.push_ra(cur_inst); cur_inst=scr.begin()+i.prm[0]-1; break;
    case ret: //ret; returns from a subroutine
      cur_inst=mem.pop_ra(); break;
    case slt: //slt <t1> <v1> <v2> <v3>; v1=v2<v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])<mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])<mem.count(i.prm[3]); break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])<mem.number(i.prm[3]); break;
        case 3: mem.c_string(i.prm[1])='f';
                if(mem.c_string(i.prm[2])<mem.c_string(i.prm[3]))
                  mem.c_string(i.prm[1])='t';
                break;
        default: serror(inval_prm);
      } break;
    case sgt: //sgt <t1> <v1> <v2> <v3>; v1=v2>v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])>mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])>mem.count(i.prm[3]); break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])>mem.number(i.prm[3]); break;
        case 3: mem.c_string(i.prm[1])='f';
                if(mem.c_string(i.prm[2])>mem.c_string(i.prm[3]))
                  mem.c_string(i.prm[1])='t';
                break;
        default: serror(inval_prm);
      } break;
    case slte: //slte <t1> <v1> <v2> <v3>; v1=v2<=v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])<=mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])<=mem.count(i.prm[3]); break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])<=mem.number(i.prm[3]); break;
        case 3: mem.c_string(i.prm[1])='f';
                if(mem.c_string(i.prm[2])<=mem.c_string(i.prm[3]))
                  mem.c_string(i.prm[1])='t';
                break;
        default: serror(inval_prm);
      } break;
    case sgte: //sgte <t1> <v1> <v2> <v3>; v1=v2>=v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])>=mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])>=mem.count(i.prm[3]); break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])>=mem.number(i.prm[3]); break;
        case 3: mem.c_string(i.prm[1])='f';
                if(mem.c_string(i.prm[2])>=mem.c_string(i.prm[3]))
                  mem.c_string(i.prm[1])='t';
                break;
        default: serror(inval_prm);
      } break;
    case seq: //seq <t1> <v1> <v2> <v3>; v1=v2==v3;
      switch(i.prm[0]) 
      {
        case 0: mem.integer(i.prm[1])=
                mem.integer(i.prm[2])==mem.integer(i.prm[3]); break;
        case 1: mem.count(i.prm[1])=
                mem.count(i.prm[2])==mem.count(i.prm[3]); break;
        case 2: mem.number(i.prm[1])=
                mem.number(i.prm[2])==mem.number(i.prm[3]); break;
        case 3: mem.c_string(i.prm[1])='f';
                if(mem.c_string(i.prm[2])==mem.c_string(i.prm[3]))
                  mem.c_string(i.prm[1])='t';
                break;
        default: serror(inval_prm);
      } break;
    case syscall: //syscall ...; system functions built into the vm
      k_sys_call(i,mem); break;
    default:
      serror(inval_op);
  }
}
//-----------------------------------------------------------------------END
}}
