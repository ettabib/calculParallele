/* Calcul de la distribution de temp�rature
   dans une ailette de radiateur
   ********* calcul stationnaire*******************
   �quation type (voir texte pour les dim physiques)

    -u''(x)+act*u(x)=0, 0<x<1
     u(0)=uo; u'(1)=0

*/
//---------------------------------------------------------------------
//----Directives du pr�processeur

#include <iostream>   // utiliser les entr�es/sorties � l'�cran
#include <fstream>    // utiliser les entr�es/sorties avec fichiers 
#include <string>     // utiliser les cha�nes de caract�res 
#include <cmath>      // utiliser les fonction math usuelles

using namespace std;  // utiliser l'espace de noms standard 

//---------------------------------------------------------------------
//------D�clarations des variables ou constantes globales

   // ! Le type des variables doit �tre toujours d�fini

const int   MMAX=501;     // D�finition d'une constante globale
      double L=40.,a1=4.,a2=50.;
      double To=46., Te=20.;
      double k=0.164, hc=1.e-6*200.;
      double act=2.*(a1+a2)*hc*L*L/(k*a1*a2);

//---------------------------------------------------------------------
//------D�finition des fonctions

   // !  La d�finition des fonctions doit pr�c�der leur utilisation

//Fonction sans aucun argument (autre d�f possible: int Compteur(void) )
//=======================================================================

int Compteur()      // Le type des fonctions doit �tre toujours d�fini 
{                   //--------------------d�but bloc
  static int i=0;   // Var statique(sa valeur est gard�e d'un appel a l'autre)
                    // La variable peut �tre initialis�e d�s sa d�claration
  i++;              // La var i est incr�ment�e de 1 (i=i+1)
                    // Toute instruction dans un bloc doit finir avec ;
  return i;         // Valeur (une seule !!!) retourn�e par la fonction
}                   //---------------------fin bloc

//Fonction simple avec argument de type double
//=======================================================================

double Solexacte(double x) 
{
    return  (To-Te)/Te*cosh(sqrt(act)*(1-x))/cosh(sqrt(act));
}

//Fonctions avec arguments: pointeur-tableau et pointeur-cha�ne de caract�res
//=======================================================================

int AfficheTab(int M, double *pTab1, double *pTab2, char *Message)
{
     // Affiche � l'�cran les vecteurs p*Tab en utilisant le flux de sortie <<
                   // "endl" ou "\n" indique un saut de ligne
                   // "\t" introduit une tabulation

      cout << Message <<endl;
      cout << "Dimension du vecteur  = "<< M <<endl;

                  // Exemple de boucle for
  for(int i=0;i<M;i++)
  { //-----d�but bloc for
                 // Acc�s � la valeur de l'�l�ment du tableau par son pointeur
   cout<< "Element "<< i <<"\t"<<pTab1[i]<<"\t"<<pTab2[i]<<"\n";
  } //-----fin   bloc for

    return 0;     // Valeur retourn�e (superflue)--> void AfficheTab(...) possible
}


void SauveTab(int M, double *pTab1,  double *pTab2, 
              double *pTab3, char *NomFic) 
{
     // Sauvegarde des vecteurs pTab dans le fichier NomFic

    ofstream fic(NomFic);         // on ouvre le fichier
     for(int m = 0; m< M; m++)    // boucle for contenant une seule instruction
       fic<<pTab1[m]<<"\t"<<pTab2[m]<<"\t"<<pTab3[m]<<endl;

    fic.close();                  // on ferme le fichier
    cout<<"fichier ecrit : "<<NomFic<<endl;
}

//Fonctions avec arguments: pointeur-tableau et pointeur-fonction
//=======================================================================

void VectExacte(int M, double* pTab, double* pX, double (*pFunc)(double x)) 
{
    for(int m=0;m<=M;m++)
    {
              // Appel de la fonction en utilisant son pointeur
     pTab[m]=pFunc(pX[m]);
    }
}

//Les autres fonctions
//=======================================================================
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


//Fonction principale ou main (l'ex�cution commence par main)
//=======================================================================

int main(int ,char ** ) 
{
// Construction du maillage

  int M;
     cout << "Dimension du maillage (N<500)"<<endl;
     cin >> M;

     // Definition d'un tableau de taille fixe (cr�ation statique); 
     //   sa dimension doit etre une constante

  double xa[MMAX];
  Abscisses(M,xa);

// Initialisation (distribution lin�aire)
     // Definition d'un tableau dynamique (pointeur); 
     // seulement M+1 �l�ments sont allou�s

  double * theta=new double[M+1];
  InitSol(& M,xa,theta);

// R�solution par Gauss-Seidel

  GaussSeidel(M,100000,1e-8,theta);

// Solution (exacte) analytique

   double * thetaa= new double[M+1];
   VectExacte(M,thetaa,xa,Solexacte);

// Affichage � l'�cran (valeurs adimensionnalis�es)

   AfficheTab(M+1,theta,thetaa,"Sol num / Sol exacte");

// Sauvegarde du r�sultat dans un fichier pour gnuplot

    SauveTab(M+1,xa,theta,thetaa,"result.dat");

    return 0;
}

