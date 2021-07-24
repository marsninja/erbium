#include "k-utils.h"
#include "scanner.h"
#include "k_vm.h"

#ifndef Krucial_LOW_Header
#define Krucial_LOW_Header
namespace k { namespace krucial {
//Label---------------------------------------------------------------------
struct label
{
  label();
  string name; u_long ptr;
};
//-----------------------------------------------------------------------END


//Assembler-----------------------------------------------------------------
class k_low
{
private:
  vector<label> lab;
  scanner scan;
  instruction dummy;
  u_long inst_num;
  void process_params(const u_long &num, u_long start=0);
  void get_immediate(const u_long &typ);
  bool find_label(const string &s);
  u_long label_value(const string &s);
  void snag_labels();
private:
  ostream &err_out; //For Error Reporting
  bool erred;
  enum err_type {serr, bad_param, no_lab, dup_lab, bad_val, bad_lab};
  void serror(err_type e); //Output Error
public:
  k_low(ostream &err=cout);
  void assemble(const string &txt, ostream &out);
  const string &text();
  bool errored(); //Returns true if error occured
};
//-----------------------------------------------------------------------END
}}
#endif
