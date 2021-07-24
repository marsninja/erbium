/*---------------------------------------------------------------------------
Erbium Machine

Implementation of the abstract virtual machine
Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#include "erb_machine.h"

namespace erb { namespace machine {
    //Erbium Machine Instruction Class-------------------------------------------
    erb_inst::erb_inst():op(end),c_imm(0),v_imm(0){for(int i=0;i<4;i++)prm[i]=0;}
    bool erb_inst::ascii_in(istream &in)
    {
        unsigned opval;
        in >> opval >> prm[0] >> prm[1] >> prm[2] >> prm[3] >> c_imm >> v_imm;
        op=(erb_op)opval;
        return true;
    }
    
    void erb_inst::ascii_out(ostream &out)
    {
        out << op << ' ' << prm[0] << ' ' << prm[1] << ' ' << prm[2] << ' '
        << prm[3] << ' ' << c_imm << ' ' << v_imm << endl;
    }
    //------------------------------------------------------------------------END
    
    
    //Erbium Memory Manager------------------------------------------------------
    erb_memory::erb_memory(){clear();}
    
    void erb_memory::clear()
    {
        for(u_long i=0; i<mem.size(); i++)
        {mem[i].erb_cnt.clear(); mem[i].erb_val.clear(); mem[i].erb_elm.clear();}
        mem.add(core());
        while(!ra.empty()) ra.pop();
    }
    
    void erb_memory::print(ostream &out)
    {
        out << "Current State\n";
        out << "Windows: " << mem.size() << endl;
    }
    //------------------------------------------------------------------------END
    
    
    //Erbium Program-------------------------------------------------------------
    erb_program::erb_program():cur(0),head(0),tail(0){}
    
    void erb_program::reset(){cur=0;}
    
    bool erb_program::at_end(){return cur==tail;}
    
    bool erb_program::ascii_in(istream &in)
    {
        while(!in.eof())
        {
            erb_inst i;
            i.ascii_in(in);
            inst.add(i);
        }
        if(inst.size()<1) return false;
        cur=head=0;
        tail=(unsigned int)inst.size()-1;
        return true;
    }
    
    void erb_program::ascii_out(ostream &out)
    {
        for(u_long i=0; i<inst.size(); i++)
            inst[i].ascii_out(out);
    }
    //------------------------------------------------------------------------END
    
    
    //Erbium Machine-------------------------------------------------------------
    void erb_machine::serror(err_type e)
    {
        static char *er[]=
        {
            "Invalid Operation!",
            "Invalid Parameter Values!",
            "Operation Does Not Support Specified Type!",
            "Fatal Division by Zero!",
            "Instruction Pointer Lost, No 'end' Operator Found!",
            "This Jump Targets an Invalid Location!",
            "Invalid Range for Randomization!"
        };
        prog.inst[prog.cur].ascii_out(err_out);
        err_out << er[e] << endl; erred=true;
    }
    
    erb_machine::erb_machine(bag<primitive> &pr, ostream &err):
    prim(pr),err_out(err),erred(false){}
    
    void erb_machine::load_program(const erb_program &p)
    {
        prog=p; 
        erred=false;
    }
    
    //void erb_machine::trace(const u_long &ins) //located in erb_proc.cpp
    //void erb_machine::execute() //located in erb_proc.cpp
    //void erb_machine::big_switch(const erb_inst &i) //located in erb_proc.cpp
    
    void erb_machine::reset(){prim.clear(); mem.clear(); prog.reset();}
    
    bool erb_machine::errored(){return erred;}
    //------------------------------------------------------------------------END
    
    
    //Erbium Machine Setup Function----------------------------------------------
    erb_machine erb_setup_machine(istream &in)
    {
        bag<erb::primitive> p;
        erb::construct c; c.ascii_in(in);
        for(u_long i=0; i<c.num_brushes(); i++)
        {
            erb::primitive t; t.set(c[i]);
            p.add(t);
        }
        
        erb_program prog; prog.ascii_in(in);
        erb_machine proc(p, cerr);
        proc.load_program(prog);
        return proc;
    }
    //------------------------------------------------------------------------END

}}
