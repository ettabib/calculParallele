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
#include "Expr.hpp"
istream *ccin;
inline void yyerror(const char * msg)
{  cerr << msg << endl; exit(1);}

int yylex(); 
%}

%union{ 
 double dnum;
 string * str;
 const  Exp::ExpBase * exp;
 R (*f1)(R);
 void * p;
}


%token WHILE
%token PRINT
%token <dnum> DNUM
%token <str> NEW_ID
%token <str> STRING
%token <exp> ID
%token <f1> FUNC1
%token ENDOFFILE
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
      
instr:  NEW_ID '=' expr ';'     { $$=(Exp(tableId.Add(*$1,Exp(new R)))=$3);}
     | ID '=' expr ';'          { $$ = (Exp($1)=$3); }
     | PRINT parg ';' {$$=$2;}
     | WHILE '(' expr ')' 
              '{' code '}'      { $$ = comp(While,Exp($3),Exp($6));}
;

parg:   expr           { $$=print($1) ;}
      | STRING         { $$=print($1) ;}
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
;
     
%%

int yylex() {
 L1:
  if (ccin->eof()) 
    return ENDOFFILE;
  int c=ccin->peek(); 
  if (c<0) return  ENDOFFILE;
  if (isdigit(c)) {
    *ccin >> yylval.dnum; 
    assert(ccin->eof() || ccin->good()); 
    cout << yylval.dnum;
    return DNUM;}
  else if (c=='.') {
    ccin->get(); c=ccin->peek(); ccin->putback('.'); 
    if (isdigit(c)) { 
      *ccin >> yylval.dnum; 
      assert(ccin->eof() || ccin->good()); 
      cout << yylval.dnum;
      return DNUM;}
    return ccin->get();}
  else if (c=='+' || c=='*' || c=='(' || c==')' 
	   ||   c=='-' || c=='/' || c=='{' || c=='}'  
	   ||   c=='=' || c=='^' || c==';' || c==',' || c=='.' ) { 
    cout << char(c) ; 
    return ccin->get(); }
  else if (c=='"')  {
    yylval.str = new string();
    ccin->get();
    while ( (c=ccin->peek()) != '"' && c != EOF)
      *yylval.str += char(ccin->get());
    assert(ccin->peek() == '"' );
    ccin->get();
    cout << '"'<< *yylval.str << '"' ;
    return STRING; }
  else if (isalpha(c)) {  
    yylval.str = new string();
    do {
      *yylval.str += char(ccin->get());
    } while (isalnum(ccin->peek()));
    cout << *yylval.str;
    pair<int,void*> p=tableId.Find(*yylval.str);
    if (p.first==0) return NEW_ID;  
    else {
      delete yylval.str; 
      yylval.p=p.second;
      return p.first;} }     
  else if (isspace(c)) {
    ccin->get(); cout << char(c) ;
    goto L1;}
  else {
    cerr << " caractere invalide " << char(c) << " " << c << endl;
    exit(1);}
};

pair<int,void*> TableOfIdentifier::Find(const string & id) const  {
  maptype::const_iterator i=m.find(id);
  if (i == m.end()) return make_pair<int,void*>(0,0);
  else return i->second;  
}

void * TableOfIdentifier::Add(const string & id, R (*f)(R))
{ return Add(id,FUNC1,(void*)f);}

const Exp::ExpBase * TableOfIdentifier::Add(const string & id,const Exp::ExpBase * f) 
{  Add(id,ID,(void*)f); return f;}

int main (int  argc, char **argv)
{
  ccin = & cin;
  if(argc>1) ccin = new ifstream(argv[1]);
  if(!ccin) { cerr << " error opening " << argv[1] <<endl;exit(1);}
  
  // yydebug = 1;
  R x,y;
  
  tableId.Add("x",Exp(&x));
  tableId.Add("y",Exp(&y));
  tableId.Add("cos",cos);
  tableId.Add("atan",atan);
  tableId.Add("end",ENDOFFILE);
  tableId.Add("while",WHILE);
  tableId.Add("print",PRINT);  
  cout << " input : \n";
  yyparse();
  cout << "\n fin normale \n";
  return 0;
}
