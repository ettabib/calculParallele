//\#define CHECK\_KN
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "sfem.hpp"


#include "RNM.hpp"
#include "GC.hpp"
#include "sfemmat.hpp"

//  The RNM package need the following function
//  to be compatible with \Blue{FreeFem++}



void MatLap::addMatMul(const  KN_<R>  & x, KN_<R> & Ax) const {

    for (int k=0;k<Th.nt;k++)
    {
        const Triangle & K(Th[k]);
        int i0(Th(K[0])),i1(Th(K[1])),i2(Th(K[2])); // numero globaux des 3 sommets
        R ax0=0,ax1=0,ax2=0;
        if (beta) {  // optimation calculer seulement si defini
                     //  $ \int_K \beta(K)\alpha \nabla w^i \nabla w^j $
            R2 H0(K.H(0)),H1(K.H(1)),H2(K.H(2));
            R2 gradx= H0*x[i0] + H1*x[i1] + H2*x[i2];
            R cl =  beta(K)*K.area ;
            ax0 +=(gradx,H0)*cl;
            ax1 +=(gradx,H1)*cl;
            ax2 +=(gradx,H2)*cl; }

        if (alpha) { //  optimation calculer seulement si non nulle
            R cm = alpha*K.area/12.; // = $ \int_K \alpha w^i w^j $
                                     //  remarque $ \int_T \alpha w^i w^i = 2 \times \mathtt{cm} $
            ax0 += (2*x[i0]+  x[i1]+  x[i2])* cm;
            ax1 += (  x[i0]+2*x[i1]+  x[i2])* cm;
            ax2 += (  x[i0]+  x[i1]+2*x[i2])* cm; }

        if ( !Gamma_d[i0] ) Ax[i0] +=  ax0;
        if ( !Gamma_d[i1] ) Ax[i1] +=  ax1;
        if ( !Gamma_d[i2] ) Ax[i2] +=  ax2;
    }

    if (Gamma_r && alpha_r)
        for (int e=0;e<Th.neb;e++)
        {
            const  BoundaryEdge & E = Th.bedges[e];
            if (E.lab == Gamma_r)
            {
                int i = Th(E[0]),  j = Th(E[1]);
                R coef =  E.length()*alpha_r/6.; // = $ \int_E \alpha_r w^i w^j $
                                                 // remarque:       $ \int_E \alpha_r w^i w^i = 2 \times \mathtt{coef} $
                if ( !Gamma_d[i] ) Ax[i] += coef*(x[i]*2+x[j]  );
                if ( !Gamma_d[j] ) Ax[j] += coef*(x[i]  +x[j]*2);
            }
        }
}

void gnuplotfile(const char * filename,const Mesh & Th,const KN<R> & x)
{ // a file for gnuplot
    ofstream plot(filename);
    for(int it=0;it<Th.nt;it++)
        plot << (R2) Th[it][0] << " " << x[Th(it,0)] << endl
            << (R2) Th[it][1] << " " << x[Th(it,1)] << endl
            << (R2) Th[it][2] << " " << x[Th(it,2)] << endl
            << (R2) Th[it][0] << " " << x[Th(it,0)] << endl << endl << endl;
}
