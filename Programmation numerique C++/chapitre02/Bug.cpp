  #include <iostream>

  using namespace std;

  class Bug{public:

   long * p;               

   Bug(): p(new long[10]){cout << "constructeur adresse "<<p<<endl;};  

  ~Bug()                 {cout << "destructeur  adresse "<<p<<endl;delete p;}    
  };

  long & GetPb(Bug   a,int i){ return a.p[i];} // copie puis destruction de la copie

  long & GetOk(Bug & a,int i){ return a.p[i];} // ok

  int main() 
{

   Bug a;
   GetPb(a,1) = 1;               // !!erreur!! le pointeur a.p est d�truit ici
                                 // l'argument est copi�, puis d�truit

   cout << GetOk(a,1) << endl;   // !!erreur!! on utilise un zone m�moire lib�r�e

   return 0;                     // le pointeur  a.p est encore d�truit ici
}




