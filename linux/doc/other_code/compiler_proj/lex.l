%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"

#define LIM 500

int yyline, yycolumn;

extern int yycolumn, yylength, yyline;
extern int yylex();
void ReportError(const char *s);

char strg_tbl[LIM][LIM];
unsigned used=0;

%}

/* regular definitions */
delim           [ \t]
ws              {delim}+
newline         [\n]

declarations    [dD][eE][cC][lL][aA][rR][aA][tT][iI][oO][nN][sS]
enddeclarations  [eE][nN][dD][dD][eE][cC][lL][aA][rR][aA][tT][iI][oO][nN][sS]
method          [mM][eE][tT][hH][oO][dD]
program         [pP][rR][oO][gG][rR][aA][mM]
val             [vV][aA][lL]
int             [iI][nN][tT]
while           [wW][hH][iI][lL][eE]
class           [cC][lL][aA][sS][sS]
else            [eE][lL][sS][eE]
if              [iI][fF]
return          [rR][eE][tT][uU][rR][nN]
void            [vV][oO][iI][dD]

const_int       [0-9]+
letter          [a-zA-Z]
identifier      {letter}+({const_int}|{letter})*
errid           [0-9]+{identifier}

%%
{newline}           {yyline++; yycolumn=0;}
{ws}                {yycolumn+=yyleng;}
"/*".*"*/"      {yyline++;}
"/*".*"*/"{ws}      {yyline++;}
"/*".*              {ReportError("Invalid Comment!");}
{errid}             {yycolumn += yyleng; ReportError("Invalid ID");}
"&&"                {yycolumn += yyleng; return ANDnum;}
":="                {yycolumn += yyleng; return ASSGNnum;}
{declarations}      {yycolumn += yyleng; return DECLARATIONSnum;} 
"."                 {yycolumn += yyleng; return DOTnum;} 
{enddeclarations}    {yycolumn += yyleng; return ENDDECLARATIONSnum;}  
"="                 {yycolumn += yyleng; return EQUALnum;} 
">"                 {yycolumn += yyleng; return GTnum;}
{int}               {yycolumn += yyleng; return INTnum;} 
"["                 {yycolumn += yyleng; return LBRACnum;} 
"("                 {yycolumn += yyleng; return LPARENnum;} 
{method}            {yycolumn += yyleng; return METHODnum;} 
"!="                {yycolumn += yyleng; return NEnum;} 
"||"                {yycolumn += yyleng; return ORnum;} 
{program}           {yycolumn += yyleng; return PROGRAMnum;} 
"]"                 {yycolumn += yyleng; return RBRACnum;} 
")"                 {yycolumn += yyleng; return RPARENnum;} 
";"                 {yycolumn += yyleng; return SEMInum;} 
{val}               {yycolumn += yyleng; return VALnum;} 
{while}             {yycolumn += yyleng; return WHILEnum;} 
{class}             {yycolumn += yyleng; return CLASSnum;} 
","                 {yycolumn += yyleng; return COMMAnum;} 
"/"                 {yycolumn += yyleng; return DIVIDEnum;} 
{else}              {yycolumn += yyleng; return ELSEnum;} 
"=="                {yycolumn += yyleng; return EQnum;} 
">="                {yycolumn += yyleng; return GEnum;} 
{const_int}         {
                      yylval.intg=atoi(yytext);
                      yycolumn += yyleng; 
                      return ICONSTnum;
                    } 
{if}                {yycolumn += yyleng; return IFnum;} 
"{"                 {yycolumn += yyleng; return LBRACEnum;} 
"<="                {yycolumn += yyleng; return LEnum;} 
"<"                 {yycolumn += yyleng; return LTnum;} 
"-"                 {yycolumn += yyleng; return MINUSnum;} 
"!"                 {yycolumn += yyleng; return NOTnum;} 
"+"                 {yycolumn += yyleng; return PLUSnum;} 
"}"                 {yycolumn += yyleng; return RBRACEnum;} 
{return}            {yycolumn += yyleng; return RETURNnum;} 
\'(\\.|[^\\'\n])*\' {
                      int count;
                      char *i, *j, dupe[strlen(yytext)+1];
                      if(strlen(yytext)>LIM-1){ReportError("String Too Long");}
                      if(used>LIM-1){ReportError("String Table Full");}
                      i=&yytext[0];
                      j=&dupe[0];
                      while(*i)
                      {
                        if(*i!='\\') {*j=*i; j++; i++; continue;}
                        else
                        {
                          switch(*(i+1))
                          {
                            case 'n': *j='\n'; j++; i++; i++; break;
                            case 't': *j='\t'; j++; i++; i++; break;
                            case '\'': *j='\''; j++; i++; i++; break;
                            case '\\': *j='\\'; j++; i++; i++; break;
                            default: *j=*i; j++; i++; break;
                          }
                        }
                      }
                      *j='\0';
                      for(count=0; count<used; count++)
                      {
                        if(!strcmp(strg_tbl[count], dupe))
                        {
                          yylval.intg=count;
                          yycolumn += yyleng; return SCONSTnum;
                        }
                      }
                      yylval.intg=used;
                      strcpy(strg_tbl[used++], dupe);
                      yycolumn += yyleng; return SCONSTnum;
                    } 
\'(\\.|[^\\'\n])*   {ReportError("Invalid String Constant");}
"*"                 {yycolumn += yyleng; return TIMESnum;} 
{void}              {yycolumn += yyleng; return VOIDnum;} 
{identifier}        {
                      int count;
                      char *i;
                      i=&yytext[0];
                      while(*i){*i=tolower(*i); i++;}
                      for(count=0; count<used; count++)
                      {
                        if(!strcmp(strg_tbl[count], yytext))
                        {
                          yylval.intg=count;
                          yycolumn += yyleng; return IDnum;
                        }
                      }
                      yylval.intg=used;
                      strcpy(strg_tbl[used++], yytext);
                      yycolumn += yyleng; return IDnum;
                    } 
.                   {ReportError("Invalid Character");}
%%

void ReportError(const char *s)
{
  printf("Error Line %d, Col %d: %s\n", yyline, yycolumn,  s);
}

/*int main(int argc, char **argv)
{
  if(argc<2){printf("Usage: ./a.out <filename>\n"); exit(1);}
  int lexrtn=1;
  char* tokenid[40]={"EOFnum", "ANDnum", "ASSGNnum",
                     "DECLARATIONSnum", "DOTnum", "ENDDECLARATIONSnum",
                     "EQUALnum", "GTnum", "IDnum", "INTnum", "LBRACnum",
                     "LPARENnum", "METHODnum", "NEnum", "ORnum", "PROGRAMnum",
                     "RBRACnum", "RPARENnum", "SEMInum", "VALnum", "WHILEnum",
                     "CLASSnum", "COMMAnum", "DIVIDEnum", "ELSEnum", "EQnum",
                     "GEnum", "ICONSTnum", "IFnum", "LBRACEnum", "LEnum", "LTnum",
                     "MINUSnum", "NOTnum", "PLUSnum", "RBRACEnum", "RETURNnum",
                     "SCONSTnum", "TIMESnum", "VOIDnum" };

  yyline = 1;  yycolumn = 0; yyin=fopen(argv[1], "r");
  if(!yyin){printf("Usage: ./a.out <filename>\n"); exit(1);}
  printf("Line\tColumn\t%-16s\tyylval\n", "Token");
  while( lexrtn!=EOFnum )
  {
     yylval.intg=-1;
     switch ( lexrtn = yylex() ){
       case EOFnum:        
         printf("%d\t%d\t%-16s\n",yyline,yycolumn, tokenid[lexrtn]); break;
       default: 
         if(yylval.intg>=0)
           printf("%d\t%d\t%-16s\t%d\n",yyline,yycolumn, tokenid[lexrtn - 256], yylval.intg);
         else
           printf("%d\t%d\t%-16s\n",yyline,yycolumn, tokenid[lexrtn - 256]);
     }
  }

  printf("\n\nString Table\n\n");
  for(lexrtn=0; lexrtn<used; lexrtn++)
  {
    printf("%d: %s\n", lexrtn+1, string_table[lexrtn]);
  }
  return 0;
}*/
