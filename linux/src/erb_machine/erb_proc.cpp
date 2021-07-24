/*---------------------------------------------------------------------------
Erbium Machine (Processing Unit)

Implementation of the abstract virtual machine
Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "erb_machine.h"
#include <cstdlib>

namespace erb { namespace machine {
//Erbium Machine-------------------------------------------------------------
void erb_machine::trace(const u_long &ins) //located in erb_proc.cpp
{
  for(u_long i=0; i<ins; i++)
  {
    if((*prog.cur).op==end){break;}
    if(prog.at_end()){serror(no_end); break;}
    big_switch(*prog.cur); prog.cur++; 
  }
}

void erb_machine::execute() //located in erb_proc.cpp
{
  while((*prog.cur).op!=end)
  {
    if(prog.at_end()){serror(no_end); break;}
    big_switch(*prog.cur); prog.cur++;
  }
}

void erb_machine::big_switch(const erb_inst &i) //located in erb_proc.cpp
{
  switch(i.op)
  {
    case expand: //expand <type> <units>; 0=count, 1=value, 2=construct
      switch(i.prm[0])
      {
        case 0: mem.mem.top().erb_cnt.resize(mem.mem.top().erb_cnt.size()+i.prm[1]); break;
        case 1: mem.mem.top().erb_val.resize(mem.mem.top().erb_val.size()+i.prm[1]); break;
        case 2: mem.mem.top().erb_elm.resize(mem.mem.top().erb_elm.size()+i.prm[1]); break;
        default: serror(bad_typ_op);
      } break;
    case deflate: //deflate <type> <units>; 0=count, 1=value, 2=construct
      switch(i.prm[0])
      {
        case 0: mem.mem.top().erb_cnt.resize(mem.mem.top().erb_cnt.size()-i.prm[1]); break;
        case 1: mem.mem.top().erb_val.resize(mem.mem.top().erb_val.size()-i.prm[1]); break;
        case 2: mem.mem.top().erb_elm.resize(mem.mem.top().erb_val.size()-i.prm[1]); break;
        default: serror(bad_typ_op);
      } break;
      break;
    case pload: //pload <prim_id> <dest>; place primitive into dest
      {
        if(i.prm[0]>=prim.size()||i.prm[1]>=mem.mem.top().erb_elm.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[1]]=(construct)prim[i.prm[0]].get();
      } break;
    case spit: //spit <type> <source> prints mem.mem.top().loc contents to screen 
      switch(i.prm[0])
      {
        case 0:
          if(i.prm[1]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          cout << mem.mem.top().erb_cnt[i.prm[1]] << endl;
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          cout << mem.mem.top().erb_val[i.prm[1]] << endl;
          break;
        case 2:
          if(i.prm[1]>=mem.mem.top().erb_elm.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_elm[i.prm[1]].ascii_out(cout);
          break;
        default: serror(bad_typ_op);
      } break;
    case spitmap: //spitmap <source> #only with constructs, prints quake map format
      if(i.prm[0]>=mem.mem.top().erb_elm.size())
        {serror(inval_prm); break;}
      mem.mem.top().erb_elm[i.prm[0]].map_out(cout);
      break;
    case cuni: //cuni <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_elm.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]]+mem.mem.top().erb_elm[i.prm[2]];
      } break;
    case cinter: //cinter <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_elm.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]]*mem.mem.top().erb_elm[i.prm[2]];
      } break;
    case csub: //csub <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_elm.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]]-mem.mem.top().erb_elm[i.prm[2]];
      } break;
      break;
    case crotx: //crotx <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        matrix m; m.rotate_x(mem.mem.top().erb_val[i.prm[2]]);
        mem.mem.top().erb_elm[i.prm[0]].apply_self(m);
      } break;
    case croty: //croty <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        matrix m; m.rotate_y(mem.mem.top().erb_val[i.prm[2]]);
        mem.mem.top().erb_elm[i.prm[0]].apply_self(m);
      } break;
    case crotz: //crotz <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        matrix m; m.rotate_z(mem.mem.top().erb_val[i.prm[2]]);
        mem.mem.top().erb_elm[i.prm[0]].apply_self(m);
      } break;
    case cmovex: //cmovex <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        matrix m; m.translate(mem.mem.top().erb_val[i.prm[2]], 0, 0);
        mem.mem.top().erb_elm[i.prm[0]].apply(m);
      } break;
    case cmovey: //cmovey <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        matrix m; m.translate(0, mem.mem.top().erb_val[i.prm[2]], 0);
        mem.mem.top().erb_elm[i.prm[0]].apply(m);
      } break;
    case cmovez: //cmovez <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        matrix m; m.translate(0, 0, mem.mem.top().erb_val[i.prm[2]]);
        mem.mem.top().erb_elm[i.prm[0]].apply(m);
      } break;
    case cscalex: //cscalex <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        matrix m; m.scale(mem.mem.top().erb_val[i.prm[2]], 1, 1);
        mem.mem.top().erb_elm[i.prm[0]].apply_self(m);
      } break;
    case cscaley: //cscaley <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        matrix m; m.scale(1, mem.mem.top().erb_val[i.prm[2]], 1);
        mem.mem.top().erb_elm[i.prm[0]].apply_self(m);
      } break;
    case cscalez: //cscalez <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        matrix m; m.scale(1, 1, mem.mem.top().erb_val[i.prm[2]]);
        mem.mem.top().erb_elm[i.prm[0]].apply_self(m);
      } break;
    case csetx: //csetx <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        if(!mem.mem.top().erb_elm[i.prm[0]].bound.valid){mem.mem.top().erb_elm[i.prm[0]].build_box();}
        double offset=mem.mem.top().erb_elm[i.prm[0]].bound.orig.x;
        matrix m; m.translate(mem.mem.top().erb_val[i.prm[2]]-offset, 0, 0);
        mem.mem.top().erb_elm[i.prm[0]].apply(m);
      } break;
    case csety: //csety <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        if(!mem.mem.top().erb_elm[i.prm[0]].bound.valid){mem.mem.top().erb_elm[i.prm[0]].build_box();}
        double offset=mem.mem.top().erb_elm[i.prm[0]].bound.orig.y;
        matrix m; m.translate(0, mem.mem.top().erb_val[i.prm[2]]-offset, 0);
        mem.mem.top().erb_elm[i.prm[0]].apply(m);
      } break;
    case csetz: //csetz <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        if(!mem.mem.top().erb_elm[i.prm[0]].bound.valid){mem.mem.top().erb_elm[i.prm[0]].build_box();}
        double offset=mem.mem.top().erb_elm[i.prm[0]].bound.orig.z;
        if(is_zero(offset)) offset=1;
        matrix m; m.translate(0, 0, mem.mem.top().erb_val[i.prm[2]]-offset);
        mem.mem.top().erb_elm[i.prm[0]].apply(m);
      } break;
    case csetw: //csetw <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        if(!mem.mem.top().erb_elm[i.prm[0]].bound.valid){mem.mem.top().erb_elm[i.prm[0]].build_box();}
        double offset=mem.mem.top().erb_elm[i.prm[0]].bound.dim.x;
        double orig=mem.mem.top().erb_elm[i.prm[0]].bound.orig.x;
        if(is_zero(offset)) offset=1;
        matrix m; m.scale(mem.mem.top().erb_val[i.prm[2]]/offset, 1, 1);
        mem.mem.top().erb_elm[i.prm[0]].apply_self(m);
        //place at original orig
        mem.mem.top().erb_elm[i.prm[0]].build_box();
        offset=mem.mem.top().erb_elm[i.prm[0]].bound.orig.x;
        m.make_identity(); m.translate(orig-offset, 0, 0);
        mem.mem.top().erb_elm[i.prm[0]].apply(m);
      } break;
    case csetl: //csetl <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        if(!mem.mem.top().erb_elm[i.prm[0]].bound.valid){mem.mem.top().erb_elm[i.prm[0]].build_box();}
        double offset=mem.mem.top().erb_elm[i.prm[0]].bound.dim.y;
        double orig=mem.mem.top().erb_elm[i.prm[0]].bound.orig.y;
        if(is_zero(offset)) offset=1;
        matrix m; m.scale(1, mem.mem.top().erb_val[i.prm[2]]/offset, 1);
        mem.mem.top().erb_elm[i.prm[0]].apply_self(m);
        //place at original orig
        mem.mem.top().erb_elm[i.prm[0]].build_box();
        offset=mem.mem.top().erb_elm[i.prm[0]].bound.orig.y;
        m.make_identity(); m.translate(0, orig-offset, 0);
        mem.mem.top().erb_elm[i.prm[0]].apply(m);
      } break;
    case cseth: //cseth <dest> <source1> <source2>
      {
        if(i.prm[0]>=mem.mem.top().erb_elm.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size()||
           i.prm[2]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        mem.mem.top().erb_elm[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]];
        if(!mem.mem.top().erb_elm[i.prm[0]].bound.valid){mem.mem.top().erb_elm[i.prm[0]].build_box();}
        double offset=mem.mem.top().erb_elm[i.prm[0]].bound.dim.z;
        double orig=mem.mem.top().erb_elm[i.prm[0]].bound.orig.z;
        matrix m; m.scale(1, 1, mem.mem.top().erb_val[i.prm[2]]/offset);
        mem.mem.top().erb_elm[i.prm[0]].apply_self(m);
        //place at original orig
        mem.mem.top().erb_elm[i.prm[0]].build_box();
        offset=mem.mem.top().erb_elm[i.prm[0]].bound.orig.z;
        m.make_identity(); m.translate(0, 0, orig-offset);
        mem.mem.top().erb_elm[i.prm[0]].apply(m);
      } break;
    case cgetx: //cgetx <dest> <source> #dest is type value, source is construct
      {
        if(i.prm[0]>=mem.mem.top().erb_val.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size())
          {serror(inval_prm); break;}
        if(!mem.mem.top().erb_elm[i.prm[1]].bound.valid){mem.mem.top().erb_elm[i.prm[1]].build_box();}
        mem.mem.top().erb_val[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]].bound.orig.x;
      } break;
    case cgety: //cgety <dest> <source>
      {
        if(i.prm[0]>=mem.mem.top().erb_val.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size())
          {serror(inval_prm); break;}
        if(!mem.mem.top().erb_elm[i.prm[1]].bound.valid){mem.mem.top().erb_elm[i.prm[1]].build_box();}
        mem.mem.top().erb_val[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]].bound.orig.y;
      } break;
    case cgetz: //cgetz <dest> <source>
      {
        if(i.prm[0]>=mem.mem.top().erb_val.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size())
          {serror(inval_prm); break;}
        if(!mem.mem.top().erb_elm[i.prm[1]].bound.valid){mem.mem.top().erb_elm[i.prm[1]].build_box();}
        mem.mem.top().erb_val[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]].bound.orig.z;
      } break;
    case cgetw: //cgetw <dest> <source>
      {
        if(i.prm[0]>=mem.mem.top().erb_val.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size())
          {serror(inval_prm); break;}
        if(!mem.mem.top().erb_elm[i.prm[1]].bound.valid){mem.mem.top().erb_elm[i.prm[1]].build_box();}
        mem.mem.top().erb_val[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]].bound.dim.x;
      } break;
    case cgetl: //cgetl <dest> <source>
      {
        if(i.prm[0]>=mem.mem.top().erb_val.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size())
          {serror(inval_prm); break;}
        if(!mem.mem.top().erb_elm[i.prm[1]].bound.valid){mem.mem.top().erb_elm[i.prm[1]].build_box();}
        mem.mem.top().erb_val[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]].bound.dim.y;
      } break;
    case cgeth: //cgeth <dest> <source>
      {
        if(i.prm[0]>=mem.mem.top().erb_val.size()||
           i.prm[1]>=mem.mem.top().erb_elm.size())
          {serror(inval_prm); break;}
        if(!mem.mem.top().erb_elm[i.prm[1]].bound.valid){mem.mem.top().erb_elm[i.prm[1]].build_box();}
        mem.mem.top().erb_val[i.prm[0]]=mem.mem.top().erb_elm[i.prm[1]].bound.dim.z;
      } break;
    case ccol: //ccol <type> <dest> <source1> <source2> #construct collision detection
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_elm.size()||
             i.prm[3]>=mem.mem.top().erb_elm.size())
            {serror(inval_prm); break;}
          if(!mem.mem.top().erb_elm[i.prm[2]].bound.valid)
            {mem.mem.top().erb_elm[i.prm[2]].build_box();}
          if(is_zero(mem.mem.top().erb_elm[i.prm[2]].bound.dim.x)||
             is_zero(mem.mem.top().erb_elm[i.prm[2]].bound.dim.y)||
             is_zero(mem.mem.top().erb_elm[i.prm[2]].bound.dim.z))
            {mem.mem.top().erb_cnt[i.prm[1]]=0; break;}
          if(!mem.mem.top().erb_elm[i.prm[3]].bound.valid)
            {mem.mem.top().erb_elm[i.prm[3]].build_box();}
          if(is_zero(mem.mem.top().erb_elm[i.prm[3]].bound.dim.x)||
             is_zero(mem.mem.top().erb_elm[i.prm[3]].bound.dim.y)||
             is_zero(mem.mem.top().erb_elm[i.prm[3]].bound.dim.z)) 
            {mem.mem.top().erb_cnt[i.prm[1]]=0; break;}
              collide(mem.mem.top().erb_elm[i.prm[2]].bound,mem.mem.top().erb_elm[i.prm[3]].bound);
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_elm.size()||
             i.prm[3]>=mem.mem.top().erb_elm.size())
            {serror(inval_prm); break;}
          if(!mem.mem.top().erb_elm[i.prm[2]].bound.valid)
            {mem.mem.top().erb_elm[i.prm[2]].build_box();}
          if(is_zero(mem.mem.top().erb_elm[i.prm[2]].bound.dim.x)||
             is_zero(mem.mem.top().erb_elm[i.prm[2]].bound.dim.y)||
             is_zero(mem.mem.top().erb_elm[i.prm[2]].bound.dim.z))
            {mem.mem.top().erb_val[i.prm[1]]=0; break;}
          if(!mem.mem.top().erb_elm[i.prm[3]].bound.valid)
            {mem.mem.top().erb_elm[i.prm[3]].build_box();}
          if(is_zero(mem.mem.top().erb_elm[i.prm[3]].bound.dim.x)||
             is_zero(mem.mem.top().erb_elm[i.prm[3]].bound.dim.y)||
             is_zero(mem.mem.top().erb_elm[i.prm[3]].bound.dim.z)) 
            {mem.mem.top().erb_val[i.prm[1]]=0; break;}
          mem.mem.top().erb_val[i.prm[1]]=
            collide(mem.mem.top().erb_elm[i.prm[2]].bound,mem.mem.top().erb_elm[i.prm[3]].bound);
          break;
        default: serror(bad_typ_op);
      } break;
    case alloc: //alloc <type> <dest> <units_src>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().cnt_mem.add(bag<long>());
          mem.mem.top().cnt_mem.top().resize(mem.mem.top().erb_cnt[i.prm[2]]);
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().cnt_mem.size()-1;
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().val_mem.add(bag<double>());
          mem.mem.top().val_mem.top().resize(mem.mem.top().erb_cnt[i.prm[2]]);
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().val_mem.size()-1;
          break;
        case 2:
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().elm_mem.add(bag<construct>());
          mem.mem.top().elm_mem.top().resize(mem.mem.top().erb_cnt[i.prm[2]]);
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().elm_mem.size()-1;
          break;
        default: serror(bad_typ_op);
      } break;
    case load: //load <type> <dest> <index> <source>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=
          mem.mem.top().cnt_mem[mem.mem.top().erb_cnt[i.prm[2]]]
          [mem.mem.top().erb_cnt[i.prm[3]]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=
          mem.mem.top().val_mem[mem.mem.top().erb_cnt[i.prm[2]]]
          [mem.mem.top().erb_cnt[i.prm[3]]];
          break;
        case 2:
          if(i.prm[1]>=mem.mem.top().erb_elm.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_elm[i.prm[1]]=
          mem.mem.top().elm_mem[mem.mem.top().erb_cnt[i.prm[2]]]
          [mem.mem.top().erb_cnt[i.prm[3]]];
          break;
        default: serror(bad_typ_op);
      } break;
    case store: //store <type> <source> <index> <dest>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().cnt_mem[mem.mem.top().erb_cnt[i.prm[2]]]
          [mem.mem.top().erb_cnt[i.prm[3]]]=
          mem.mem.top().erb_cnt[i.prm[1]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().val_mem[mem.mem.top().erb_cnt[i.prm[2]]]
          [mem.mem.top().erb_cnt[i.prm[3]]]=
          mem.mem.top().erb_val[i.prm[1]];
          break;
        case 2:
          if(i.prm[1]>=mem.mem.top().erb_elm.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().elm_mem[mem.mem.top().erb_cnt[i.prm[2]]]
          [mem.mem.top().erb_cnt[i.prm[3]]]=
          mem.mem.top().erb_elm[i.prm[1]];
          break;
        default: serror(bad_typ_op);
      } break;
    case push: //push <stack> <type> <source>
      if(i.prm[0]!=0&&i.prm[0]!=1)
        {serror(inval_prm); break;}
      switch(i.prm[1])
      {
        case 0: 
          if(i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.cnt_stk[i.prm[0]].add(mem.mem.top().erb_cnt[i.prm[2]]);
          break;
        case 1:
          if(i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.val_stk[i.prm[0]].add(mem.mem.top().erb_val[i.prm[2]]);
          break;
        case 2:
          if(i.prm[2]>=mem.mem.top().erb_elm.size())
            {serror(inval_prm); break;}
          mem.elm_stk[i.prm[0]].add(mem.mem.top().erb_elm[i.prm[2]]);
          break;
        default: serror(bad_typ_op);
      } break;
    case pop: //pop <stack> <type> <dest>
      if(i.prm[0]!=0&&i.prm[0]!=1)
        {serror(inval_prm); break;}
      switch(i.prm[1])
      {
        case 0: 
          if(i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[2]]=mem.cnt_stk[i.prm[0]].top();
          mem.cnt_stk[i.prm[0]].pop();
          break;
        case 1:
          if(i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[2]]=mem.val_stk[i.prm[0]].top();
          mem.val_stk[i.prm[0]].pop();
          break;
        case 2:
          if(i.prm[2]>=mem.mem.top().erb_elm.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_elm[i.prm[2]]=mem.elm_stk[i.prm[0]].top();
          mem.elm_stk[i.prm[0]].pop();
          break;
        default: serror(bad_typ_op);
      } break;
    case copy: //copy <type> <dest> <source>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]];
          break;
        case 2:
          if(i.prm[1]>=mem.mem.top().erb_elm.size()||
             i.prm[2]>=mem.mem.top().erb_elm.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_elm[i.prm[1]]=mem.mem.top().erb_elm[i.prm[2]];
          break;
        default: serror(bad_typ_op);
      } break;
    case cast: //cast <type> <dest> <source>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=(u_long)mem.mem.top().erb_val[i.prm[2]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]];
          break;
        default: serror(bad_typ_op);
      } break;
    case copydown: //copydown <type> <dest>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[1]>=mem.mem[mem.mem.size()-2].erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem[mem.mem.size()-2].erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[1]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[1]>=mem.mem[mem.mem.size()-2].erb_val.size())
            {serror(inval_prm); break;}
          mem.mem[mem.mem.size()-2].erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[1]];
          break;
        case 2:
          if(i.prm[1]>=mem.mem.top().erb_elm.size()||
             i.prm[1]>=mem.mem[mem.mem.size()-2].erb_elm.size())
            {serror(inval_prm); break;}
          mem.mem[mem.mem.size()-2].erb_elm[i.prm[1]]=mem.mem.top().erb_elm[i.prm[1]];
          break;
        default: serror(bad_typ_op);
      } break;
    case seti: //seti <type> <dest>; these instructions only supports type 0, and 1
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=i.c_imm;
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=i.v_imm;
          break;
        default: serror(bad_typ_op);
      } break;
    case add: //add <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]+mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]+mem.mem.top().erb_val[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case addi:
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]+i.c_imm;
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]+i.v_imm;
          break;
        default: serror(bad_typ_op);
      } break;
    case sub: //sub <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]-mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]-mem.mem.top().erb_val[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case subi: //subi <type> <dest> <source> <imm>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]-i.c_imm;
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]-i.v_imm;
          break;
        default: serror(bad_typ_op);
      } break;
    case mult: //mult <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]*mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]*mem.mem.top().erb_val[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case multi: //multi <type> <dest> <source> <imm>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]*i.c_imm;
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]*i.v_imm;
          break;
        default: serror(bad_typ_op);
      } break;
    case div: //div <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_cnt[i.prm[3]]==0){serror(div_zero); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]/mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_val[i.prm[3]]==0){serror(div_zero); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]/mem.mem.top().erb_val[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case divi: //divi <type> <dest> <source> <imm>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          if(i.c_imm==0){serror(div_zero); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]/i.c_imm;
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          if(i.v_imm==0){serror(div_zero); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]/i.v_imm;
          break;
        default: serror(bad_typ_op);
      } break;
    case mod: //mod <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]%mem.mem.top().erb_cnt[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case modi: //modi <type> <dest> <source> <imm>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]%i.c_imm;
          break;
        default: serror(bad_typ_op);
      } break;
    case seq: //seq <type> <dest> <source1> <source2> 
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]==mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]==mem.mem.top().erb_val[i.prm[3]];
          break;
        case 2:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_elm.size()||
             i.prm[3]>=mem.mem.top().erb_elm.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_elm[i.prm[2]]==mem.mem.top().erb_elm[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case sne: //sne <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]!=mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]!=mem.mem.top().erb_val[i.prm[3]];
          break;
        case 2:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_elm.size()||
             i.prm[3]>=mem.mem.top().erb_elm.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=!(mem.mem.top().erb_elm[i.prm[2]]==mem.mem.top().erb_elm[i.prm[3]]);
          break;
        default: serror(bad_typ_op);
      } break;
    case slt: //slt <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]<mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]<mem.mem.top().erb_val[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case sgt: //sgt <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]>mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]>mem.mem.top().erb_val[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case slte: //slte <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]<=mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]<=mem.mem.top().erb_val[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case sgte: //sgte <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]>=mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]>=mem.mem.top().erb_val[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case land: //land <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]&&mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]&&mem.mem.top().erb_val[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case lor: //lor <type> <dest> <source1> <source2>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=mem.mem.top().erb_cnt[i.prm[2]]||mem.mem.top().erb_cnt[i.prm[3]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]||mem.mem.top().erb_val[i.prm[3]];
          break;
        default: serror(bad_typ_op);
      } break;
    case lnot: //lnot <type> <dest> <source1> 
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=!mem.mem.top().erb_cnt[i.prm[2]];
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_val[i.prm[1]]=!mem.mem.top().erb_val[i.prm[2]];
          break;
        default: serror(bad_typ_op);
      } break;
    case seed:
      if(i.prm[0]>=mem.mem.top().erb_cnt.size())
        {serror(inval_prm); break;}
      srand(mem.mem.top().erb_cnt[i.prm[0]]);
      break;
    case timeseed:
      srand(time(0));
      break;
    case gen: //gen <type> <dest> <min> <max>
      switch(i.prm[0])
      {
	u_long head; double res; //used later in case 1
        case 0:
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size()||
             i.prm[3]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          mem.mem.top().erb_cnt[i.prm[1]]=
            mem.mem.top().erb_cnt[i.prm[2]]+rand()%abs(
            mem.mem.top().erb_cnt[i.prm[3]]-mem.mem.top().erb_cnt[i.prm[2]]);  
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size()||
             i.prm[3]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_val[i.prm[3]]<mem.mem.top().erb_val[i.prm[2]])
            {serror(bad_rand); break;}
          head=(u_long)
            (mem.mem.top().erb_val[i.prm[3]]-mem.mem.top().erb_val[i.prm[2]]);
          res=(double)(rand()%head)+(double)rand()/(double)RAND_MAX;    
          mem.mem.top().erb_val[i.prm[1]]=mem.mem.top().erb_val[i.prm[2]]+res;
          break;
        default: serror(bad_typ_op);
      } break;
    case jump: //jump <dest>
      if(prog.head+i.prm[0]>prog.tail){serror(bad_jump); break;}
      prog.cur=prog.head+i.prm[0]-1; break;
    case jchance: //jchance <source> <dest>
      {
        if(i.prm[0]>=mem.mem.top().erb_val.size())
          {serror(inval_prm); break;}
        if(prog.head+i.prm[2]>prog.tail){serror(bad_jump); break;}
        double res=(double)rand()/(double)RAND_MAX;
        if(res<i.prm[0]) prog.cur=prog.head+i.prm[1]-1;
      } break;
    case jeq: //jeq <type> <source1> <source2> <dest>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_cnt[i.prm[1]]==mem.mem.top().erb_cnt[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_val[i.prm[1]]==mem.mem.top().erb_val[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        case 2:
          if(i.prm[1]>=mem.mem.top().erb_elm.size()||
             i.prm[2]>=mem.mem.top().erb_elm.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_elm[i.prm[1]]==mem.mem.top().erb_elm[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        default: serror(bad_typ_op);
      } break;
    case jne: //jne <type> <source1> <source2> <dest>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          if(!(mem.mem.top().erb_cnt[i.prm[1]]==mem.mem.top().erb_cnt[i.prm[2]]))
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          if(!(mem.mem.top().erb_val[i.prm[1]]==mem.mem.top().erb_val[i.prm[2]]))
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        case 2:
          if(i.prm[1]>=mem.mem.top().erb_elm.size()||
             i.prm[2]>=mem.mem.top().erb_elm.size())
            {serror(inval_prm); break;}
          if(!(mem.mem.top().erb_elm[i.prm[1]]==mem.mem.top().erb_elm[i.prm[2]]))
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        default: serror(bad_typ_op);
      } break;
    case jlt: //jlt <type> <source1> <source2> <dest>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_cnt[i.prm[1]]<mem.mem.top().erb_cnt[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_val[i.prm[1]]<mem.mem.top().erb_val[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        default: serror(bad_typ_op);
      } break;
    case jgt: //jgt <type> <source1> <source2> <dest>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_cnt[i.prm[1]]>mem.mem.top().erb_cnt[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_val[i.prm[1]]>mem.mem.top().erb_val[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        default: serror(bad_typ_op);
      } break;
    case jlte: //jlte <type> <source1> <source2> <dest>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_cnt[i.prm[1]]<=mem.mem.top().erb_cnt[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_val[i.prm[1]]<=mem.mem.top().erb_val[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        default: serror(bad_typ_op);
      } break;
    case jgte: //jgte <type> <source1> <source2> <dest>
      switch(i.prm[0])
      {
        case 0: 
          if(i.prm[1]>=mem.mem.top().erb_cnt.size()||
             i.prm[2]>=mem.mem.top().erb_cnt.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_cnt[i.prm[1]]>=mem.mem.top().erb_cnt[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        case 1:
          if(i.prm[1]>=mem.mem.top().erb_val.size()||
             i.prm[2]>=mem.mem.top().erb_val.size())
            {serror(inval_prm); break;}
          if(mem.mem.top().erb_val[i.prm[1]]>=mem.mem.top().erb_val[i.prm[2]])
          {
            if(prog.head+i.prm[3]>prog.tail){serror(bad_jump); break;}
            prog.cur=prog.head+i.prm[3]-1; break;
          }
          break;
        default: serror(bad_typ_op);
      } break;
    case jsub: //jsub <dest>
      if(prog.head+i.prm[0]>prog.tail){serror(bad_jump); break;}
      //mem.mem.add(mem.mem.top());
      mem.ra.push(prog.cur); prog.cur=prog.head+i.prm[0]-1; break;
    case ret:
      //mem.mem.pop();
      prog.cur=mem.ra.top(); mem.ra.pop(); break;
    case end:
      break;
  } 
}
//------------------------------------------------------------------------END
}}
