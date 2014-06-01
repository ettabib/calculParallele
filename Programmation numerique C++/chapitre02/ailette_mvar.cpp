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
    maillage variable -> nouvelle discretisation
                         de la derivee seconde
    ***********************************************
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

// Fonction pour le maillage variable
double MVarSin(double x) 
{
  double  eps=3./4.;
  double pis2=2.*atan(1.);
  return  sin(eps*pis2*x)/sin(eps*pis2);
}

//Fonction pour le maillage uniforme
double MUnif(double x)
{  return x;}



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

void Abscisses(int  M, double * pX,double (*pFunc)(double x)) 
{
    double h=1./M;
    for(int m=0;m <= M;m++)
         pX[m]=pFunc(m*h);
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

void CalcCoeff(int M, double * x, double *a, double *b, double *c, double *f)
{
  // le pas de discretisation est maintenant variable
  double * dx=new double[M];
  for(int m=0;m<M;m++)
    dx[m]=x[m+1]-x[m];

  // Condition de Dirichlet
  a[0]=0.;b[0]=1.;c[0]=0.;
  f[0]  =(To-Te)/Te;
 
  // Les autres coefficients
  for(int m=1;m<M;m++)
    {  double demis=(dx[m]+dx[m-1])/2.;
       a[m]=-1./(dx[m-1]*demis);
       b[m]= 2./(dx[m]*dx[m-1])+act;
       c[m]=-1./(dx[m]*demis);
       f[m]=0.;
    }

  // Condition de Neumann
  a[M]=-1.;b[M]= 1.;c[M]= 0.;  
  f[M]= 0.;

  delete[] dx;
}


//=======================================================================

int main( ) 
{
// Construction du maillage

  int M;
     cout << "Dimension du maillage "<<endl;
     cin >> M;

  double * xa=new double[M+1];
  Abscisses(M,xa,MVarSin);
  //Abscisses(M,xa,MUnif);// a tester pour retrouver les res pour maillage uniforme


// Calcul des coefficients de la matrice

  double * a=new double[M+1];
  double * b=new double[M+1];
  double * c=new double[M+1];
  double * f=new double[M+1];

  CalcCoeff(M,xa,a,b,c,f);

// Résolution par la methode LU
// (systeme de dimension M+1)

  LuTridiag(M+1,a,b,c,f);

// Solution (exacte) analytique

   double * thetaa= new double[M+1];
   VectExacte(M,thetaa,xa,Solexacte);

// Affichage a l'ecran (valeurs adimensionnalisees)

   AfficheTab(M+1,f,thetaa,"Sol num / Sol exacte");

// Sauvegarde du resultat dans un fichier pour gnuplot

    SauveTab(M+1,xa,f,thetaa,"result_mvar.dat");

// Destruction des pointeurs
   
   delete[] a;delete[] b;    delete[] c;
   delete[] f;delete[] thetaa;
   delete[]xa;

    return 0;
}