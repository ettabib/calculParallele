/* Calcul de la distribution de temperature 
   dans une ailette de radiateur
   ********* calcul stationnaire*******************
   equation type (voir texte pour les dim physiques)

    -u''(x)+act*u(x)=0, 0<x<1
     u(0)=uo; u'(1)=0
   **************************************************
   discretisation a l'ordre 1 de la condition u'(1)=0
   **************************************************
*/

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const int   MMAX=501;
      double L=40.,a1=4.,a2=50.;
      double To=46., Te=20.;
      double k=0.164, hc=1.e-6*200.;
      double act=2.*(a1+a2)*hc*L*L/(k*a1*a2);

int main( )
{// Construction du maillage

  int M;
     cout << "Dimension du maillage (N<500)"<<endl;
     cin  >> M;
  double h=1./M;

 // Initialisation (distribution linéaire)

  double theta[MMAX]; 
  theta[0]=(To-Te)/Te;       //condition en x=0
     for(int m=1;m <= M;m++) //distribution lineaire
         theta[m]=(1.-m*h)*(To-Te)/Te;

 // Resolution par Gauss-Seidel  

   int iter=0, itermax=1000000;
   double xnew, epsilon;

     do
       { epsilon=0.;

         for(int m=1;m < M;m++)
         {   xnew  = (theta[m-1]+theta[m+1])/(2.+h*h*act);
          epsilon += (xnew-theta[m])*(xnew-theta[m]);
          theta[m] = xnew;
         }

	   xnew = theta[M-1]; //condition en x=1
       epsilon += (xnew-theta[M])*(xnew-theta[M]);
	theta[M]=  xnew; 

       iter=iter+1;     
       }while((sqrt(epsilon) > 1e-8) && (iter < itermax) );

    if(iter<itermax)
      cout << "M="<<M<<"  Convergence en GS apres "<<iter<<" iterations\n";
    else
      cout << "Stop en GS apres "<<iter<<" iterations\n";

 // Solution (exacte) analytique (grandeur physique)

   double thetaa[MMAX];
     for(int m=0;m <= M;m++)
       thetaa[m]=Te+(To-Te)*cosh(sqrt(act)*(1-m*h))/cosh(sqrt(act));

 // Sauvegarde du résultat dans un fichier pour gnuplot
 // (on revient aux valeurs physiques pour theta)

     cout<<"fichier resultat : result.dat"<<endl;
     ofstream f("result.dat");
       for(int m = 0; m<= M; m++)
         f<<m*h*L<<"\t"<<Te*(1.+theta[m])<<"\t"<<thetaa[m]<<endl;

     f.close();

     return 0;
}