#include <iostream>
 // pour la fonction pow
#include <cmath>
 typedef double R;
 class Cvirt { public: virtual R operator()(R ) const =0;};

 class Cfonc : public Cvirt { public:
   R (*f)(R);
   R operator()(R x) const { return (*f)(x);}
   Cfonc( R (*ff)(R)) : f(ff) {} };

 class Cconst : public Cvirt { public:
   R a;
   R operator()(R ) const { return a;}
   Cconst( R aa) : a(aa) {}         };

 class Coper : public Cvirt { public:
   const  Cvirt *g, *d;
   R (*op)(R,R);
   R operator()(R x) const { return (*op)((*g)(x),(*d)(x));}
   Coper( R (*opp)(R,R), const Cvirt *gg, const Cvirt *dd)
         : op(opp),g(gg),d(dd) {}
   ~Coper(){delete g,delete d;}    };

//  les 5 opérateur binaire
 static  R Add(R a,R b) {return a+b;}
 static  R Sub(R a,R b) {return a-b;}
 static  R Mul(R a,R b) {return a*b;}
 static  R Div(R a,R b) {return a/b;}
 static  R Pow(R a,R b) {return pow(a,b);}

 class Fonction { public:
   const Cvirt *f;
   R operator()(const R x){ return (*f)(x);}
   Fonction(const Cvirt *ff) : f(ff) {}
   Fonction(R (*ff)(R)) : f(new Cfonc(ff)) {}
   Fonction(R a) : f(new Cconst(a)) {}
   operator const  Cvirt * () const {return f;}
   Fonction operator+(const Fonction & dd) const
        {return new Coper(Add,f,dd.f);}
   Fonction operator-(const Fonction & dd) const
        {return new Coper(Sub,f,dd.f);}
   Fonction operator*(const Fonction & dd) const
        {return new Coper(Mul,f,dd.f);}
   Fonction operator/(const Fonction & dd) const
        {return new Coper(Div,f,dd.f);}
   Fonction operator^(const Fonction & dd) const
        {return new Coper(Pow,f,dd.f);}
 };

using namespace std;  //introduces namespace std

R Identite(R x){ return x;}
int main()
{
  Fonction Cos(cos),Sin(sin),x(Identite);
  Fonction f((Cos^2)+Sin*Sin+(x^4)); // attention  ^ n'est prioritaire
  cout << f(2) << endl;
  cout << ((Cos^2)+Sin*Sin+(x^4))(1) << endl;
  return 0;
}
