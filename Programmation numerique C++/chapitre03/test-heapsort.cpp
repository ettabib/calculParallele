#include <iostream>
#include <cmath>

using namespace std;

typedef double Reel;

template <class T>
T Max(const T & a, const T & b){T z=a<b ? b : a; return z;}

template< class T>  
void  HeapSort(T *c, long n) {

  c--; //  dans l'algorithme original le premier indice de tableau est 1
       // il faut donc décaler le pointeur d'une unité

   long m,j,r,i;

   T crit;

   if( n <= 1)  return;

  m = n/2 + 1;
  r = n;

   while (1) 
   {
     if(m <= 1 ) 
     {
      crit = c[r];
      c[r--] = c[1];
      if ( r == 1 ) { c[1]=crit;  return;}
     }  
     else  
      crit = c[--m];

    j=m;

    while (1) 
    {
      i=j;
      j=2*j;
       if  (j>r) {c[i]=crit; break;}

       if ((j<r) && (c[j] < c[j+1])) j++;

       if (crit < c[j]) c[i]=c[j];
       else {c[i]=crit; break;}
    }
  }
} 

class Complex
{public:

    Reel x,y;
    Reel Norme() const;

    Complex( ): x(0), y(0){ }
    Complex(Reel a, Reel b);     
};

Complex::Complex(Reel a, Reel b)
{   x=a;y=b; }   

Reel Complex::Norme() const
{   return sqrt(x*x+y*y); }

bool operator < (const Complex & a, const Complex & b)
{   return a.Norme() < b.Norme() ? true : false ;  }

ostream & operator << (ostream & f, const Complex & a)
{   f<<"("<<a.x<<","<<a.y<<")"; return f;  }

int main()
{   
    int a=3, b=5; 
    cout <<"Max de "<<a<<"  et  "<< b<< "  est  " <<Max(a,b)<<endl;

    double c=1.5, d=2.5;
    cout <<"Max de "<<c<<"  et  "<< d<< "  est  " <<Max(c,d)<<endl;

    Complex f(1,2), g(1.2,3.2);
    cout <<"Max de "<<f<<"  et  "<< g<< "  est  " <<Max(f,g)<<endl;

    Complex * pf= new Complex(1,3);
	cout <<"valeur du pointeur pf="<< *pf << endl;

    cout<<"=====application de heapsort=====" <<endl;

    Complex * ch=new Complex[10];

    for(int i=0;i<10;i++)
	{ch[i].x=(9-i);ch[i].y=(9-i)*(9-i); 
	  cout<<"Initial i="<<i<<" "<<ch[i]<<endl;
	}

    HeapSort(ch,10);

	cout<<"=======apres heapsort======"<<endl;
	for(int i=0;i<10;i++)
	  cout<<"Final   i="<<i<<" "<<ch[i]<<endl;

	return 0;
  }


