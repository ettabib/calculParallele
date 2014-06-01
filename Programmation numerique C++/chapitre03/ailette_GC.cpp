#include <fstream>
#include <cassert>
#include <cmath> 

#define CHECK_KN
#include "RNM.hpp"
#include "GC.hpp"

using namespace std;

// fonction necessaire pour la compatibilité avec RNM
void InternalError(const char * str) {cerr << str; exit(1);}

typedef double R;
typedef double R;
typedef KN<R> Rn;
typedef KN_<R> Rn_;
typedef KNM<R> Rnm;
typedef KNM_<R> Rnm_;

      double L=40.,a1=4.,a2=50.;
      double To=46., Te=20.;
      double k=0.164, hc=1.e-6*200.;
      double act=2.*(a1+a2)*hc*L*L/(k*a1*a2);

double Solexacte(R x) 
{ return  (To-Te)/Te*cosh(sqrt(act)*(1-x))/cosh(sqrt(act));}

void Verif(Rn_ & xnum,char * filename)
{
  //Sauvegarde du fichier gnuplot

  int M=xnum.size();double h=1./M;

  ofstream fic(filename);
       fic<< 0.<<"\t"<< (To-Te)/Te <<"\t"<< (To-Te)/Te<<endl;        

     for(int m = 0; m< M; m++)    
       fic<<(m+1)*h<<"\t"<<xnum[m]<<"\t"<<Solexacte((m+1)*h)<<endl;

  fic.close();  cout<<"fichier ecrit  " <<filename<<endl;
}


// Classes pour la méthode 2 (sans construction de la matrice)


class Matrice_ailette_1D: VirtualMatrice<R> 
{ public:
  typedef  VirtualMatrice<R>::plusAx plusAx;

  Matrice_ailette_1D() {}; 

  void addMatMul(const KN_<R> & x, KN_<R> & Ax) const ;  

  plusAx operator*(const KN<R> &  x) const {return plusAx(this,x);} 

}; 

 
 void  Matrice_ailette_1D::addMatMul(const KN_<R> & x, KN_<R> & Ax) const 
{  
  int M= x.size();double h=1./M;
  double adiag=2.+act*h*h;

  for(int m=1;m<M-1;m++)
    Ax[m] += -x[m-1]+adiag*x[m]-x[m+1];

  Ax[0]   += adiag*x[0]-x[1];
  Ax[M-1] += -x[M-2]+x[M-1];
}

int main( ) 
{
  int M;
     cout << "Dimension du maillage "<<endl;
     cin >> M;

{ 
  cout<<"Méthode 1: Construction de la matrice du système"<<endl;

  Rnm   A(M,M);A=0;
  Rn_   Ap (A,SubArray(M  , 0 ,M+1)); //diagonale principale
  Rn_   Asi(A,SubArray(M-1, 1 ,M+1)); //diagonale secondaire inf
  Rn_   Ass(A,SubArray(M-1, M, M+1)); //diagonale secondaire sup

  double h=1./M;

  Ap=2+h*h*act;Ap[M-1]=1.;
  Asi=-1;
  Ass=-1;

  //cout<<A<<endl;

  // Construction du second membre

  Rn b(M);
  b=0.; b[0]=(To-Te)/Te;

  // Solution numérique 

  Rn xnum(M);

  cout<<"==gc sans preconditionnement=="<<endl;
  xnum=b;
  GradienConjugue(A,MatriceIdentite<R>(),b,xnum,M,1.e-10);
  Verif(xnum,"verif1.dat");

  cout<<"==gc conditionne par l'inverse de la diagonale=="<<endl;
  xnum=b;
  Rnm C(M,M);C=0;
  Rn_ Cp(C,SubArray(M, 0, M+1));
  for(int i=0;i<M;i++) Cp[i]=1./Ap[i];

  GradienConjugue(A,C,b,xnum,M,1.e-10);
  Verif(xnum,"verif2.dat");               

  cout<<"====== fin methode 1 ================="<<endl;
}

{ 
  cout<<"Methode 2: sans construction de la matrice du systeme"<<endl; 

  // Construction du second membre

  Rn b(M);
  b=0.; b[0]=(To-Te)/Te;



  // Solution numérique 

  Rn xnum(M);xnum=b;



  GradienConjugue( Matrice_ailette_1D(),MatriceIdentite<R>(),b,xnum,M,1.e-10);
  Verif(xnum,"verif3.dat");

  cout<<"====== fin methode 2 ================="<<endl;
}

  return 0; 
}

