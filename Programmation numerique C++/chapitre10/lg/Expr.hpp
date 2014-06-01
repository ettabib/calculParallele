#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
using namespace std;
typedef double R;

  static R Add(R a,R b) { return a+b;} 
  static R Comma(R a,R b) { return a,b;} 
  static R Mul(R a,R b) { return a*b;} 
  static R Div(R a,R b) { return a/b;} 
  static R Sub(R a,R b) { return a-b;} 
  static R Neg(R a)     { return -a;} 
  static R Print(R a)     { cout << a ; ; return a;} 
  static R Print(const string &s) {cout << s; return 0;}
  
class Exp { public:
  class ExpBase{ public:    virtual R operator()() const = 0;
    virtual ExpBase * set(const ExpBase *) const {return 0;}
  };
  class ExpConst : public ExpBase {public:
    R a;
    ExpConst(R aa) : a(aa) {}
    R operator()() const {return a;}};
    
     
  class ExpPtr : public  ExpBase {public:
    class SetExpPtr  : public  ExpBase  {public:
      R *a;      
      const ExpBase *e;
      R operator()() const {return *a=(*e)();} // set
      SetExpPtr(R * aa, const ExpBase *ee) : a(aa),e(ee) {}};
    R *a;
    ExpPtr(R *aa) : a(aa) {}
    R operator()() const {return *a;}
    virtual ExpBase * set(const ExpBase * e) const 
       {return new SetExpPtr(a,e);}};
    
  class ExpOp2 : public ExpBase {public:
    typedef  R (*func)(R,R); 
    const ExpBase &a,&b; func op;
    ExpOp2(func o,const ExpBase &aa,const ExpBase &bb ) 
         : a(aa),b(bb),op(o) {assert(&a && &b);}
    R operator()() const {
      R aa=a(),bb=b(); // attention l'ordre d'appel des arg. est important 
      return op(aa,bb);}};
    
  class ExpOp2_ : public ExpBase {public:
    typedef  R (*func)(const ExpBase &,const ExpBase &); 
    const ExpBase &a,&b;func op;
    ExpOp2_(func o,const ExpBase &aa,const ExpBase &bb ) 
        : a(aa),b(bb),op(o) {assert(&a && &b);}
    R operator()() const {return op(a,b);}};
    
  class ExpOp : public ExpBase {public:
    const ExpBase &a; R (*op)(R);
    ExpOp(R (*o)(R),const ExpBase &aa) 
        : a(aa),op(o) {assert(&a);}
    R operator()() const {return op(a());}};

  class ExpPStringOp : public  ExpBase {public:
     const string *p;
     R (*op)(const string &);
     ExpPStringOp(R (*o)(const string &),const string *pp) 
         :op(o),p(pp){}
     R operator()() const { op( *p) ; return 0;}
     };
    
  
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
   
 Exp comp(R (*ff)(R ),const Exp & a)
       {  return new Exp::ExpOp(ff,*a.f);}
       
 Exp comp(R (*ff)(R,R ),const Exp & a,const Exp & b)
       {  return new Exp::ExpOp2(ff,*a.f,*b.f);}
       
 Exp comp(Exp::ExpOp2_::func ff,const Exp & a,const Exp & b)
       {  return new Exp::ExpOp2_(ff,*a.f,*b.f);}
       
 Exp print(const  Exp & a)  
       { return new Exp::ExpOp(Print,*a.f);}
       
 Exp print(const string * p)  
       { return new Exp::ExpPStringOp(Print,p);}
       
 R While(const  Exp::ExpBase & a,const  Exp::ExpBase & b) {
   R r=0;
   while( a() ) { r=b();}
   return r;
 }

//  ------ utilisation de la stl --------
class TableOfIdentifier {
  public:
  typedef  pair<int,void  *> value; // typeid + data 
  typedef map<string,value> maptype;
  typedef maptype::iterator iterator;
  typedef maptype::const_iterator const_iterator;
  maptype m;
  
  value Find(const string & id) const;  
  void * Add(const string & id,int i=0,void *p=0) 
    { m.insert(make_pair<string,value>(id,value(i,p))); return p;}
  void * Add(const string & id, R (*f)(R));
  const Exp::ExpBase * Add(const string & id,const Exp::ExpBase * f) ;
  
} tableId;

