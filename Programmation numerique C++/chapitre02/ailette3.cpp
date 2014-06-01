/* Calcul de la distribution de température
   dans une ailette de radiateur
   ********* calcul stationnaire*******************
   équation type (voir texte pour les dim physiques)

    -u''(x)+act*u(x)=0, 0<x<1
     u(0)=uo; u'(1)=0

*/

#include <iostream>   
#include <fstream>    
#include <string>     
#include <cmath>      

using namespace std;  

const int   MMAX=501;   
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

void Abscisses(int & M, double * pX) 
{
    double h=1./M;
    for(int m=0;m <= M;m++)
         pX[m]=m*h;
}


void InitSol(int * M, double* pX, double* pTheta) 
{
     pTheta[0]=(To-Te)/Te;
     for(int m=1;m <= (*M);m++)
         pTheta[m]=(1.-pX[m])*(To-Te)/Te;
}


void GaussSeidel(int M, int itermax, double prec, double* pTheta) 
{
	 double h=1./M;double epsilon=0.;
     do
       { epsilon=0.;double xnew;

         for(int m=1;m < M;m++)
	 {    xnew   = (pTheta[m-1]+pTheta[m+1])/(2.+h*h*act);
           epsilon  += (xnew-pTheta[m])*(xnew-pTheta[m]);
           pTheta[m] = xnew;
	 }

     //a tester ******************************************************
	 //xnew  = (4.*pTheta[M-1]-pTheta[M-2])/3.; //condition en x=1
    //****************************************************************
	     xnew  = pTheta[M-1]; //condition en x=1
         epsilon  += (xnew-pTheta[M])*(xnew-pTheta[M]);
	 pTheta[M] = xnew; 

       }while((sqrt(epsilon) > prec) && (Compteur() < itermax) );

    int iter=Compteur();
    if(iter<itermax)
      cout << "M="<<M<<"  Convergence en GS apres "<<iter<<" iterations\n";
    else
      cout << "Stop en GS apres "<<iter<<" iterations\n";
}

//=======================================================================

int main(int, char** ) 
{
// Construction du maillage

  int M;
     cout << "Dimension du maillage (N<500)"<<endl;
     cin >> M;

  double xa[MMAX];
  Abscisses(M,xa);

// Initialisation (distribution linéaire)

  double * theta=new double[M+1];
  InitSol(& M,xa,theta);

// Résolution par Gauss-Seidel

  GaussSeidel(M,100000,1e-8,theta);

// Solution (exacte) analytique

   double * thetaa= new double[M+1];
   VectExacte(M,thetaa,xa,Solexacte);

// Affichage à l'écran (valeurs adimensionnalisées)

   AfficheTab(M+1,theta,thetaa,"Sol num / Sol exacte");

// Sauvegarde du résultat dans un fichier pour gnuplot

    SauveTab(M+1,xa,theta,thetaa,"result.dat");

    return 0;
}

