#include<iostream>
using namespace std;

class A
{public: 
  int i;
  A(int n=0): i(n)
              {cout <<"constr A - adresse "<<this<<endl;}

  virtual ~A(){cout <<" destr A - adresse "<<this<<endl;}

  virtual void Affiche() {cout <<"i="<<i<<endl;}

          void SAffiche(){cout<<"****"<<endl;
                          Affiche();
                          cout<<"****"<<endl;}
};

class B : public A
{public: 
  int j;
  B(int n=0, int m=0):A(n),j(m)
     {cout <<"constr B - adresse "<<this<<endl;}

 ~B(){cout <<" destr B - adresse "<<this<<endl;}

  virtual void Affiche(){A::Affiche();cout<<"j="<<j<<endl;}
};

class C : public B
{public: 
  int k;
};

int main()
{   A * pa;
    pa=new A(5)  ;pa->Affiche();delete pa;
       cout<<"---------------"<<endl;

    pa=new B(2,5);pa->Affiche();delete pa;
       cout<<"---------------"<<endl;

    pa=new C;pa->Affiche();
       cout<<"---------------"<<endl;

             pa->SAffiche();delete pa;
       cout<<"---------------"<<endl;

       return 0;
}