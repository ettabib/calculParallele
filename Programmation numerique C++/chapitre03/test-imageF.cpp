#include <iostream>
#include <cassert>

using namespace std;

int main() 
{  int n=5, m=4; 
   int F[]={0,1,3,1,1},head_F[m],next_F[n];

// initialement, les listes des antécédents sont vides

int Not_In_Im_F = -1;

 for (int j=0;j<m;j++)
  head_F[j]=Not_In_Im_F;

// chaînage amont

 for (int i=0;i<n;i++)
  {int j=F[i];next_F[i]=head_F[j];head_F[j]=i;}

// parcours des antécédents et affichage

  for(int ii=0;ii<n;ii++)
  {int j=F[ii];cout<<"==== j="<<j<<" ============"<<endl;

   for (int i=head_F[j];i!=Not_In_Im_F;i=next_F[i])
    { assert(F[i]==j);// j doit être dans l'image de i
    cout<<"i="<<i<<endl; 
    }
  }
  return 0;
}

