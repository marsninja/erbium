%{ /* definition */
#include "proj2.h"
#include <stdio.h>

tree glob;
tree rememberme;

%}

%token <intg> ANDnum ASSGNnum DECLARATIONSnum DOTnum ENDDECLARATIONSnum EQUALnum GTnum IDnum INTnum LBRACnum LPARENnum METHODnum NEnum ORnum PROGRAMnum RBRACnum RPARENnum SEMInum VALnum WHILEnum CLASSnum COMMAnum DIVIDEnum ELSEnum EQnum GEnum ICONSTnum IFnum LBRACEnum LEnum LTnum MINUSnum NOTnum PLUSnum RBRACEnum RETURNnum SCONSTnum TIMESnum VOIDnum EOFnum Dummy

%type <tptr> Program ClassDeclList ClassDecl CBList ClassBody Decls MethodDecl FDList FieldDecl VDList Type VariableDeclId VariableInitializer BRACList TBList ArrayInitializer ArrayCreationExpression Expression SimpleExpression Term Factor VIList TermList ACExpressionList UnsignedConstant MethodCallStatement Variable VExpressionList MCExpressionList SelectList Block FormalParameterList Statements StatementList AssignmentStatement Statement ReturnStatement WhileStatement VAL RAL VL RL IfStatement VoidFormalParameterList

/**/

%%

Program : PROGRAMnum IDnum SEMInum ClassDeclList
  {$$=MakeTree(ProgramOp, $4, MakeLeaf(IDNode, $2)); };

ClassDeclList : ClassDecl
				{$$=MakeTree(ClassOp, NullExp(), $1);}
              //ex4.txt <-- a list of classes in one program!
		      | ClassDeclList ClassDecl 
				{$$=MakeTree(ClassOp, $1, $2);} 
              //Before, the second rule was switched:
              //| ClassDecl ClassDeclList
			  //{$$=MakeTree(ClassOp, $2, $1);}

ClassDecl : CLASSnum IDnum ClassBody
  	    {$$=MakeTree(ClassDefOp, $3, MakeLeaf(IDNode, $2));};

ClassBody : LBRACEnum CBList MethodDecl RBRACEnum 
  	    {$$=MakeTree(BodyOp, $2, $3);};
	  | LBRACEnum Decls MethodDecl RBRACEnum
  	    {$$=MakeTree(BodyOp, $2, $3);};
	  | LBRACEnum Decls RBRACEnum
        //ex4.txt <-- a class that has no method, only decls!
	  	{$$=$2;}; //<-- We had an extra BodyOp child with a dummy node here, 
	  	//Before, it looked like:
  	    //{$$=MakeTree(BodyOp, $2, NullExp());};
	  | LBRACEnum MethodDecl RBRACEnum
  	    {$$=MakeTree(BodyOp, NullExp(), $2);};
	  | LBRACEnum RBRACEnum
		//ex9.txt <-- a class with no body at all, just {}
		{$$=NullExp();}; //We had an unwanted BodyOp tier before:
		//{$$=MakeTree(BodyOp, NullExp(), NullExp());};

CBList : Decls MethodDecl
		 {$$=MakeTree(BodyOp, $1, $2);};
       | MethodDecl
		 {$$=MakeTree(BodyOp, NullExp(), $1);};
       | CBList MethodDecl
		 {$$=MakeTree(BodyOp, $1, $2);};
	 
Decls : DECLARATIONSnum FDList FieldDecl ENDDECLARATIONSnum
		{$$=MakeTree(BodyOp, $2, $3);};
	   |  DECLARATIONSnum FieldDecl ENDDECLARATIONSnum
		{$$=MakeTree(BodyOp, NullExp(), $2);};
	   | DECLARATIONSnum ENDDECLARATIONSnum 
		//ex8.txt <-- a class with no method and no declarations!	    
		{$$=MakeTree(BodyOp, NullExp(), NullExp());}; //added this line
		//Need an extra BodyOp node for this, that's all!  Before it was:
		//{$$=NullExp();};

MethodDecl : METHODnum VOIDnum IDnum  LPARENnum VoidFormalParameterList RPARENnum Block
	     {$$=MakeTree(MethodOp, MakeTree(HeadOp, MakeLeaf(IDNode, $3), $5), $7);};
	   | METHODnum Type IDnum  LPARENnum FormalParameterList RPARENnum Block
	     {$$=MakeTree(MethodOp, MakeTree(HeadOp, MakeLeaf(IDNode, $3), $5), $7);};

Block : Decls StatementList
	{$$=MakeTree(BodyOp, $1, $2);};
      |	StatementList
	{$$=MakeTree(BodyOp, NullExp(), $1);};
	
VoidFormalParameterList : VAL
		      {$$=MakeTree(SpecOp, $1, NullExp());};
		    | RAL
		      {$$=MakeTree(SpecOp, $1, NullExp());};
		    | 
		      {$$=MakeTree(SpecOp, NullExp(), NullExp());};

FormalParameterList : VAL
		      {$$=MakeTree(SpecOp, $1, glob);};
		    | RAL
		      {$$=MakeTree(SpecOp, $1, glob);};
		    | 
		      {$$=MakeTree(SpecOp, NullExp(), glob);};

RAL :	      INTnum IDnum
		      {$$=MakeTree(RArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), NullExp());};
		|     INTnum IDnum SEMInum VAL
		      {$$=MakeTree(RArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), $4);};
		|     INTnum IDnum SEMInum RAL
		      {$$=MakeTree(RArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), $4);};
		|     INTnum IDnum RL
		      {$$=MakeTree(RArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), $3);};
		
		
RL : COMMAnum IDnum
     {$$=MakeTree(RArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), NullExp());};
   | COMMAnum IDnum RL
     {$$=MakeTree(RArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), $3);};
   | COMMAnum IDnum SEMInum RAL
     {$$=MakeTree(RArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), $4);};
   | COMMAnum IDnum SEMInum VAL
     {$$=MakeTree(RArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), $4);};

VAL :	      VALnum INTnum IDnum
		      {$$=MakeTree(VArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $3), MakeLeaf(INTEGERTNode, INTnum)), NullExp());};
		|     VALnum INTnum IDnum SEMInum VAL
		      {$$=MakeTree(VArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $3), MakeLeaf(INTEGERTNode, INTnum)), $5);};
		|     VALnum INTnum IDnum SEMInum RAL
		      {$$=MakeTree(VArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $3), MakeLeaf(INTEGERTNode, INTnum)), $5);};
		|     VALnum INTnum IDnum VL
		      {$$=MakeTree(VArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $3), MakeLeaf(INTEGERTNode, INTnum)), $4);};

VL : COMMAnum IDnum
     {$$=MakeTree(VArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), NullExp());};
   | COMMAnum IDnum VL
     {$$=MakeTree(VArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), $3);};
   | COMMAnum IDnum SEMInum RAL
     {$$=MakeTree(VArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), $4);};
   | COMMAnum IDnum SEMInum VAL
     {$$=MakeTree(VArgTypeOp, MakeTree(CommaOp, MakeLeaf(IDNode, $2), MakeLeaf(INTEGERTNode, INTnum)), $4);};

StatementList : LBRACEnum Statements Statement SEMInum RBRACEnum
		{$$=MakeTree(StmtOp, $2, $3);};
              | LBRACEnum Statement SEMInum RBRACEnum
		{$$=MakeTree(StmtOp, NullExp(), $2);};
              | LBRACEnum Statements Statement RBRACEnum
		{$$=MakeTree(StmtOp, $2, $3);};
              | LBRACEnum Statement RBRACEnum
		{$$=MakeTree(StmtOp, NullExp(), $2);};
		      | LBRACEnum RBRACEnum
		{$$=MakeTree(StmtOp, NullExp(), NullExp());};

Statements : Statement SEMInum
	     {$$=MakeTree(StmtOp, NullExp(), $1);};
           | Statements Statement SEMInum 
	     {$$=MakeTree(StmtOp, $1, $2);};

Statement : AssignmentStatement
		{$$=$1;};
	  | MethodCallStatement
		{$$=$1;};
	  | WhileStatement
		{$$=$1;};
	  | ReturnStatement
		{$$=$1;};
	  | IfStatement
		{$$=$1;};

WhileStatement : WHILEnum Expression StatementList
		  {$$=MakeTree(LoopOp, $2, $3);};

ReturnStatement : RETURNnum 
		  {$$=MakeTree(ReturnOp, NullExp(), NullExp());};
		| RETURNnum Expression
		  {$$=MakeTree(ReturnOp, $2, NullExp());};

IfStatement : IFnum Expression StatementList
	      {$$=MakeTree(IfElseOp, NullExp(), MakeTree(CommaOp, $2, $3));};
	    | IfStatement ELSEnum IFnum Expression StatementList
	      {$$=MakeTree(IfElseOp, $1, MakeTree(CommaOp, $4, $5));};
	    | IfStatement ELSEnum StatementList
              {$$=MakeTree(IfElseOp, $1, $3);};

AssignmentStatement : Variable ASSGNnum Expression 
		      {$$=MakeTree(AssignOp, MakeTree(AssignOp, NullExp(), $1), $3);};

FDList : FDList FieldDecl
	 {$$=MakeTree(BodyOp, $1, $2);};
       | FieldDecl
	 {$$=MakeTree(BodyOp, NullExp(), $1);};

FieldDecl : Type VDList VariableDeclId SEMInum
	    {$$=MakeTree(DeclOp, $2, MakeTree(CommaOp, $3, MakeTree(CommaOp, glob, NullExp()))); };
          | Type VDList VariableDeclId EQUALnum VariableInitializer SEMInum
	    {$$=MakeTree(DeclOp, $2, MakeTree(CommaOp, $3, MakeTree(CommaOp, glob, $5))); };
	  | Type VariableDeclId SEMInum
	    {$$=MakeTree(DeclOp, NullExp(), MakeTree(CommaOp, $2, MakeTree(CommaOp, glob, NullExp()))); };
          | Type VariableDeclId EQUALnum VariableInitializer SEMInum
	    {$$=MakeTree(DeclOp, NullExp(), MakeTree(CommaOp, $2, MakeTree(CommaOp, glob, $4))); };

VDList : VDList VariableDeclId COMMAnum  
	    {$$=MakeTree(DeclOp, $1, MakeTree(CommaOp, $2, MakeTree(CommaOp, glob, NullExp())));};
       | VariableDeclId COMMAnum 
	    {$$=MakeTree(DeclOp, NullExp(), MakeTree(CommaOp, $1, MakeTree(CommaOp, glob, NullExp())));};
       | VDList VariableDeclId EQUALnum VariableInitializer COMMAnum  
	    {$$=MakeTree(DeclOp, $1, MakeTree(CommaOp, $2, MakeTree(CommaOp, glob, $4)));};
       | VariableDeclId EQUALnum VariableInitializer COMMAnum
	    {$$=MakeTree(DeclOp, NullExp(), MakeTree(CommaOp, $1, MakeTree(CommaOp, glob, $3)));};

VariableDeclId : IDnum
		{$$=MakeLeaf(IDNode, $1);};
	       | IDnum BRACList
		{$$=ArrMakeLeaf(IDNode, $1);};

BRACList : LBRACnum RBRACnum BRACList 
	   {$$=MakeTree(IndexOp, NullExp(), $3);}
         | LBRACnum RBRACnum 
	   {$$=MakeTree(IndexOp,NullExp(), NullExp());}	

TBList : LBRACnum RBRACnum TBList  
	   {$$=MakeTree(IndexOp, NullExp(), $3);}
         | DOTnum Type
	   {$$=MakeTree(FieldOp, $2, NullExp());}; 

VariableInitializer : Expression
	    	      {$$=$1;};
		    | ArrayInitializer
		      {$$=$1;};
		    | ArrayCreationExpression
		      {$$=$1;};

Expression : SimpleExpression 
	     {$$=$1;};
	   | SimpleExpression NEnum SimpleExpression
	     {$$=MakeTree(NEOp, $1, $3);};
	   | SimpleExpression GEnum SimpleExpression
	     {$$=MakeTree(GEOp, $1, $3);};
	   | SimpleExpression GTnum SimpleExpression
	     {$$=MakeTree(GTOp, $1, $3);};
	   | SimpleExpression LTnum SimpleExpression
	     {$$=MakeTree(LTOp, $1, $3);};
	   | SimpleExpression EQnum SimpleExpression
	     {$$=MakeTree(EQOp, $1, $3);};
	   | SimpleExpression LEnum SimpleExpression
	     {$$=MakeTree(LEOp, $1, $3);};

SimpleExpression : PLUSnum TermList
		   {$$=$2;};
		 | TermList
		   {$$=$1;};
		 | MINUSnum TermList
                   {$$=MakeTree(UnaryNegOp,$2,NullExp());};
Term : Factor 
       {$$=$1;};
     | Factor TIMESnum Term
       {$$=MakeTree(MultOp, $3, $1);};
     | Factor DIVIDEnum Term
       {$$=MakeTree(DivOp, $3, $1);};
     | Factor ANDnum Term
       {$$=MakeTree(AndOp, $3, $1);};

TermList : Term
           {$$=$1;};
//         | Term PLUSnum TermList
         | TermList PLUSnum Term
           {$$=MakeTree(AddOp, $1, $3);};
//         | Term MINUSnum TermList
         | TermList MINUSnum Term
           {$$=MakeTree(SubOp, $1, $3);};
//         | Term ORnum TermList
         | TermList ORnum Term
           {$$=MakeTree(OrOp, $1, $3);}; //<--fixed this again, somehow it had gone back to being "AssignOp" instead of "OrOp"

Factor : UnsignedConstant
	 	 {$$=$1;};
       | Variable
	 	 {$$=$1;};
       | MethodCallStatement
         {$$=$1;};
       | LPARENnum Expression RPARENnum
         {$$=$2;};
       | NOTnum Factor
         {$$=$2;};

UnsignedConstant : ICONSTnum
                   {$$=MakeLeaf(NUMNode, $1);};
                   | SCONSTnum
		           {$$=MakeLeaf(STRINGNode, $1);};

Variable : IDnum 
	       {$$=MakeTree(VarOp, MakeLeaf(IDNode, $1), NullExp());};
         | IDnum SelectList
	       {$$=MakeTree(VarOp, MakeLeaf(IDNode, $1), $2);};

SelectList : DOTnum IDnum
             {$$=MakeTree(SelectOp, MakeTree(FieldOp, MakeLeaf(IDNode, $2), NullExp()), NullExp());};
	       | DOTnum IDnum SelectList
             {$$=MakeTree(SelectOp, MakeTree(FieldOp, MakeLeaf(IDNode, $2), NullExp()), $3);};
           | LBRACnum Expression RBRACnum 
		     {$$=MakeTree(SelectOp, MakeTree(IndexOp, $2, NullExp()), NullExp());};
           | LBRACnum Expression COMMAnum VExpressionList RBRACnum 
		     {$$=MakeTree(SelectOp, MakeTree(IndexOp, $2, $4), NullExp());};
           | LBRACnum Expression RBRACnum SelectList 
		     {$$=MakeTree(SelectOp, MakeTree(IndexOp, $2, NullExp()), $4);};
           | LBRACnum Expression COMMAnum VExpressionList RBRACnum SelectList 
		     {$$=MakeTree(SelectOp, MakeTree(IndexOp, $2, $4), $6);};

VExpressionList : Expression
		          {$$=MakeTree(IndexOp, $1, NullExp());};
               | Expression COMMAnum VExpressionList
		         {$$=MakeTree(IndexOp, $1, $3);};

MethodCallStatement : Variable LPARENnum RPARENnum
             {$$=MakeTree(RoutineCallOp, $1, NullExp());};

MethodCallStatement : Variable LPARENnum MCExpressionList RPARENnum
             {$$=MakeTree(RoutineCallOp, $1, $3);};

MCExpressionList : Expression
		   {$$=MakeTree(CommaOp, $1, NullExp());};
		 | Expression COMMAnum MCExpressionList 
		   {$$=MakeTree(CommaOp, $1, $3);};

ArrayInitializer : LBRACEnum VIList RBRACEnum
		   {$$=MakeTree(ArrayTypeOp, $2, glob);};

VIList : VariableInitializer
         {$$=MakeTree(CommaOp, NullExp(), $1);};
       | VariableInitializer COMMAnum VIList
	 {$$=MakeTree(CommaOp, $3, $1);};

ArrayCreationExpression : INTnum ACExpressionList
	 {$$=MakeTree(ArrayTypeOp, $2, MakeLeaf(INTEGERTNode, $1));};

ACExpressionList : LBRACnum Expression RBRACnum 
		 {$$=MakeTree(BoundOp, NullExp(), $2);};
	       | LBRACnum Expression RBRACnum ACExpressionList
		 {$$=MakeTree(BoundOp, $4, $2);};

Type : INTnum
	    {$$=glob=MakeTree(TypeIdOp, MakeLeaf(INTEGERTNode, $1), NullExp());};
     | INTnum BRACList 
	    {$$=glob=MakeTree(TypeIdOp, ArrMakeLeaf(INTEGERTNode, $1), $2);};
     | IDnum 
	    {$$=glob=MakeTree(TypeIdOp, MakeLeaf(IDNode, $1), NullExp());};
     | IDnum BRACList 
	    {$$=glob=MakeTree(TypeIdOp, ArrMakeLeaf(IDNode, $1), $2);};
     | IDnum TBList
	    {$$=glob=MakeTree(TypeIdOp, ArrMakeLeaf(IDNode, $1), $2);};

%%

int yycolumn, yyline;
#include "lex.yy.c"
yyerror(char *str){printf("Source: %s at line %d\n", str, yyline);}

