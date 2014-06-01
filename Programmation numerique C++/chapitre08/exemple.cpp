#include "ddouble.h"

int main()
{
ddouble  x(0,1);
ddouble a(1),b,c;

b = sin(x);
c = (a+x) * b ;

cout << "f(x)=(1+x)* sin(x) : [f(x),df/dx] pour x=0 : "<<c << endl;

 return 0;
}







