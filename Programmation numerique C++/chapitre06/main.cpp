#include <cassert>
#include <fstream>
#include <iostream>
using namespace std;
#include "Mesh.hpp"

using namespace bamg;

void GnuPlot(const Triangles & Th,const char *filename) {
   ofstream ff(filename);
   for (int k=0;k<Th.nbt;k++)    
    {
     const Triangle &K=Th[k];
     if (K.det>0) // true triangle 
      {
        for (int k=0;k<4;k++)
          ff << K[k%3].r << endl;
         ff << endl << endl ;} 
   }
  }
int main(int argc, char ** argv)
{  
 int nbv = argc > 1 ? atoi(argv[1]) : 100;
 int na= argc > 2 ? atoi(argv[2]) : 0 ;
 int nb= argc > 3 ? atoi(argv[3]) : nbv-1 ;
 assert ( na != nb && na >=0 && nb >=0 && na < nbv && nb < nbv);
 
 Triangles Th(nbv);
 Th.RandomInit();
 Th.Insert();
 TriangleAdjacent ta(0,0);
 GnuPlot(Th,"Th0.gplot"); 
 int nbswp =  ForceEdge(Th(na),Th(nb),ta);
 if(nbswp<0) { cerr << " --Impossible de forcer l'arête " << endl;}
 else {
   cout << " Nb de swap pour forcer l'arete [" << na << " " << nb 
	<< "] =" << nbswp << endl;
   GnuPlot(Th,"Th1.gplot"); }
 
 return(0);
} 

