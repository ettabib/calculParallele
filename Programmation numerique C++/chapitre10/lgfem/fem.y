%{
#include <iostream>
#include <fstream>
#include  <complex>
#include <string>
#include <cassert>
using namespace std;
#ifdef __MWERKS__
#include "alloca.h"
#endif
#include "expr.hpp"
#include "table.hpp"
Link globals;
    
istream *ccin;
 Rn * u_current;
inline void yyerror(const char * msg)
{  cerr << msg << endl;
   exit(1);}

int yylex(); 
%}

%union{ 
 double dnum;
 string * str;
 const  Exp::ExpBase * exp;
 R (*f1)(R);
 R (*f2)(R,R);
 Rn * u; 
 void * p;
}

%token WHILE
%token PRINT
%token SOLUTION
%token MESH
%token <u> RN
%token GRAD
%token DIV
%token SOLVE
%token BIDON

%token <dnum> DNUM
%token <str> NEW_ID
%token <str> STRING
%token <exp> ID
%token <f1> FUNC1
%token <f2> FUNC2
%token ENDOFFILE
%type  <exp> divgrad
%type  <exp> mass
%type  <u> solve
%type  <u> u
%type  <exp> code
%type  <exp> parg
%type  <exp> instr
%type  <exp> expr
%type  <exp> start
%type  <exp> unary_expr
%type  <exp> pow_expr
%type  <exp> primary
%left  <exp> ','
%left  <exp>  '+' '-'
%left  <exp> '*' '/'  '%'
%right <exp>  '^' 

%%
start:   code  ENDOFFILE { 
   cout << "------- exec ------- \n" ;
  (*$1)() ; 
  cout << endl;
  return 0; }               
;

code : instr 
      | code instr { $$=(Exp($1),$2);} 
;

instr:  NEW_ID '=' expr ';' { $$=(Exp(tableId.Add(*$1,Exp(new R)))=$3)}
     | SOLUTION NEW_ID ';' { tableId.Add(*$2,new Rn);  $$=zero();}
     | RN '=' expr ';' {  $$=setRn($1,$3);}
     | ID '=' expr ';' { $$ = (Exp($1)=$3); }
     | PRINT parg ';' {$$=$2;}
     | WHILE '(' expr ')' '{' code '}' { $$ = comp(While,Exp($3),Exp($6));}
     | MESH STRING ';' {$$=readmesh($2);}
     | solve  mass '+' divgrad '=' expr ';' {
        $$=new Exp::ExpSolve(*$1,*$2,*$4,zero(),*$6,
                             nombre(1.0),nombre(4.0),nombre(3.0),
                             nombre(0.0),nombre(0.0));
        u_current=0;}
     | solve  mass '+' divgrad '=' question3 ';' {
        assert(0); // votre action a faire

        u_current=0;}

;

u: RN   {$$=$1;assert($$==u_current);};

question3: DNUM u '^' DNUM {
  assert($4==2.);
  assert($1==12.); };




solve: SOLVE '(' RN ')' { u_current=$$=$3;};

mass:  expr u {$$=$1;};




divgrad:  DIV  expr  GRAD  u  { $$=$2;}
        | DIV    GRAD  u  { $$=nombre(1);};


parg:   expr { $$=print($1) ;}
      | STRING { $$=print($1) ;}
      |  parg ',' parg {$$=(Exp($1),$3);}
;

expr: unary_expr 
    | expr '+' expr {$$ = Exp($1)+$3;}
    | expr '-' expr {$$ = Exp($1)-Exp($3);}
    | expr '*' expr {$$ = Exp($1)*$3;}
    | expr '/' expr {$$ = Exp($1)/$3;}
;     

unary_expr:   pow_expr    
  | '-'  pow_expr {$$=-Exp($2);}
  | '+'  pow_expr {$$=$2;}
;

pow_expr: primary
     |      primary  '^' unary_expr   {$$=comp(pow,$1,$3);} 
;

primary:   DNUM {$$=Exp($1);}
     | '(' expr ')' { $$=$2;} 
     |  ID { $$=$1;}
     |  FUNC1 '('expr ')' { $$=comp($1,$3);}
     |  FUNC2 '('expr ',' expr ')' { $$=comp($1,$3,$5);}
     |  RN { $$= valueRn($1);}

;
     
%%
#include "lex.hpp"

pair<int,void*> TableOfIdentifier::Find(const string & id) const
{
maptype::const_iterator i=m.find(id);
    if (i == m.end()) return make_pair<int,void*>(0,0);
    else return i->second;
}

void * TableOfIdentifier::Add(const string & id, R (*f)(R))
{ return Add(id,FUNC1,(void*)f);}

void * TableOfIdentifier::Add(const string & id, R (*f)(R,R))
{ return Add(id,FUNC2,(void*)f);}

void * TableOfIdentifier::Add(const string & id, Rn * p)
{ return Add(id,RN,(void*) p);}

const Exp::ExpBase * TableOfIdentifier::Add(const string & id,const Exp::ExpBase * f)
{  Add(id,ID,(void*)f); return f;}


int main (int  argc, char **argv)
{
    ccin = & cin;
 if(argc>1) ccin = new ifstream(argv[1]);
 if(!ccin) { cerr << " err opinging " << argv[1] <<endl;exit(1);}
 
// yydebug = 1;

  globals.pTh=0;
  globals.unset();
  tableId.Add("x",Exp(&globals.P.x));
  tableId.Add("y",Exp(&globals.P.y));
  tableId.Add("cos",cos);
  tableId.Add("atan",atan);
  tableId.Add("end",ENDOFFILE);
  tableId.Add("while",WHILE);
  tableId.Add("print",PRINT);
  tableId.Add("solution",SOLUTION);
  tableId.Add("mesh",MESH);
  tableId.Add("solve",SOLVE);
  tableId.Add("div",DIV);
  tableId.Add("grad",GRAD);
  tableId.Add("atan2",atan2); 
  cout << " input : \n";
  yyparse();
  cout << "\n fin normal \n";
  return 0;
}
