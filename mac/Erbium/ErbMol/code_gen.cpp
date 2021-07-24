
/*---------------------------------------------------------------------------
Molokai - the erbium high level language

Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "molokai.h"
#include <sstream>

namespace molokai {
//---------------------------------------------------------------------------  
void print_prms(erb_inst &ins, u_long n, ostream &s)
{
  for(u_long i=0; i<n; i++)
  {
    s << ins.prm[i] << " "; 
  }
}

void compiler_state::code_gen(erb_op op, u_long p1, u_long p2, 
     u_long p3, u_long p4, u_long c_imm, double v_imm, string targ) 
{
  //Set up instruction to hold data
  erb_inst i;
  i.op=op; 
  i.prm[0]=p1;
  i.prm[1]=p2;
  i.prm[2]=p3;
  i.prm[3]=p4;
  i.c_imm=c_imm;
  i.v_imm=v_imm;
  
  //Create string to hold this instruction output
  ostringstream out;
  
  switch(i.op)
  {
    case expand: //expand <type> <units>; 0=count, 1=value, 2=construct
      out << "expand ";
      print_prms(i,2,out);
      break;
    case deflate: //deflate <type> <units>; 0=count, 1=value, 2=construct
      out << "deflate ";
      print_prms(i,2,out);
      break;
    case pload: //pload <prim_id> <dest>; place primitive into dest
      out << "pload ";
      print_prms(i,2,out);
      break;
    case spit: //spit <type> <source> prints mem loc contents to screen 
      out << "spit ";
      print_prms(i,2,out);
      break;
      case spitmap: //spitmap <source>
          out << "spitmap ";
          print_prms(i,1,out);
          break;
      case render: //render <source>
          out << "render ";
          print_prms(i,1,out);
          break;
      case cuni: //cuni <dest> <source1> <source2>
      out << "cuni ";
      print_prms(i,3,out);
      break;
    case cinter: //cinter <dest> <source1> <source2>
      out << "cinter ";
      print_prms(i,3,out);
      break;
    case csub: //csub <dest> <source1> <source2>
      out << "csub ";
      print_prms(i,3,out);
      break;
    case crotx: //crotx <dest> <source1> <source2>
      out << "crotx ";
      print_prms(i,3,out);
      break;
    case croty: //croty <dest> <source1> <source2>
      out << "croty ";
      print_prms(i,3,out);
      break;
    case crotz: //crotz <dest> <source1> <source2>
      out << "crotz ";
      print_prms(i,3,out);
      break;
    case cmovex: //cmovex <dest> <source1> <source2>
      out << "cmovex ";
      print_prms(i,3,out);
      break;
    case cmovey: //cmovey <dest> <source1> <source2>
      out << "cmovey ";
      print_prms(i,3,out);
      break;
    case cmovez: //cmovez <dest> <source1> <source2>
      out << "cmovez ";
      print_prms(i,3,out);
      break;
    case cscalex: //cscalex <dest> <source1> <source2>
      out << "cscalex ";
      print_prms(i,3,out);
      break;
    case cscaley: //cscaley <dest> <source1> <source2>
      out << "cscaley ";
      print_prms(i,3,out);
      break;
    case cscalez: //cscalez <dest> <source1> <source2>
      out << "cscalez ";
      print_prms(i,3,out);
      break;
    case csetx: //csetx <dest> <source1> <source2>
      out << "csetx ";
      print_prms(i,3,out);
      break;
    case csety: //csety <dest> <source1> <source2>
      out << "csety ";
      print_prms(i,3,out);
      break;
    case csetz: //csetz <dest> <source1> <source2>
      out << "csetz ";
      print_prms(i,3,out);
      break;
    case csetw: //csetw <dest> <source1> <source2>
      out << "csetw ";
      print_prms(i,3,out);
      break;
    case csetl: //csetl <dest> <source1> <source2>
      out << "csetl ";
      print_prms(i,3,out);
      break;
    case cseth: //cseth <dest> <source1> <source2>
      out << "cseth ";
      print_prms(i,3,out);
      break;
    case cgetx: //cgetx <dest> <source> #dest is type value, source is construct
      out << "cgetx ";
      print_prms(i,2,out);
      break;
    case cgety: //cgety <dest> <source>
      out << "cgety ";
      print_prms(i,2,out);
      break;
    case cgetz: //cgetz <dest> <source>
      out << "cgetz ";
      print_prms(i,2,out);
      break;
    case cgetw: //cgetw <dest> <source>
      out << "cgetw ";
      print_prms(i,2,out);
      break;
    case cgetl: //cgetl <dest> <source>
      out << "cgetl ";
      print_prms(i,2,out);
      break;
    case cgeth: //cgeth <dest> <source>
      out << "cgeth ";
      print_prms(i,2,out);
      break;
    case ccol: //ccol <type> <dest> <source1> <source2>
      out << "ccol ";
      print_prms(i,4,out);
      break;
    case machine::alloc: //alloc <type> <dest> <units_src>
      out << "alloc ";
      print_prms(i,3,out);
      break;
    case machine::load: //load <type> <dest> <index> <source>
      out << "load ";
      print_prms(i,4,out);
      break;
    case machine::store: //store <type> <source> <index> <dest>
      out << "store ";
      print_prms(i,4,out);
      break;
    case machine::push: //push <type> <source>
      out << "push ";
      print_prms(i,3,out);
      break;
    case machine::pop: //pop <type> <dest>
      out << "pop ";
      print_prms(i,3,out);
      break;
    case machine::copy: //copy <type> <dest> <source>
      out << "copy ";
      print_prms(i,3,out);
      break;
    case machine::cast: //cast <type> <dest> <source>
      out << "cast ";
      print_prms(i,3,out);
      break;
    case machine::copydown: //copydown <type> <dest> 
      out << "copydown ";
      print_prms(i,2,out);
      break;
    case seti: //seti <type> <dest>; these instructions only supports type 0, and 1
      out << "seti ";
      print_prms(i,2,out);
      switch(i.prm[0])
      {
        case 0: out << i.c_imm << " "; break;
        case 1: out << i.v_imm << " "; break;
        default: break;
      }
      break;
    case add: //add <type> <dest> <source1> <source2>
      out << "add ";
      print_prms(i,4,out);
      break;
    case addi:
      out << "addi ";
      print_prms(i,3,out);
      switch(i.prm[0])
      {
        case 0: out << i.c_imm << " "; break;
        case 1: out << i.v_imm << " "; break;
        default: break;
      }
      break;
    case sub: //sub <type> <dest> <source1> <source2>
      out << "sub ";
      print_prms(i,4,out);
      break;
    case subi: //subi <type> <dest> <source> <imm>
      out << "subi ";
      print_prms(i,3,out);
      switch(i.prm[0])
      {
        case 0: out << i.c_imm << " "; break;
        case 1: out << i.v_imm << " "; break;
        default: break;
      }
      break;
    case mult: //mult <type> <dest> <source1> <source2>
      out << "mult ";
      print_prms(i,4,out);
      break;
    case multi: //multi <type> <dest> <source> <imm>
      out << "multi ";
      print_prms(i,3,out);
      switch(i.prm[0])
      {
        case 0: out << i.c_imm << " "; break;
        case 1: out << i.v_imm << " "; break;
        default: break;
      }
      break;
    case machine::div: //div <type> <dest> <source1> <source2>
      out << "div ";
      print_prms(i,4,out);
      break;
    case divi: //divi <type> <dest> <source> <imm>
      out << "divi ";
      print_prms(i,3,out);
      switch(i.prm[0])
      {
        case 0: out << i.c_imm << " "; break;
        case 1: out << i.v_imm << " "; break;
        default: break;
      }
      break;
    case mod: //mod <type> <dest> <source1> <source2>
      out << "mod ";
      print_prms(i,4,out);
      break;
    case modi: //modi <type> <dest> <source> <imm>
      out << "modi ";
      print_prms(i,3,out);
      switch(i.prm[0])
      {
        case 0: out << i.c_imm << " "; break;
        case 1: out << i.v_imm << " "; break;
        default: break;
      }
      break;
    case seq: //seq <type> <dest> <source1> <source2> 
      out << "seq ";
      print_prms(i,4,out);
      break;
    case sne: //sne <type> <dest> <source1> <source2>
      out << "sne ";
      print_prms(i,4,out);
      break;
    case slt: //slt <type> <dest> <source1> <source2>
      out << "slt ";
      print_prms(i,4,out);
      break;
    case sgt: //sgt <type> <dest> <source1> <source2>
      out << "sgt ";
      print_prms(i,4,out);
      break;
    case slte: //slte <type> <dest> <source1> <source2>
      out << "slte ";
      print_prms(i,4,out);
      break;
    case sgte: //sgte <type> <dest> <source1> <source2>
      out << "sgte ";
      print_prms(i,4,out);
      break;
    case land: //land <type> <dest> <source1> <source2>
      out << "land ";
      print_prms(i,4,out);
      break;
    case lor: //lor <type> <dest> <source1> <source2>
      out << "lor ";
      print_prms(i,4,out);
      break;
    case lnot: //lnot <type> <dest> <source1>
      out << "lnot ";
      print_prms(i,3,out);
      break;
    case seed:
      out << "seed ";
      print_prms(i,1,out);
      break;
    case timeseed:
      out << "timeseed ";
      break;
    case gen: //gen <type> <dest> <min> <max>
      out << "gen ";
      print_prms(i,4,out);
      break;
    case jump: //jump <dest>
      out << "jump " << targ << " ";
      break;
    case jchance: //jchance <source> <dest>
      out << "jchance ";
      print_prms(i,1,out);
      out << targ << " ";
      break;
    case jeq: //jeq <type> <source1> <source2> <dest>
      out << "jeq ";
      print_prms(i,3,out);
      out << targ << " ";
      break;
    case jne: //jne <type> <source1> <source2> <dest>
      out << "jne ";
      print_prms(i,3,out);
      out << targ << " ";
      break;
    case jlt: //jlt <type> <source1> <source2> <dest>
      out << "jlt ";
      print_prms(i,3,out);
      out << targ << " ";
      break;
    case jgt: //jgt <type> <source1> <source2> <dest>
      out << "jgt ";
      print_prms(i,3,out);
      out << targ << " ";
      break;
    case jlte: //jlte <type> <source1> <source2> <dest>
      out << "jlte ";
      print_prms(i,3,out);
      out << targ << " ";
      break;
    case jgte: //jgte <type> <source1> <source2> <dest>
      out << "jgte ";
      print_prms(i,3,out);
      out << targ << " ";
      break;
    case jsub: //jsum <dest>
      out << "jsub ";
      out << targ << " ";
      break;
    case ret:
      out << "ret ";
      break;
    case end:
      out << "end ";
      break;
  }

  //now add it to the list of instructions
  string s=out.str();
  src_out.add(s);
}
//------------------------------------------------------------------------END
}
