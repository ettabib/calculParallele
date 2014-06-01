#include <fstream>
#include <cassert>
#include <cmath> 

#define CHECK_KN
#include "RNM.hpp"

using namespace std;  

// fonction necessaire pour la compatibilité avec RNM
void InternalError(const char * str) {cerr << str; exit(1);}


typedef double R;

class MatriceMorseSymetrique : VirtualMatrice<R> 
{ public:

  int n, nbcoeff;
  KN<int> ligne, colonne;
  KN<R> aa;

         // constructeur à partir du tableau A
  MatriceMorseSymetrique(const KNM_<R> & A,R tol=1.e-30);

         // opérateurs 
  void addMatMul(const KN_<R> & x, KN_<R> & Ax) const ;  
  plusAx operator*(const KN<R> &  x) const {return plusAx(this,x);} 

};

MatriceMorseSymetrique::MatriceMorseSymetrique (const KNM_<R> & A,R tol)
{n=A.N();nbcoeff=0;

 // tout d'abord, il faut évaluer nbcoeff

 for(int i=0;i<n;i++)
    for(int j=0;j<i+1;j++)
      if(fabs(A(i,j))>tol) nbcoeff++;

 // allocation des vecteurs

       ligne.set(new int[n+1],     n+1);
     colonne.set(new int[nbcoeff], nbcoeff);
          aa.set(new   R[nbcoeff], nbcoeff);

 // stockage morse

	  nbcoeff=0;
 for(int i=0;i<n;i++)     //balayage suivant les lignes i
   {ligne[i]=nbcoeff;
    for(int j=0;j<i+1;j++)//partie triang inf 
      if(fabs(A(i,j))>tol) 
        {aa[nbcoeff]=A(i,j);colonne[nbcoeff]=j;nbcoeff++;}
   }

 ligne[n]=nbcoeff;
}


 void  MatriceMorseSymetrique::addMatMul(const KN_<R> & x, KN_<R> & Ax) const 
{  
  assert(n==x.size());

  for(int i=0;i<n;i++) // balayage des lignes
    for(int k=ligne[i];k<ligne[i+1];k++)
      {int j=colonne[k];
      if(i !=j) 
        {Ax[i] += aa[k]*x[j];  // contrib partie triang inf
         Ax[j] += aa[k]*x[i];} // contrib partie symétrique
       else
         Ax[i] +=aa[k]*x[j];   // contrib diagonale
      }
}

int main( ) 
{ int n=6;

  cout<<"======Construction de la matrice -> A"<<endl;

  KNM<R>  A(n,n);A=0;

  A(0,0)=0.1;A(1,1)=1;A(2,1)=A(1,2)=2;A(2,2)=3;

  A(3,1)=A(1,3)=4;A(3,2)=A(2,3)=5;A(3,3)=6;

  A(4,2)=A(2,4)=7;A(4,4)=8;

  A(5,3)=A(3,5)=9;A(5,4)=A(4,5)=10;A(5,5)=11;

  cout<<A<<endl; 



  cout<<"======Stockage morse -> M"<<endl;

  MatriceMorseSymetrique M(A);

  cout<<"aa="<<M.aa<<endl;

  cout<<"colonne="<<M.colonne<<endl;

  cout<<"ligne="<<M.ligne<<endl;



  cout<<"======Test du produit A*x"<<endl;

  KN<R> x(n),b(n);x=1.;

 

  b=0;b=A*x;cout<<"A*x="<<b<<endl;

  b=0;b=M*x;cout<<"M*x="<<b<<endl;

  return 0;
}
