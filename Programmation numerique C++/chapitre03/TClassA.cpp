#include <iostream>
#include <cassert>
#include <cstdlib>

 using namespace std;

 typedef double K ;

 class A { public:  
    int n;        
    K *v;            
    A() 
      { cerr <<" Pas de constructeur pas défaut " << endl; exit(1);}

    A(int i) : n(i),v(new K[i])         
      { cout<<"constructeur       "<<this<<endl;assert(v);} 

    A(const A& a) :n(a.n),v(new K[a.n]) 
      { cout<<"constructeur/copie "<<this<<endl;operator=(a);}

    A& operator=(const A &a)            
      { cout<<"operateur  =    de "<<&a<<endl;
        assert(n==a.n);
        for(int i=0;i<n;i++) v[i]=a.v[i];return (*this);}

    ~A()                                
      { cout<<"destructeur        "<<this<<endl;
        delete [] v;} 

    K & operator [](int i) const  
      { assert(i>=0 && i <n);return v[i];}

   //A & operator + (const A &a) const ;

    A  operator + (const A &a) const ;

    A operator*(const K &a) const ;     

  private:             
    A(int i, K* p) : n(i),v(p)          
      { cout<<"constructeur/point "<<this<<endl; assert(v);}

    friend A operator*(const K& a,const A& ); 

};

//définition du flux de sortie pour l'affichage

ostream & operator << (ostream & f, const A & a)
{f<<"( ";for(int k=0;k<a.n;k++) f<<a[k]<<" ";f<<")"<<endl;return f;}

/*

//operateur + : version avec pointeurs

 A & A::operator+(const A & a) const 
{
  A * b = new A(n); assert(n == a.n);
  for (int i=0;i<n;i++) (*b)[i]= v[i]+a.v[i];
  return (*b);  // on retourne le pointeur évalué
 }



//operateur + : version avec copie au niveau de return

 A A::operator+(const A & a) const 
{
  A b(n); assert(n == a.n);
  for (int i=0;i<n;i++) b.v[i]= v[i]+a.v[i];
  return b;  // ici le constructeur  A(const A& a) est appelé
 }

*/





//operateur + : version optimisée sans copie

 A A::operator+(const A & a) const 
{
  K  *b=new K[n]; assert(n == a.n);
  for (int i=0;i<n;i++) b[i]= v[i]+a.v[i];
  return A(n,b);  // ici le constructeur  A(n,K*) ne fait pas de copie
 }



//operateur * : multiplication à droite

 A A::operator*(const K & a) const 
{
  K  *b = new K[n];
  for (int i=0;i<n;i++) b[i]= v[i]*a;
  return A(n,b);  // ici le constructeur  A(n,K*)  ne fait pas de copie
 } 



//operateur * : multiplication à gauche

 A operator*(const K & a,const A & c) 
{
  K  *b=new K[c.n]; 
  for (int i=0;i< c.n;i++) b[i]= c[i]*a;
  return A(c.n,b);
}



int main()
{    int m=4;

      cout<<"=======definition de z1======"<<endl;

      A z1(m);for(int k=0;k<z1.n;k++) z1[k]=(K)k;
      cout << "z1="<<z1;  

      cout<<"======definition de z2======"<<endl;

      A z2(z1);  
      cout << "z2="<< z2;

      cout<<"======definition de z3======"<<endl;  

      A z3(m);

      cout<<"======debut z3=(z1+z2)======"<<endl;

      z3=(z1+z2);  cout<< "z3="<< z3; 

      cout<<"======fin   z3=(z1+z2)======"<<endl;

      cout<<"======debut z3=z1*2.======"<<endl;

      z3=z1*2.;    cout<< "z3="<< z3; 

      cout<<"======fin   z3=z1*2.======"<<endl;

      cout<<"======debut z3=3.*z2======"<<endl;

      z3=3.*z2;    cout<< "z3="<< z3; 

      cout<<"======fin   z3=3.*z2======"<<endl;

      cout<<"======debut z3=(z1+2.*z2)+z3*2.======"<<endl;

      z3=(z1+2.*z2)+z3*2.;   cout<< "z3="<< z3; 

      cout<<"======fin   z3=(z1+2.*z2)+z3*2.======"<<endl;

      return 0;
}
