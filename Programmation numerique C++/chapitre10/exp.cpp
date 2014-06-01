#include <iostream>
#include <cassert>
#include <cctype>
using namespace std;
//  la grammaire : 
//   E = T  | T '+' E
//   T =  F | F '*' T     
//   F = c  | '(' E ')'
// ---------

class Exp {  public: 
  typedef double R; 
  int sym;
  static const int C=257;
  istream & cin;
  R valeur;
   
   Exp(istream & f) : cin(f)        // initialisation: 
     { NextSym();}                  //    lit le premier symbol 
   
   void Error(const char * message) // gestion des erreurs
    { cerr <<  message  << endl;
      exit(1);}
      
   void NextSym() {   // analyse lexical 
     int c=cin.peek();
     if (isdigit(c) || c == '.') 
       { sym=C; cin >> valeur; assert(cin.good());}
    else if (c=='+' || c=='*' || c=='(' || c==')'  || c==EOF) sym=cin.get();
    else if (isspace(c)) {sym=' '; cin.get();}
    else { 
        cerr << " caractere invalide " << char(c) << " " << c << endl;
        exit(1);}
  };
  
  void Match(int c) // assure que le  symbole courant est c
   { if(c!=sym) 
        { cerr << " On attendait le symbole " ;
          if ( c == C) cerr << " un nombre "  ;
          else cerr << c ; exit(1);}
     else NextSym();}
    
  bool IFSym(int c) // test is le symbole courant est c
    { if (c==sym) 
          { NextSym(); 
            return true;} 
      else return false;} 
  
  bool F(R & v) { // la valeur de l'expression est dans v
    
    if(IFSym(C)) {
          v=valeur;    
          cout << " F " << v <<endl;          
          return true;}
    else if (IFSym('(')) { 
          if (!E(v) ) 
          Match(')');    cout << " E " << v <<endl;          
          return true;}
    else return false;}

  bool T(R & v) { //  la valeur de l'expression est dans v
    if ( ! F(v) ) return false;
    else if (IFSym('*')) { 
              R vv;
              if (!T(vv) )  Error( " On attendait un term ");
              v*=vv; }
    cout << " T " << v <<endl;          
    return true;}

  bool E(R & v) { // la valeur de l'expression est dans v
    if ( !T(v) ) { 
          cout << " E false " << endl;
          return false;}
    else if (IFSym('+')) {
          R vv;
          if (!E(vv) ) Error(" On attendait un expression ") ;
          v+=vv;}
    cout << " E " << v <<endl;          
    return true;}
};

int main()
{
  Exp::R v;
  cerr << "Entrez une exp (ex faux  2*3+1 ) ? ";
  Exp exp(cin);
  while (true) {
      if (  exp.E(v) ) {
         cout << "exp  = " << v 
                           << "\n Autre exp : "; 
         exp.Match(' '); } // mange le blanc qui separe les expressions
      else  break; }
  cout << " On a fini sans erreur."<< endl;
  return 0;
}
