//\#define CHECK\_KN 
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

// Modules n�cessaires
#include "sfem.hpp"
#include "RNM.hpp"
#include "GC.hpp"

//  La fonction InternalError est n�cessaire pour la compatibilit�
//  du FreeFem++ avec RNM
void InternalError(const char * str) {cerr << str; exit(1);}

// Les constantes physiques
    const R L=40. , H=20. ;
    const R T_0= 46, T_e=20 ;
    const R kond = 164.e-3, h_c = 200.e-6;
    const R theta_d= (T_0 -T_e)/T_e;
    const R c_robin = (h_c*L)/kond; 

// Fonctions pour initialisation (t=0)  
R ka(const Triangle & K){ return 1.;}   // diffusivit� 
R theta_0(const R2 & ){return 0;} // champ initial de temp.


//  la classe pour modeliser la matrice  (�q. 4.30) 
// ------------------------------------------

class MatLap: public VirtualMatrice<R> { public:
    const  Mesh & Th;
    const KN<bool> & Gamma_d;    //   true pour les sommets Dirichlet
    R (*beta)(const Triangle &); // pointeur vers la fonction diffusivit�

    const R alpha;       // coef. (de masse) 
    const int Gamma_r;   // num�ro de de la fronti�re Robin
    const R  alpha_r;    // coef. devant l'int�grale sur la fronti�re Robin

    typedef  VirtualMatrice<R>::plusAx plusAx;

  // constructeur (par d�faut) avec liste d'initialisation 
    MatLap(const  Mesh & T, const KN<bool> & g_d, R a, 
           R (*b)(const Triangle &)=0, R a_r=0, int g_r=0)
        : Th(T),Gamma_d(g_d),alpha(a),beta(b),alpha_r(a_r), Gamma_r(g_r) {};

    void addMatMul(const  KN_<R>  & x, KN_<R> & Ax) const;
    plusAx operator*(const KN<R> &  x) const {return plusAx(this,x);}
    
};

// La fonction qui calcule le produit matrice-vecteur pour
// la m�thode du gradient conjugu� 
// ------------------------------------------
void MatLap::addMatMul(const  KN_<R>  & x, KN_<R> & Ax) const {

    // int�grales sur Omega
    // assemblage cf. algorithme 4.1
    //-----------------------------
    for (int k=0;k<Th.nt;k++)
    {
        const Triangle & K(Th[k]);
        int i0(Th(K[0])),i1(Th(K[1])),i2(Th(K[2])); // num�ros globaux des 3 sommets
        R ax0=0,ax1=0,ax2=0;

        // contribution de  int w^i w^j 
        //------------------------       
        if (alpha) { // (optimisation) � calculer seulement si alpha diff�rent de 0
            R cm = alpha*K.area/12.; 
            ax0 += (2*x[i0]+  x[i1]+  x[i2])* cm;
            ax1 += (  x[i0]+2*x[i1]+  x[i2])* cm;
            ax2 += (  x[i0]+  x[i1]+2*x[i2])* cm; }


        // contribution de  int nabla w^i \nabla w^j 
        //------------------------
        if (beta) {  // (optimisation) � calculer seulement si beta diff�rent de 0
            R2 H0(K.H(0)),H1(K.H(1)),H2(K.H(2));
            R2 gradx= H0*x[i0] + H1*x[i1] + H2*x[i2];
            R cl =  beta(K)*K.area ;
            ax0 +=(gradx,H0)*cl;
            ax1 +=(gradx,H1)*cl;
            ax2 +=(gradx,H2)*cl; }

        if ( !Gamma_d[i0] ) Ax[i0] +=  ax0;
        if ( !Gamma_d[i1] ) Ax[i1] +=  ax1;
        if ( !Gamma_d[i2] ) Ax[i2] +=  ax2;
    }

    // int�grales sur Gamma_R
    //-----------------------------
    if (Gamma_r && alpha_r)  
        for (int e=0;e<Th.neb;e++)
        {
            const  BoundaryEdge & E = Th.bedges[e];
            if (E.lab == Gamma_r){
	      int i = Th(E[0]),  j = Th(E[1]); // num�ros globaux des sommets de l'ar�te
              R coef =  E.length()*alpha_r/6.; 
                if ( !Gamma_d[i] ) Ax[i] += coef*(x[i]*2+x[j]  );
                if ( !Gamma_d[j] ) Ax[j] += coef*(x[i]  +x[j]*2);}
        }
}




// La fonction qui sauvegarde les r�sultats 3D pour gnuplot
// format (x,y,T) (grandeurs physiques)
// ------------------------------------------
void gnuplotfile(const char * filename,const Mesh & Th,const KN<R> & x)
{ 
    ofstream plot(filename);
    for(int it=0;it<Th.nt;it++)
        plot << ((R2) Th[it][0])*L << " " << T_e*(1+x[Th(it,0)]) << endl
             << ((R2) Th[it][1])*L << " " << T_e*(1+x[Th(it,1)]) << endl
             << ((R2) Th[it][2])*L << " " << T_e*(1+x[Th(it,2)]) << endl
             << ((R2) Th[it][0])*L << " " << T_e*(1+x[Th(it,0)]) << endl 
	     << endl << endl;
    plot.close();
}

// La fonction qui calcule la norme L2
// ------------------------------------------
R NormeL2(Mesh & Th, KN_<R> & x) 
{
        R norm=0.;
    for (int k=0;k<Th.nt;k++)
    {
        const Triangle & K(Th[k]);
        int i0(Th(K[0])),i1(Th(K[1])),i2(Th(K[2])); // num�ros globaux des 3 sommets
        R coef=K.area/6.;
        norm += (x[i0]*(x[i0]+x[i1])
               + x[i1]*(x[i1]+x[i2])
       	       + x[i2]*(x[i2]+x[i0]))*coef;
    }
    return sqrt(norm);
}



// La fonction principale
// ------------------------------------------
int main(int , char** )
{
    // lecture du maillage
    Mesh Th("Th.msh");      

    // la fronti�re Dirichlet (Gamma_D) � l'�tiquette (label) 1
    //  Gamma_d [i] == true  si  i sur Gamma_D
    //--------------------------
    KN<bool> Gamma_d(Th.nv);
    Gamma_d=false;
    for (int i=0;i<Th.neb;i++)
        if (Th.bedges[i].lab==1  )
            for (int j=0;j<2;j++)
                Gamma_d[Th(Th.bedges[i][j])] = true;

    // Les vecteurs n�cessaires au calcul
    //-----------------------------
    KN<R> b(Th.nv),        // le second membre du syst�me 
          theta_n(Th.nv),  // la solution theta^n
          theta_o(Th.nv),  // la solution theta^{n-1}
          bc(Th.nv) ;      // les valeurs impos�es sur Gamma_D

    // On impose les valeurs sur Gamma_D : theta=theta_D
    //---------------------------
    for (int i=0;i<Th.nv;i++)
        if (Gamma_d[i])   bc[i] = theta_d;
    else bc[i] =0;

    // Initialisation de la solution (t=0)
    //---------------------------
    theta_n = bc;  // v�rification des conditions de Dirichlet
    for (int i=0;i<Th.nv;i++) // valeurs � l'int�rieur du domaine
        if ( !Gamma_d[i]) theta_n[i] =theta_0(Th(i));
            theta_o=0.;

     gnuplotfile("plot-init",Th,theta_n);

    // On fixe le pas de temps
    //---------------------------
    R dt =0.5;

    // D�finition de la matrice  A^{alpha,beta,alpha_R} 
    // avec alpha=1/(Delta t), beta=0, alpha_R=0
    // (pour le second membre)
    //--------------------------- 
    MatLap M(Th,Gamma_d,1./dt);

    // D�finition de la matrice A^{alpha,beta,alpha_R}
    // avec alpha=1/\Delta t, beta=beta_K, alpha_R=c_robin
    // (matrice du syst�me)
    //--------------------------- 
    MatLap A(Th,Gamma_d,1./dt,ka,c_robin,4);

    // D�finition de la matrice identit�
    // (utilis�e comme pr�conditionneur dans le gradient conjugu�)
    //----------------------------
    MatriceIdentite<R> Id;

    // Fichier qui va stocker les r�sultats
    //----------------------------
    char filename[256];


    // Iteration en temps
    //----------------------------
    R time=0;int iter=0;
    R eps=1.;

      while(eps > 1.e-5)
    {
        time += dt;
        iter += 1;
        cout<<"================================================="<<endl;
        cout <<" iteration = " <<  iter << " temps = "<< time << " max="
            << theta_n.max() << " min=" << theta_n.min() << endl;
 
        b =  M*theta_n;   
        GradienConjugue(A,Id, b,theta_n,Th.nv,1e-6);
 
        // calcul du crit�re d'arr�t $||\theta^n-\theta^{n-1}|| \le \varepsilon$
        theta_o=theta_n-theta_o;
        eps=NormeL2(Th,theta_o);
        cout<<" Norme diff ="<<eps<<endl;
        theta_o=theta_n;


        // sauvegarde des fichiers r�sultats
        //--------------------------------
        sprintf(filename,"plot-%d",iter); // nom du fichier gnuplot 3D
        gnuplotfile(filename,Th,theta_n);
    }
    return 0;
}
