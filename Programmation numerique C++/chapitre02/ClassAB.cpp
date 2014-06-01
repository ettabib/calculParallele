#include<iostream>

using namespace std;

class A
{public: 
  int i;
  A(int n=0): i(n)
     {cout <<"constr A - adresse "<<this<<endl;}

 ~A(){cout <<" destr A - adresse "<<this<<endl;}

  void Affiche(){cout <<"i="<<i<<endl;}
};

class B : public A    // syntaxe de l'héritage
{public: 
  int j;
  B(int n=0, int m=0):A(n),j(m)
     {cout <<"constr B - adresse "<<this<<endl;}

 ~B(){cout <<" destr B - adresse "<<this<<endl;}

  void Affiche(){A::Affiche();cout<<"j="<<j<<endl;}
};

int main()
{   
    A a; a.Affiche(); 
       cout<<"---------------"<<endl;

    B b; b.Affiche(); 
       cout<<"---------------"<<endl;   

    A * pa;
    pa=new A(5)  ;pa->Affiche();delete pa;
       cout<<"---------------"<<endl;

    pa=new B(2,5);pa->Affiche();delete pa;
       cout<<"---------------"<<endl;
}




