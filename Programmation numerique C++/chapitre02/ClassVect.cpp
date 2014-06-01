#include <iostream>
#include <cassert>

using namespace std;

template <class T>
class Vect
{public:
    int n;
    T *v;

    void Affiche(char* Message) const;

    Vect(int nn): n(nn), v(new T[n]){assert(v || !n);}
    Vect(const Vect & w);                         

   ~Vect(){if(v) delete [] v;}                   

    void    operator = (const Vect & a);          

    T & operator [] (int i) {assert(v && i>=0 && i < n);return v[i];}    
};

template <class T>
Vect<T>::Vect(const Vect &w):n(w.n),v(new T[n])
{   (*this)=w;  }   

template <class T>
void Vect<T>::operator = (const Vect & a)
{   assert(!n || (n==a.n && v && a.v) );
    for(int i=0;i<n;i++)
       v[i]=a.v[i];
}

template <class T>
void Vect<T>::Affiche(char * Message) const
{   cout<<Message<<endl;
    for(int i=0;i<n;i++)
      cout<<"Element i="<<i<<"\t"<<v[i]<<endl;
}

template <class U>
void InitVect(Vect<U> & v)
{  for(int i=0;i<v.n;i++)
     v[i]= (U)i/v.n;                       
}


typedef Vect<double> VectR;
int main()
{   
    Vect<double> w(3);
    VectR        u(3);

    InitVect(w);   w.Affiche("---- vecteur w ----"); 
                   u.Affiche("---- vecteur u ----"); 

    u=w; u[0]=10;  u.Affiche("---- vecteur u ----"); 

    Vect<int>    s(3);
    InitVect(s);   s.Affiche("---- vecteur s ----"); 


    return 0;
}