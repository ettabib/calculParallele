#include "expr.hpp"


R Exp::ExpSetSolution::operator()() const {
  assert( globals.pTh) ; // mesh exist
  int nv= globals.pTh->nv;
  if (s.unset()) s.set(new R[nv],nv);
  for (int i=0; i< nv;i++)
    {
      globals.set(i);
      s[i]= a();
      globals.unset();
    }
  return 0;
}

R Exp::ExpSolve::operator()() const {
  assert(globals.pTh);
  Mesh & Th=*globals.pTh;
  int nv=Th.nv;
  R c1=a(),c3=c();
  Value(Th[0])=b(); // initialisation de la valeur retourne par kaSolve 
  Rn bb(Th.nv);
  for (int i=0; i< nv;i++)
    {
      globals.set(i);
      bb[i]= rhs();
      globals.unset();
    }
  KN<bool> Gamma_d(Th.nv); //   Gamma\_d[i] == true =>  $i \in \Gamma_d$
  int ion=(int) don();
  int iron=(int) ron();
  int inon=(int) non();
  Gamma_d=false;
  if (s.unset()) {s.set(new R[nv],nv); s=0;}// allocation
  for (int i=0;i<Th.neb;i++)
    if (Th.bedges[i].lab==ion  )
      for (int j=0;j<2;j++)
	Gamma_d[Th(Th.bedges[i][j])] = true;
  for (int i=0;i<Th.nv;i++)
    if (Gamma_d[i] )
      { globals.set(i);
      s[i]=g();}
         globals.unset();
	 
	 MatLap M(Th,Gamma_d,1.);
	 MatLap A(Th,Gamma_d,c1,kaSolve,c3,iron);
	 MatriceIdentite<R> Id;
	 Rn b(Th.nv);
	 b =  M*bb;
	 GradienConjugue(A,Id, b,s,Th.nv,1e-6);
}
