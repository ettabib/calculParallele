/* Calcul de la distribution de température
   dans une ailette de radiateur
   ********* calcul stationnaire*******************
   équation type (voir texte pour les dim physiques)

    -u''(x)+act*u(x)=0, 0<x<1
     u(0)=uo; u'(1)=0
     **********************************************
    résolution du système tridiagonal par 
         la méthode directe LU
     **********************************************
*/

#include <iostream>   
#include <fstream>    
#include <string>     
#include <cmath>      

using namespace std;  

      double L=40.,a1=4.,a2=50.;
      double To=46., Te=20.;
      double k=0.164, hc=1.e-6*200.;
      double act=2.*(a1+a2)*hc*L*L/(k*a1*a2);


int Compteur()     
{                  
  static int i=0;                     
  i++;                               
  return i;        
}                  


double Solexacte(double x) 
{
    return  (To-Te)/Te*cosh(sqrt(act)*(1-x))/cosh(sqrt(act));
}


int AfficheTab(int M, double *pTab1, double *pTab2, char *Message)
{
      cout << Message <<endl;
      cout << "Dimension du vecteur  = "<< M <<endl;

  for(int i=0;i<M;i++)
      cout<< "Element "<< i <<"\t"<<pTab1[i]<<"\t"<<pTab2[i]<<"\n";

    return 0;   
}


void SauveTab(int M, double *pTab1,  double *pTab2, 
              double *pTab3, char *NomFic) 
{
    ofstream fic(NomFic); 
     for(int m = 0; m< M; m++)
       fic<<pTab1[m]<<"\t"<<pTab2[m]<<"\t"<<pTab3[m]<<endl;

    fic.close();              
    cout<<"fichier ecrit : "<<NomFic<<endl;
}


void VectExacte(int M, double* pTab, double* pX, double (*pFunc)(double x)) 
{
    for(int m=0;m<=M;m++)
     pTab[m]=pFunc(pX[m]);
}

void Abscisses(int  M, double * pX) 
{
    double h=1./M;
    for(int m=0;m <= M;m++)
         pX[m]=m*h;
}



// Resolution du systeme tridiagonal
//   |b_0 c_0  0                | |x_0    |   | f_0    |
//   |a_1 b_1 c_1               | |x_1    | = | f_1    |
//   |  ........................| | ...   |   | ...    |
//   |          a_(N-1) b_(N-1) | |x_(N-1)|   | f_(N-1)|
//
//  Attention !! les coefficients de la matrice sont modifies
//       la solution est stockee dans le vecteur f
//
void LuTridiag(int N, double *a, double *b, double *c, double *f)
{
	//----------Calcul des coeff star (b* -> b; c* -> c)
	c[0]=c[0]/b[0];
	for(int i=1;i<N;i++)
	{
	 b[i]=b[i]-a[i]*c[i-1];
	 c[i]=c[i]/b[i];
	}
    //----------Substitution directe Ly=f
	f[0]=f[0]/b[0];
	for(int i=1;i<N;i++)	
	 f[i]=(f[i]-a[i]*f[i-1])/b[i];

	//----------Substitution inverse Ux=y -> solution dans f
	for(int i=N-2;i>=0;i--)	
	 f[i]=f[i]-c[i]*f[i+1];
}

// Calcul des coefficients de la matrice et du second membre
// (la matrice est de taille M+1, la premiere equation
//   etant constitue par la condition de Dirichlet)

void CalcCoeff(int M, double *a, double *b, double *c, double *f)
{
  double h=1./M;
  a[0]=0.;b[0]=1.;c[0]=0.;
  f[0]  =(To-Te)/Te;
  for(int m=1;m<=M;m++)
    {  a[m]=-1.;
       b[m]=2.+act*h*h;
       c[m]=-1.;
       f[m]=0.;
    }

  b[M]=1;
}


//=======================================================================

int main( ) 
{
// Construction du maillage

  int M;
     cout << "Dimension du maillage "<<endl;
     cin >> M;

  double * xa=new double[M+1];
  Abscisses(M,xa);

// Calcul des coefficients de la matrice

  double * a=new double[M+1];
  double * b=new double[M+1];
  double * c=new double[M+1];
  double * f=new double[M+1];

  CalcCoeff(M,a,b,c,f);

// Résolution par la methode LU -> solution dans f
// (systeme de dimension M+1)

  LuTridiag(M+1,a,b,c,f);


// Solution (exacte) analytique

   double * thetaa= new double[M+1];
   VectExacte(M,thetaa,xa,Solexacte);

// Affichage a l'ecran (valeurs adimensionnalisees)

   AfficheTab(M+1,f,thetaa,"Sol num / Sol exacte");

// Sauvegarde du resultat dans un fichier pour gnuplot

    SauveTab(M+1,xa,f,thetaa,"result_LU.dat");

// Destruction des pointeurs
   
   delete[] a;delete[] b;    delete[] c; delete[] f;
   delete[] thetaa; delete[]xa;

    return 0;
}