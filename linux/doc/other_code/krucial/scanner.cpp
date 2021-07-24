/**************************************************************************
 * JmarsKoder Presents 
 * JmarsKoder's Code Parsing System v. 1.0
 * 
 * written by Jason Mars (jmarskoder@gmail.com)
 * ***********************************************************************/

#include "scanner.h"

namespace k {
//Scanner-------------------------------------------------------------------
bool scanner::is_delim(char c)
{
  if(strchr(" +-/\\*%^=()><[]&!|:;#~@$,.'?{}\n\r",c)||c==9||c==0)
    return true;
  return false;
}

bool scanner::is_sp_tab(char c)
{
  if(c==' '||c=='\t') return true; 
  return false;
}

void scanner::serror(err_type e)
{
  static char *er[]=
  {
    "Syntax Error!",
    "Unbalanced Quotes!"
  };
  err_out << "Line " << line << ": " << er[e] << endl; erred=true;
}

scanner::scanner(ostream &err):err_out(err),loc(0),line(0),erred(0){}

scanner::scanner(const string &t, ostream &err):
  txt(t),err_out(err),loc(0),line(1),erred(false){}

void scanner::setup(const string &t, const u_long &sl)
  {txt=t+'\0';loc=0;line=sl;erred=0;}

bool scanner::get_token(token &t) //Get Next Token
{
  erred=0; size_t size=txt.size();
  t.type=undef_tok; t.tok="";
  if(loc>=size){return false;}
  while(is_sp_tab(txt[loc])||txt[loc]=='\n'||txt[loc]=='\r'||txt[loc]=='\0')
  {
    if(txt[loc]=='\n'||txt[loc]=='\r'){line++;}
    loc++; if(loc>=size){return false;}
  }
  if(txt[loc]=='\0') {return false;}
  if(strchr("+-/\\*%^=()><[]&!|:;#~@$?.,'{}",txt[loc])) //Operator
  {
    t.tok=txt[loc];
    t.type=oper_tok;
    loc++; 
    if(loc<size&&txt[loc]=='/'&&txt[loc-1]=='/') //C++ Style Comments
    {
      t.type=comment_tok;
      while(txt[loc]!='\n'&&txt[loc]!='\r')
      {
        t.tok+=txt[loc]; loc++;
        if(loc>=size){break;}
      }
    }
    else if(loc<size&&txt[loc]=='*'&&txt[loc-1]=='/') //C Style Comments
    {
      t.type=comment_tok;
      t.tok+='*'; loc++;
      while(loc+1<size&&(txt[loc]!='*'||txt[loc+1]!='/'))
      {
        t.tok+=txt[loc];
        if(txt[loc]=='\n'||txt[loc]=='\r'){line++;}
        loc++;
      }
      t.tok+=txt[loc]; loc++; t.tok+=txt[loc]; loc++;
    }
  }
  else if(txt[loc]=='\"') //Quotes
  {
    if(loc>0&&txt[loc-1]=='\\')
    {
      t.type=oper_tok;
      t.tok+='\"';
      loc++; return true;
    }
    loc++;
    while(txt[loc]!='\"'||txt[loc-1]=='\\')
    {
      if(txt[loc]=='\\')
      {
        loc++;
        switch(txt[loc])
        {
          case '\'': t.tok+='\''; break;
          case '\"': t.tok+='\"'; break;
          case '\?': t.tok+='\?'; break;
          case '\\': t.tok+='\\'; break;
          case 'a': t.tok+='\a'; break;
          case 'b': t.tok+='\b'; break;
          case 'f': t.tok+='\f'; break;
          case 'n': t.tok+='\n'; break;
          case 'r': t.tok+='\r'; break;
          case 't': t.tok+='\t'; break;
          case 'v': t.tok+='\v'; break;
        }
        loc++; continue;
      }
      else if(strchr("\n\r\0",txt[loc])){serror(quot); break;}
      t.tok+=txt[loc]; loc++;
    }
    t.type=quote_tok;
    loc++;
  }
  else if(isdigit(txt[loc])) //Number (supports sci. notation)
  { 
    while(isdigit(txt[loc]))
    {
      t.tok+=txt[loc]; loc++;
      if(loc>=size){break;}
    }
    if(txt[loc]=='.'){t.tok+='.';loc++;}
    while(isdigit(txt[loc]))
    {
      t.tok+=txt[loc]; 
      loc++;
      if(loc>=size){break;}
    }
    if(txt[loc]=='e'&&((loc+1<size&&isdigit(txt[loc+1]))||
       (loc+2<size&&txt[loc+1]=='-'&&isdigit(txt[loc+2]))||
       (loc+2<size&&txt[loc+1]=='+'&&isdigit(txt[loc+2]))))
    {
      t.tok+=txt[loc++]; t.tok+=txt[loc++];
      while(isdigit(txt[loc]))
      {
        t.tok+=txt[loc];
        loc++;
        if(loc>=size){break;}
      }
    }
    t.type=num_tok;
  }
  else if(isalpha(txt[loc])) //Alpha String
  {
    while(!is_delim(txt[loc])&&loc<size){t.tok+=txt[loc]; loc++;}
    t.type=str_tok;
  }
  else 
  {
    t.tok+=txt[loc]; loc++; return false;
  }
  return true;
}

bool scanner::get_token()
{
  return get_token(tok);
}

bool scanner::errored() //If Get-Token Caused Error
{
  return erred;
}

void scanner::set_line(u_long l)
{
  line=l;
}

u_long scanner::current_line() //Current Line
{
  return line;
}

const token &scanner::current_token() //Current Token
{
  return tok;
}

tok_type scanner::token_type() //Type of Token
{
  return tok.type;
}
 
const string &scanner::token_text() //Text of Token
{
  return tok.tok;
}

token scanner::look_ahead() //Look Ahead 1 Token
{
  token tmp; int l=loc;
  get_token(tmp);
  loc=l; 
  return tmp;
}

u_long scanner::location(){return loc;} //Returns current location in text
void scanner::location(u_long x){loc=x;} //Sets current location in text

const string &scanner::text(){return txt;}
//-----------------------------------------------------------------------END


//Utility Functions---------------------------------------------------------
string string_to_quote(const string &s) //turn esc. seq. symbols into text 
{
  string out="\"";
  for(u_long i=0; i<s.size(); i++)
  {
    switch(s[i])
    {
      case '\'': out+="\\\'"; break;
      case '\"': out+="\\\""; break;
      case '\?': out+="\\?"; break;
      case '\\': out+="\\\\"; break;
      case '\a': out+="\\a"; break;
      case '\b': out+="\\b"; break;
      case '\f': out+="\\f"; break;
      case '\n': out+="\\n"; break;
      case '\r': out+="\\r"; break;
      case '\t': out+="\\t"; break;
      case '\v': out+="\\v"; break;
      default: out+=s[i];
    }
  }
  out+="\"";
  return out;
}
//-----------------------------------------------------------------------END
}
