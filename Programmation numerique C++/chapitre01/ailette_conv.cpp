/* (Exercice 1.3 page 16)
   Modification du programme ailette.cpp
   pour calculer ||sol_num -sol_exacte ||
   pour differentes normes

   on utilise les valeurs adimensionnees
   (sans dimension physique)

   attention hc est multiplie par 100 
   pour reduire le temps de calcul
   (cond aux limites a l'ordre 1 ou 2)
*/

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const int   MMAX=501;
      double L=40.,a1=4.,a2=50.;
      double To=46., Te=20.;
      double k=0.164, hc=1.e-6*200.*100;
      double act=2.*(a1+a2)*hc*L*L/(k*a1*a2);


double max(double a, double b)
{  return (a>b)?a:b;  // Operateur ternaire
}

int main( )
{     ofstream f1("erreur_norms.dat"); // norme sup
      ofstream f2("erreur_norm1.dat"); // norme 1
      ofstream f3("erreur_norm2.dat"); // norme 2

// Boucle "plusieurs calculs"
 int M, icalmax=10, icaltimes=25;

 //*******************************************************
  for(int ical=1;ical<= icalmax;ical++)
 //*******************************************************
{
  M=ical*icaltimes; // dimension du maillage
  double h=1./M;

// Initialisation (distribution linéaire)

  double theta[MMAX]; 
  theta[0]=(To-Te)/Te; //condition en x=0
     for(int m=1;m <= M;m++)
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

     //a tester aussi*************************************************
       //    xnew  = (4.*theta[M-1]-theta[M-2])/3.; //condition en x=1
    //****************************************************************
	      xnew  = theta[M-1]; //condition en x=1
          epsilon  += (xnew-theta[M])*(xnew-theta[M]);
	   theta[M] =  xnew; 

       iter=iter+1;     
       }while((sqrt(epsilon) > 1e-8) && (iter < itermax) );

    if(iter<itermax)
      cout << "M="<<M<<"  Convergence en GS apres "<<iter<<" iterations\n";
    else
      cout << "M="<<M<<"  Stop en GS apres "<<iter<<" iterations\n";

// Solution (exacte) analytique
// (valeurs sans dimension physique)
   double thetaa[MMAX];
     for(int m=0;m <= M;m++)
       thetaa[m]=(To-Te)/Te*cosh(sqrt(act)*(1-m*h))/cosh(sqrt(act));

// Calcul d'erreur et comparaison avec la loi theorique
// (on applique les formules de la remarque 1.1 page 25)

     double err=0.;

     //norme sup
     for(int m = 0; m<= M; m++)
       err = max(err,abs(theta[m]-thetaa[m]));

     f1<<h<<"\t"<<err<<"\t"<<pow(h,2)<<endl;

     //norme 1
     err=0.;
     for(int m = 0; m<= M; m++)
       err += abs(theta[m]-thetaa[m]);

     f2<<h<<"\t"<<err<<"\t"<<pow(h,1)<<endl;

     //norme 2
     err=0.;
     for(int m = 0; m<= M; m++)
       err += (theta[m]-thetaa[m])*(theta[m]-thetaa[m]);

     f3<<h<<"\t"<<sqrt(err)<<"\t"<<pow(h,1.5)<<endl;

     if(ical==icalmax)
       {
// Sauvegarde de la solution pour le dernier calcul
// (on revient aux grandeurs physiques)

     ofstream f("result_conv.dat");
     for(int m = 0; m<= M; m++)
       f<<m*h*L<<"\t"<<Te*(1.+theta[m])<<"\t"<<Te*(1.+thetaa[m])<<endl;

     f.close();
       }
}
  f1.close();f2.close();f3.close();

  cout<<"fichiers ecrits :"<<endl<<"erreur_norms.dat"<<endl
      <<"erreur_norm1.dat"<<endl
      <<"erreur_norm2.dat"<<endl
      <<"result_conv.dat"<<endl;

  return 0;
}












