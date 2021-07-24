%{
#include "k-utils.h"
#include "molokai.h"
#include "molokai.tab.h"
  
u_long yyline=1;
void ReportError(const char *s);
%}

D     [0-9]
L     [a-zA-Z_]
E     [Ee][+-]?{D}+

%%

"//".* 
{D}*"."{D}+({E})? { 
                    yylval.line=yyline; 
                    char *a; yylval.num=strtod(yytext, &a); 
                    return NUMBER;
                  }
{D}+"."{D}*({E})? {
                    yylval.line=yyline; 
                    char *a; yylval.num=strtod(yytext, &a); 
                    return NUMBER;
                  }
{D}+ {
       yylval.line=yyline; 
       char *a; yylval.cnt=strtoul(yytext, &a, 0); 
       return COUNT;
     }
"number" {return NUM_TYPE;}
"counter" {return COUNT_TYPE;}
"construct" {return CONSTRUCT_TYPE;}
"load" {return LOAD;}
[xX] {return X;}
[yY] {return Y;}
[zZ] {return Z;}
[wW] {return W;}
[lL] {return L;}
[hH] {return H;}
"POS"|"pos" {return POS;}
"DIM"|"dim" {return DIM;}
"maybe"|"or_maybe" {return MAYBE;}
"otherwise" {return OTHERWISE;}
"rotate" {return ROTATE;}
"move" {return MOVE;}
"scale" {return SCALE;}
"print" {return PRINT;}
"printmap" {return PRINTMAP;}
"generate" {return GEN;}
"->" {return UPTO;}
">=" {return GTE;}
"<=" {return LTE;}
"==" {return EQ;}
"!=" {return NE;}
"&&" {return LAND;}
"||" {return LOR;}
{L}({D}|{L})* {yylval.line=yyline; strcpy(yylval.id, yytext); return ID;}

[ \t] {} 
[\n] {yyline++;}

. {return yytext[0];}

%%

void ReportError(const char *s)
{
  printf("Error Line %d: %s\n", (int)yyline, s);
}

int yywrap(void){return 1;}
