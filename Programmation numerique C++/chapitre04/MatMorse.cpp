#include "MatMorse.hpp"
MatriceMorseSymetrique::MatriceMorseSymetrique(const  Mesh & Th)
{
  int color=0, * mark;
  int i,j,jt,k,p,t;
  n = m = Th.nv;
  mark = new int [n];
  // construction optimisee de l'image réciproque de Th(k,j)
  int end_list=-1,*head_s,*next_p;
  head_s = new int [Th.nv];
  next_p = new int [Th.nt*3];
  p=0;
  for (i=0;i<Th.nv;i++)
    head_s[i] = end_list;
  for (k=0;k<Th.nt;k++)
    for(j=0;j<3;j++)
    next_p[p]=head_s[i=Th(k,j)], head_s[i]=p++;

  //   initialisation du tableau de couleur
  for(j=0;j<Th.nv;j++)
    mark[j]=color;
  color++;

  //1)  calcul du nombre de coefficients non nuls a priori de la matrice
  nbcoef = 0;
  for(i=0; i<n; i++,color++,nbcoef++)
    for (p=head_s[i],t=p/3; p!=end_list; t=(p=next_p[p])/3)
    for(jt=0; jt< 3 ; jt++ )
    if ( i <= (j=Th(t,jt) )  && (mark[j] != color))
  mark[j]=color,nbcoef++; // nouveau coefficient => marquage + ajout
// 2) allocations mémoires
   ligne.set(new int [n+1],n+1);

colonne.set( new int [ nbcoef],nbcoef);
a.set(new double [nbcoef],nbcoef);
// 3) constructions des deux tableaux ligne et colonne
 ligne[0] = -1;
  nbcoef =0;
  for(i=0; i<n; ligne[i++]=nbcoef, color++)
    for (p=head_s[i],t=p/3; p!=end_list; t=(p=next_p[p])/3)
       for(jt=0; jt< 3 ; jt++ )
        if ( i <= (j=Th(t,jt))  && (mark[j] != color))
         mark[j]=color, colonne[nbcoef++]=j; // nouveau coefficient => marquage + ajout
// 4) tri des lignes par index de colonne
  for(i=0; i<n; i++)
    HeapSort(colonne + ligne[i] + 1 ,ligne[i+1] - ligne[i]);
// nettoyage
  delete  [] head_s;
  delete  [] next_p;
}

int main() {
  Mesh Th("Th.msh");
  MatriceMorseSymetrique A(Th);
  cout << A.n << endl ;
  cout << " Nb non zero coef " << A.nbcoef << endl;
}


