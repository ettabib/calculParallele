#include <iostream>

using namespace std;

class Vide
{public:
     Vide() 
     {cout << "Constructeur par defaut  a l'adresse  "<< this << endl;}
      Vide(const Vide & t)
     {cout << "Constructeur par copie   a l'adresse  "<< this <<endl;}
     ~Vide()              
     {cout << "Destructeur  de l'objet    d'adresse  "<< this <<endl;}

Vide & operator = (Vide &t)
        {cout << "Operateur = pour Vide"<< endl;
         cout << "  on copie l'objet d'adresse          "<< &t << endl;
         return t;}
};

Vide  & fct1(Vide v)
{   return v;}

Vide &  fct2(Vide &v)
{   return v;}

int main()
{  
   Vide a, b(a);
        cout<<"--------------------"<< endl;
   b=fct1(a);
        cout<<"--------------------"<< endl;
   b=fct2(a);
        cout<<"--------------------"<< endl;

   Vide * c = new Vide;
   Vide * d = new Vide(a);
        cout << "c pointe vers l'adresse  "<< c << endl;
        cout<<"--------------------"<< endl;

        c=d;
        cout << "c pointe vers l'adresse  "<< c << endl;
        cout<<"--------------------"<< endl;

   delete c;
        cout << "c pointe vers l'adresse  "<< c << endl;
        cout<<"--------------------"<< endl;

   return 0;
}




