/*---------------------------------------------------------------------------
Erbium Assembler

Implementation of the Erbium Assembler
Written and Maintained by Jason Mars (JmarsKoder@gmail.com)
---------------------------------------------------------------------------*/

#ifndef ErbAssemblerHeader
#define ErbAssemblerHeader

//#include "erb_engine.h"
#include "erb_machine.h"
#include "scanner.h"

using namespace std;
using namespace k;
using k::u_long;

namespace erb { namespace assembler {
//Erbium Assembler-----------------------------------------------------------
enum err_type {serr, bad_data, bad_prm, no_lab, dup_lab, bad_val, bad_lab};
struct label {string name; u_long ptr;};
void process_prms(const u_long &num, u_long start=0);
void get_imm(const u_long &typ);
bool find_label(const string &s);
u_long label_value(const string &s);
void snag_labels();
void serror(err_type e); //Output Error
void assemble(const string &txt, ostream &out, ostream &er=cerr);
string assemble(const string &txt, ostream &er=cerr);
//------------------------------------------------------------------------END
}}
#endif
