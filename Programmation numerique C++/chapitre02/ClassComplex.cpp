#include <iostream>
#include <cmath>

using namespace std;
typedef double Reel;

template <class T>
T Max(const T & a, const T & b){T z=a>b ? a : b; return z;}

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

bool operator > (const Complex & a, const Complex & b)
{   return a.Norme() > b.Norme() ? true : false ;  }

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
	cout << *pf << endl;


    return 0;
}




