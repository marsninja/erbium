/*---------------------------------------------------------------------------
 Erbium Machine
 
 Implementation of the abstract virtual machine
 Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
 ---------------------------------------------------------------------------*/

#ifndef ErbMachineHeader
#define ErbMachineHeader

#include "erb_engine.h"
#include <stack>

using namespace std;
using namespace k;
using k::u_long;

namespace erb { namespace machine {
    class erb_inst; //a single erb machine instruction
    class erb_memory; //memory management class for the machine
    class erb_program; //series of instructions that make an erb program
    class erb_machine; //the machine that executes erb programs
    extern bag<construct> renderlist; //globally visible list of renderable objects
    
    //Erbium Machine Op Codes----------------------------------------------------
    enum erb_op
    {
        expand, //expand <type> <units>; 0=count, 1=value, 2=construct
        deflate, //deflate <type> <units>; 0=count, 1=value, 2=construct
        pload, //pload <prim_id> <dest>; load primitive into dest
        spit, //spit <type> <source> prints mem loc contents to screen
        spitmap, //spitmap <source> #only with constructs, prints quake map format
        render, //render <source> #only constructs, adds to global renderlist
        
        cuni, //cuni <dest> <source1> <source2>
        cinter, //cinter <dest> <source1> <source2>
        csub, //csub <dest> <source1> <source2>
        
        crotx, //crotx <dest> <source1> <source2> //source 1 must be type 2
        croty, //croty <dest> <source1> <source2> //source 2 must be type 1
        crotz, //crotz <dest> <source1> <source2>
        cmovex, //cmovex <dest> <source1> <source2>
        cmovey, //cmovey <dest> <source1> <source2>
        cmovez, //cmovez <dest> <source1> <source2>
        cscalex, //cscalex <dest> <source1> <source2>
        cscaley, //cscaley <dest> <source1> <source2>
        cscalez, //cscalez <dest> <source1> <source2>
        
        csetx, //csetx <dest> <source1> <source2> //source 1 must be type 2
        csety, //csety <dest> <source1> <source2> //source 2 must be type 1
        csetz, //csetz <dest> <source1> <source2>
        csetw, //csetw <dest> <source1> <source2>
        csetl, //csetl <dest> <source1> <source2>
        cseth, //cseth <dest> <source1> <source2>
        
        cgetx, //cgetx <dest> <source> #dest is type value, source is construct
        cgety, //cgety <dest> <source>
        cgetz, //cgetz <dest> <source>
        cgetw, //cgetw <dest> <source>
        cgetl, //cgetl <dest> <source>
        cgeth, //cgeth <dest> <source>
        
        ccol, //ccol <type> <dest> <source1> <source2> #construct collision detection
        
        alloc, //alloc <type> <dest> <units_src>
        load, //load <type> <dest> <index> <source>
        store, //store <type> <source> <index> <dest>
        
        push, //push <stack> <type> <source>
        pop, //pop <stack> <type> <dest>
        
        copy, //copy <type> <dest> <source>
        cast, //cast <type> <dest> <source> //type 1 and 2 only, dest is of type, source is other
        copydown, //copydown <type> <dest> //send a value to previous window
        
        seti, //seti <type> <dest> <imm>; only supports type 0, and 1
        add, //add <type> <dest> <source1> <source2>
        addi, //addi <type> <dest> <source> <imm>
        sub, //sub <type> <dest> <source1> <source2>
        subi, //subi <type> <dest> <source> <imm>
        mult, //mult <type> <dest> <source1> <source2>
        multi, //multi <type> <dest> <source> <imm>
        div, //div <type> <dest> <source1> <source2>
        divi, //divi <type> <dest> <source> <imm>
        mod, //mod <type> <dest> <source1> <source2>
        modi, //modi <type> <dest> <source> <imm>
        
        seq, //seq <type> <dest> <source1> <source2>
        sne, //sne <type> <dest> <source1> <source2>
        slt, //slt <type> <dest> <source1> <source2>
        sgt, //sgt <type> <dest> <source1> <source2>
        slte, //slte <type> <dest> <source1> <source2>
        sgte, //sgte <type> <dest> <source1> <source2>
        land, //land <type> <dest> <source1> <source2>
        lor, //lor <type> <dest> <source1> <source2>
        lnot, //lnot <type> <dest> <source1>
        
        seed, //seed <value> #uses type count; seeds randomizer accordingly
        timeseed, //timeseed #seeds to current time
        gen,  //gen <type> <dest> <min> <max> #types 0 - 1, set random number
        
        jump, //jump <dest>
        jchance, //jchance <source> <dest> //source of type value
        //#if source=.5, there is a 50% chance
        //of jumping to the dest
        
        jeq, //jeq <type> <source1> <source2> <dest>
        jne, //jne <type> <source1> <source2> <dest>
        jlt, //jlt <type> <source1> <source2> <dest>
        jgt, //jgt <type> <source1> <source2> <dest>
        jlte, //jlte <type> <source1> <source2> <dest>
        jgte, //jgte <type> <source1> <source2> <dest>
        jsub, //jsub <dest>
        ret, //ret
        
        end //end
    };
    //------------------------------------------------------------------------END
    
    
    //Erbium Machine Instruction Class-------------------------------------------
    class erb_inst
    {
    public:
        erb_op op; //operation
        u_long prm[4]; //operands and immediates
        long c_imm;
        double v_imm;
    public:
        erb_inst();
        bool ascii_in(istream &in);
        void ascii_out(ostream &out);
    };
    //------------------------------------------------------------------------END
    
    
    //Erbium Memory Management Class---------------------------------------------
    class erb_memory
    {
    public:
        stack<unsigned int> ra; //return address for (jsub, and ret)
    public: //Stack structure
        //3 classes of memory
        bag<long> cnt_stk[2];
        bag<double> val_stk[2];
        bag<construct> elm_stk[2];
    public: //Register window structure
        struct core //each sub gets its own core
        {
            bag<long> erb_cnt;
            bag<double> erb_val;
            bag<construct> erb_elm;
            //Dynamic Allocation Structure
            bag< bag<long> > cnt_mem;
            bag< bag<double> > val_mem;
            bag< bag<construct> > elm_mem;
        };
        bag<core> mem;
    public:
        erb_memory();
        void clear();
    public:
        void print(ostream &out); //debug purposes only
    };
    //------------------------------------------------------------------------END
    
    
    //An Erbium Program----------------------------------------------------------
    class erb_program
    {
    public:
        bag<erb_inst> inst; //programs code
    public:
        unsigned int cur, head, tail; //pointers into the programs code
    public:
        erb_program();
        void reset();
        bool at_end(); //true if at the end of program
    public:
        bool ascii_in(istream &in);
        void ascii_out(ostream &out);
    };
    //------------------------------------------------------------------------END
    
    
    //Erbium Machine-------------------------------------------------------------
    class erb_machine
    {
    public:
        erb_memory mem; //erb memory
        erb_program prog; //current erb program
        bag<primitive> prim; //set of primitives
    private:
        ostream &err_out; //for all error reporting
        bool erred; //was there an error?
        enum err_type 
        {inval_op, inval_prm, bad_typ_op, div_zero, no_end, bad_jump, bad_rand};
        void serror(err_type e); //commit error
    public:
        erb_machine(bag<primitive> &pr, ostream &err=cerr);
        void load_program(const erb_program &p); //load program to be executed
        void trace(const u_long &ins=1); //execute ins number of instructions
        void execute(); //execute program
        void big_switch(const erb_inst &i); //apply the big switch to some i 
        void reset(); //clear memory, and got to beginning of program
        bool errored(); //true if there was an error
    public:
        bool ascii_load_primitives(istream &in);
        bool bin_load_primitives(istream &in);
    };
    //------------------------------------------------------------------------END
    
    
    //Erbium Machine Setup Function----------------------------------------------
    erb_machine erb_setup_machine(istream &in);
    //------------------------------------------------------------------------END
}}
#endif
