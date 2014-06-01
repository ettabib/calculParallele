#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
void InternalError(const char * str);
#include "RNM.hpp"
#include "GC.hpp"
#include "gmres.hpp"
#include "sfem.hpp"
#include "sfemmat.hpp"
using namespace std;
typedef double R;
typedef KN<R> Rn;


//  une petite classe pour faire le lien  entre langague et programe
class Link { public:
    Mesh * pTh; 
    R2 P;
    int index;
    void set(int i){assert(pTh); index=i;P=(*pTh)(i);}
    void unset() { index=-1;P.x=1e100,P.y=1e100;}
    int i() const {
          assert(pTh && index >=0 && index < pTh->nv);
          return index;}
};


R kaSolve(const Triangle & K) ; 

extern Link globals; // la variable global 

static R Add(R a,R b) { return a+b;} 
static R Comma(R a,R b) { return a,b;} 
static R Mul(R a,R b) { return a*b;} 
static R Div(R a,R b) { return a/b;} 
static R Sub(R a,R b) { return a-b;} 
static R Neg(R a)     { return -a;} 
static R Print(R a)     { cout << a ; ; return a;} 
static R Print(const string &s) {cout << s; return 0;}

class Exp { public:
  
  // la classe qui modelise une instruction   
  class ExpBase{ public:    virtual R operator()() const = 0;
    virtual ExpBase * set(const ExpBase *) const {return 0;}
  };
  
  // un classe pour defini les constantes double    
  class ExpConst : public ExpBase {public:
    R a;
    ExpConst(R aa) : a(aa) {}
    R operator()() const {return a;}};
  
  // une classe pour definir un variable double
  class ExpPtr : public  ExpBase {public:
    // la class pour affecter la variable double (cf. set )
    class SetExpPtr  : public  ExpBase  {public:
      R *a;      
      const ExpBase *e;
      R operator()() const {return *a=(*e)();} // set
      SetExpPtr(R * aa, const ExpBase *ee) : a(aa),e(ee) {}};
    
    R *a;
    ExpPtr(R *aa) : a(aa) {}
    R operator()() const {return *a;}
    virtual ExpBase * set(const ExpBase * e) const {return new SetExpPtr(a,e);}};
  
  //  une class pour definir les operateurs binaire via un fonction R (*) (R,R)  
  class ExpOp2 : public ExpBase {public:
    typedef  R (*func)(R,R); 
    const ExpBase &a,&b; func op;
    ExpOp2(func o,const ExpBase &aa,const ExpBase &bb ) : a(aa),b(bb),op(o) {assert(&a && &b);}
    R operator()() const   // // on passe par de variable pour force l'ordre d'évaluation
    {R aa=a(),bb=b(); return (*op)(aa,bb);}};  // car  f(a(),b()) peut appel b() puis a()
  
  //  une class pour definir les operateurs binaire via un fonction
  // R (*) (const ExpBase & a,const ExpBase & b) que l'on utilisera
  // pour faire un while qui fait:  while (a()) a();
  class ExpOp2_ : public ExpBase {public:
    typedef  R (*func)(const ExpBase &,const ExpBase &); 
    const ExpBase &a,&b;func op;
    ExpOp2_(func o,const ExpBase &aa,const ExpBase &bb ) : a(aa),b(bb),op(o) {assert(&a && &b);}
    R operator()() const {return op(a,b);}};
  
  // une classe  pour definir des operateur unaire
  class ExpOp : public ExpBase {public:
    const ExpBase &a; R (*op)(R);
    ExpOp(R (*o)(R),const ExpBase &aa) : a(aa),op(o) {assert(&a);}
    R operator()() const {return op(a());}};
  
  // une class pour definir des operators sur des chaines de caractères
  class ExpPStringOp : public  ExpBase {public:
    const string *p;
    R (*op)(const string &);
    ExpPStringOp(R (*o)(const string &),const string *pp) :op(o),p(pp){}
    R operator()() const { op( *p) ; return 0;}
  };
  
  //  pour definir un solution  u = x*un;
  class ExpSetSolution :  public  ExpBase {public:
    Rn & s;
    const ExpBase &a;
    ExpSetSolution(Rn & ss,const ExpBase &aa) : s(ss),a(aa) {}
    R operator()() const ;
  };
  
  // evalutation d'un solution avec  index
  class ExpVSolution: public  ExpBase {public:
    Rn &  s;
    ExpVSolution( Rn & ss) : s(ss) {}
    R operator()() const {
      assert( !s.unset() ) ; return s[globals.i()];}
  };
  
  class ExpSolve : public  ExpBase {public:
    Rn & s; // the solution
    const ExpBase &a; // \int TH  w_i w_j
    const ExpBase &b; // \int grad w_i grad w_j
    const ExpBase &c; //  
    const ExpBase &rhs; // rhs*w_j
    const ExpBase &g; //   condition au limite dirichket
    const ExpBase &gn; //   condition au limite neuman
    
    const ExpBase & don;   // label dirichlet
    const ExpBase & ron;  // label Robin
    const ExpBase & non;  // label  neuman 
    ExpSolve( Rn & ss,const ExpBase &aa, const ExpBase &bb, const ExpBase &cc,const ExpBase &r,
	      const  ExpBase & onn , const ExpBase & ronn,const ExpBase & nonn, const  ExpBase & gg ,
	      const ExpBase & ggn)
      : s(ss),a(aa),b(bb),c(cc),rhs(r),don(onn),ron(ronn),non(nonn),g(gg),gn(ggn)  {}
    
    static R & Value(const Triangle &K) {
      static R  ka;
      return ka;};
    R operator()() const ;    
  };
  
  // la classe qui encapsule toutes les pointeurs sur ExpBase
  const ExpBase * f;
  
  Exp() : f(0) {} 
  Exp(const ExpBase *ff) : f(ff) {}
  Exp(R a)  : f(  new ExpConst(a)) {}
  Exp(R* a) :f( new ExpPtr(a)) {}
  R operator()() const {return (*f)();}
  operator const ExpBase * () const  {return f;}
  operator const ExpBase & () const  {return *f;}
  
  Exp operator=(const Exp & a) const { return f->set(a.f);}    
  Exp operator,(const Exp & a) const { return new ExpOp2(Comma,*f,*a.f);}
  Exp operator+(const Exp & a) const { return new ExpOp2(Add,*f,*a.f);}
  Exp operator-(const Exp & a) const { return new ExpOp2(Sub,*f,*a.f);}
  Exp operator-() const              { return new ExpOp(Neg,*f);}
  Exp operator+() const              { return *this;}
  Exp operator/(const Exp & a) const { return new ExpOp2(Div,*f,*a.f);}
  Exp operator*(const Exp & a) const { return new ExpOp2(Mul,*f,*a.f);}
  Exp comp(R (*ff)(R )){  return new ExpOp(ff,*f);}
  Exp comp(R (*ff)(R,R ),const Exp & a){  return new ExpOp2(ff,*f,*a.f);}
};
// pour definir une expression qui ne fait rien ou qui retourne 0
inline Exp zero() { return new Exp::ExpConst(0);}
inline Exp nombre(R a) { return new Exp::ExpConst(a);}
// definir les 3 operateurs de compositions 
inline Exp comp(R (*ff)(R ),const Exp & a){  return new Exp::ExpOp(ff,*a.f);}
inline Exp comp(R (*ff)(R,R ),const Exp & a,const Exp & b){  return new Exp::ExpOp2(ff,*a.f,*b.f);}
inline Exp comp(Exp::ExpOp2_::func ff,const Exp & a,const Exp & b){  return new Exp::ExpOp2_(ff,*a.f,*b.f);}

// pour definir les instruction d'impression
inline Exp print(const  Exp & a)  { return new Exp::ExpOp(Print,*a.f);}
inline Exp print(const string * p)  { return new Exp::ExpPStringOp(Print,p);}

inline Exp setRn(Rn  * s,const  Exp & a) { return new Exp::ExpSetSolution(*s,*a.f);}
inline Exp valueRn(Rn  * s) { return new Exp::ExpVSolution(*s);}

inline R Readmesh(const string & p) { globals.pTh=new Mesh(p.c_str()); return 0;}
inline Exp readmesh(const string * p)  { return new Exp::ExpPStringOp(Readmesh,p);}
inline R kaSolve(const Triangle & K)  { return Exp::ExpSolve::Value(K) ; }

// la function while
inline R While(const  Exp::ExpBase & a,const  Exp::ExpBase & b) {
  R r=0;
  while( a() ) { r=b();}
  return r;
}
