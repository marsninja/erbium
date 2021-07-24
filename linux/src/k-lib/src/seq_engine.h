/**************************************************************************
 * JmarsKoder Presents 
 * JmarsKoder's Sequitur Engine
 * Adapted from original sequitur c++ code
 * 
 * written by Jason Mars (jmarskoder@gmail.com){}
 * ***********************************************************************/

#include "k-utils.h"
#include <deque>
#include <set>

#ifndef K_Sequitur_Header
#define K_Sequitur_Header
namespace k {
//Global Stuff-------------------------------------------------------------
#define SEQ_TAB_SIZE 311111
struct seq_element
{
  bool is_term; 
  u_long value, length, cold_uses;
  double coverage;
  seq_element(bool t, u_long v);
};
//-------------------------------------------------------------------------


//Sequitur-----------------------------------------------------------------
class seq_engine
{
  class seq_rule;
  class seq_symbol;
  seq_symbol **seq_table;
  seq_symbol **seq_find_digram(seq_symbol *s);
  u_long num_rules; //Number of total rules count

  class seq_symbol 
  {
    bool is_t; //Is Terminal
    seq_symbol *n, *p; //Pointer to previous and next symbols
    u_long s; //Value of term or pointer to rule
    seq_engine *master; //Seq Engine I belong to
  public:
    seq_symbol(seq_engine *m, u_long sym); //If terminal
    seq_symbol(seq_engine *m, seq_rule *r); //If Rule
    void delete_self(); //Delete self
    void insert_after(seq_symbol *y); //Push a symbol to end of rule
    void delete_digram(); //Remove digram from hash table
    bool is_guard(); //Test if self is guard
    bool non_term(); //Test if self is a non terminal
    seq_symbol *next(); //Returns pointer to next symbol
    seq_symbol *prev(); //Returns pointer to prev symbol
    inline u_long value(); //Return value
    seq_rule *rule(); //Return pointer to rule if self is rule
    void substitute(seq_rule *r); //Replace digram w/ rule
    bool check(); //Uses match, checks if digram is a rule
    void expand(); //Replace rule with digram 
    void point_to_self(); //For guards only
  private:
    static void join(seq_symbol *left, seq_symbol *right);
    static void match(seq_symbol *ss, seq_symbol *m);
  };


  class seq_rule 
  {
  private:
    seq_symbol *guard; //points to self 
    u_long count; //occurances in the grammar
    u_long number; //index
    seq_engine *master; //Seq Engine I belong to
  public:
    seq_rule(seq_engine *m);
    void delete_self();
    void reuse(); //increment count
    void deuse(); //decrement count
    seq_symbol *first(); //first symbol in rule
    seq_symbol *last(); //last symbol in rule
    u_long freq(); //return count
    u_long index(); //return index
    void index(u_long i); //set index
  };

private:
  seq_rule *start; //Start Symbol of seq Grammar
  u_long input_size; //total size of input
  set<seq_symbol *> s_scrub; //Reset scrub lists
  set<seq_rule *> r_scrub;
private:
  void clean(seq_rule *r); //Fill scrub lists
  u_long calc_length (seq_rule *r); //Recursively count length
public:
  deque<deque<seq_element> > gram; //Post harvest grammar
public:
  seq_engine(); 
  ~seq_engine();
  void append(u_long in); //Add 1 item to input
  void harvest(); //Generate post harvest grammar
  void reset(); //clean memory and start over

  u_long length(); //return current inputsize
  deque<u_long> expand_rule(u_long idx); //Expands a rule, gives list of terms
  void print_rule(u_long idx); //print rule to screen
  void print_grammar(); //print grammar to screen
  void print_stats(); //print grammar stats to screen
};
//-------------------------------------------------------------------------
}
#endif
