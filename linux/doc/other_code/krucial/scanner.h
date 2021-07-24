/**************************************************************************
 * JmarsKoder Presents 
 * JmarsKoder's Code Parsing System v. 1.0
 * 
 * written by Jason Mars (jmarskoder@gmail.com)
 * ***********************************************************************/

#include "k-utils.h"

#ifndef K_Scanner_Header
#define K_Scanner_Header
namespace k {
//Basic Globals-------------------------------------------------------------
enum tok_type {undef_tok, oper_tok, num_tok, 
               str_tok, quote_tok, comment_tok};
//-----------------------------------------------------------------------END


//Basic Scanner-------------------------------------------------------------
struct token
{
  string tok;
  tok_type type;
};
  
class scanner
{
private:
  string txt;
  token tok;
  ostream &err_out;
  u_long loc;
  u_long line;
  bool erred;
  enum err_type {serr, quot};
private:
  bool is_delim(char c); //Is Delimiter?
  bool is_sp_tab(char c); //Is Space or Tab?
  void serror(err_type e); //Output Error
public:
  scanner(ostream &err=cout); //Error Stream
  scanner(const string &t, ostream &err=cout);
  void setup(const string &t, const u_long &sl=1); //T=Text sl=Start Line
  bool get_token(token &t); //Get Next Token
  bool get_token(); //Get Next Token (Internal)
  bool errored(); //If Get-Token Caused Error
  void set_line(u_long l); //Sets current line
  u_long current_line(); //Current Line
  const token &current_token(); //Current Token
  tok_type token_type(); //Type of Token
  const string &token_text(); //Text of Token
  token look_ahead(); //Look Ahead 1 Token
  u_long location(); //Returns current location in text
  void location(u_long x); //Sets current location in text
  const string &text(); //Returns complete text
};
//-----------------------------------------------------------------------END


//Utility Functions---------------------------------------------------------
string string_to_quote(const string &s); //turn esc. seq. symbols into text 
//-----------------------------------------------------------------------END
}
#endif
