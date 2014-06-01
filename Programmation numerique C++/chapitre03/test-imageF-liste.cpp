#include <iostream>
#include <cassert>
#include <cstring>
#include <string>

using namespace std;

template<class T>
class Val_I 
{public :
         T i;
         Val_I * eSuivant;

         Val_I(T ii):i(ii){eSuivant=NULL;}         
        ~Val_I() {if(eSuivant) delete eSuivant;}

    void Affiche(){cout<<"i="<<i<<endl;}
};

template<class T,class R>
class Image_F
{private :
          Val_I<T> * eDebut;
          Val_I<T> * eCourant;

 public :
          int ne;    // compte le nombre d'enregistrements

          Image_F(int n, T * I, R * FI, R j) 
             { ne=0;eDebut=NULL;eCourant=NULL;

               for(int i=0;i<n;i++)
                if(j==FI[i]) // si j est dans Image(F), on ajoute I[i] à la liste
                { Val_I<T> *pp=new Val_I<T>(I[i]); Ajouter(pp); ne++;}
             }

         ~Image_F(){if(eDebut) delete eDebut;}

     void Ajouter(Val_I<T> * p)
          {if(!eDebut)
            {eDebut=p; eCourant=p;} // ajoute le premier élément de la liste
           else
            {(eCourant ->eSuivant)=p; eCourant=p;} //ajoute un élément 
          }

    void AfficheTout() // parcours des antécédents et affichage
         {cout<<ne<<" antecedents trouves "<<endl;
          Val_I<T> * p = eDebut;
          while(p)
           {p -> Affiche();
            p=(p->eSuivant);}
         }
};

int main() 
{

 {cout<<"=========Application 1============"<<endl;

 int n=5; 
 int I[]={0,1,2,3,4};
 int F[]={0,1,3,1,1}; 

 for(int j=0;j<n;j++)
   {cout<<"*********j="<<F[j]<<endl;
   Image_F<int,int> imj(n,I,F,F[j]);imj.AfficheTout();}
 }



 {cout<<"=========Application 2============"<<endl;

 int n=4;
 string Voiture[]={"Alpha","Fiat","Ferrari","Lamborghini"};
 string Couleur[]={"rouge","bleu","rouge","rouge"}; 

 cout<<"*********j="<<"rouge"<<endl;

 Image_F<string,string> imj(n,Voiture,Couleur,"rouge");

 imj.AfficheTout();
 }

 return 0;
}
