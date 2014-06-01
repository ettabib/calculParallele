%{

#include <iostream>
#include <cassert>
#include <cctype>
#include <cstdlib>
using namespace std;

inline void yyerror(const char * msg)
{  cerr << msg << endl;
   exit(1);}

int yylex(); 
%}


%union{ 
 double dnum;
 char oper;
}

%token <dnum> DNUM
%token ENDOFFILE
%type  <dnum> expr
%type  <dnum> start
%type  <dnum> primary
%left  <oper>  '+' '-'
%left  <oper> '*' '/'  

%%

start:  expr { cout << " exp = " <<  $1 <<endl}
      | start  ',' expr { cout  << " exp = " <<  $3 <<endl}
      | start  ENDOFFILE { cout << "fin " << endl; return 0;}
      
;

expr: primary 
    | expr '+' expr {$$ = $1+$3}
    | expr '-' expr {$$ = $1-$3}
    | expr '*' expr {$$ = $1*$3}
    | expr '/' expr {$$ = $1/$3}
;     


primary:   DNUM 
     | '(' expr ')' { $$=$2}        
;
     
%%

int yylex() {
  L1:
   int c=cin.peek(); 
   if (cin.eof()) {return ENDOFFILE;}  
   if (isdigit(c) || c == '.') {  cin >> yylval.dnum; assert(cin.eof() || cin.good()); return DNUM;}
   else if (c==',' || c=='+' || c=='*' || c=='(' || c==')'  || c==EOF) return yylval.oper=cin.get();
   else if (isspace(c)) {cin.get();goto L1;}
   else {  cerr << " caractere invalide " << char(c) << " " << c << endl; exit(1);}
  };

int main (int  argc, char **argv)
{
 // yydebug = 1;
  yyparse();
  return 0;
}
