/* Calcul de la distribution de temperature 
   dans une ailette de radiateur
   ********* calcul instationnaire*******************
   equation type (voir texte pour les dim physiques)

    (d/dt)u(x,t)-(d2/dx2)u(x,t)+act*u(x,t)=0, 0<x<1, 0<t<tmax
     cond aux limites : u(x,t)=uo(x);(d/dx)u(1,t)=0
     cond initiale    : u(0,t)=u0; 

*/

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

      double L=40.,a1=4.,a2=50.;
      double To=46., Te=20.;
      double k=0.164, hc=1.e-6*200.;
      double kappa=68.2, t0=L*L/kappa; 
      double act=2.*(a1+a2)*hc*L*L/(k*a1*a2);

// une classe pour la temperature
class Temp {
public :
  int M;
  double To, Te ;
  double * T;
  Temp(int Mu,double Tou=20., double Teu=20.)
  {    M=Mu;T=new double[M+1]; // allocation du tableau
       To=Tou;Te=Teu;
       T[0]=(To-Te)/Te; // initialisation
       for(int m=1;m <= M;m++)
         T[m]=0.; // a tester T[m]=T[0];       
  }
  // sauvegarde des valeurs dans un fichier (grandeurs physiques)
  void Sauve(ofstream & f)
  { for(int m=0;m <= M;m++)
      f<<m*L*(1./M)<<"\t"<<Te*(1.+T[m])<<endl;
    f<<endl<<endl;  // deux lignes vides
  }
};


int main( )
{
// Construction du maillage

  int M;
     cout << "Dimension du maillage "<<endl;
     cin >> M;

  double h=1./M;
  double cfl=1., dt=cfl*(h*h)/2.; 

// Définition d'un objet de type temp

     Temp theta(M,To,Te);

// Avancement en temps  

   int iter=0, itermax=1000000;
   double xnew, epsilon,time=0.;


   ofstream f("result_inst.dat"); // stocke des resultats interm

     do
       { // sauvegarde de la solution intermédiaire

	if((iter%1000 == 0)&(iter < 5000))
          {theta.Sauve(f); 
	   cout<<"Sauvegarde pour t="<<time<<endl;}         

        epsilon=0.;  
        for(int m=1;m < M;m++)
          {xnew  = (1.-act*dt)*theta.T[m]
                 +dt*(theta.T[m-1]-2.*theta.T[m]+theta.T[m+1])/(h*h);

           epsilon   += (xnew-theta.T[m])*(xnew-theta.T[m]);
           theta.T[m] = xnew;
          }

	   xnew       = theta.T[M-1]; //condition en x=1
           epsilon   += (xnew-theta.T[M])*(xnew-theta.T[M]);
	   theta.T[M] = xnew; 

         iter++;time +=dt;
       }while((sqrt(h*epsilon) > 1e-8) && (iter < itermax) );

    if(iter<itermax)
      cout << "M="<<M<<" Sol stationnaire apres "<<iter<<" pas de temps \n";
    else
      cout << "Stop apres "<<iter<<" pas de temps \n";

      cout<<" --> Temps final   =" << time<<endl;

// Solution (exacte) stationnaire
// (valeurs sans dimension)

   Temp thetaa(M);
     for(int m=0;m <= M;m++)
       thetaa.T[m]=(To-Te)/Te*cosh(sqrt(act)*(1-m*h))/cosh(sqrt(act));

     // sauvegarde de la solution analytique
   ofstream ff("result_stat.dat");
     thetaa.Sauve(ff);    
   ff.close();

// Sauvegarde du résultat du dernier pas de temps
   theta.Sauve(f);

   cout<<"Resultat instationnaire dans : result_inst.dat"<<endl;
   cout<<"Resultat   stationnaire dans : result_stat.dat"<<endl;
   return 0;
}










