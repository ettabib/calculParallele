/* dot.f -- translated by f2c (version 20010821).

   You must link the resulting object file with the libraries:

    -lf2c -lm   (in that order)

*/



#include "ddouble.h"

typedef long int integer;

typedef ddouble real;

typedef ddouble doublereal;



doublereal dot_(const int* n, ddouble* x, ddouble* y)

{

    /* System generated locals */

    integer i__1;

    real ret_val;



    /* Local variables */

    static integer i__;



    /* Parameter adjustments */

    --y;

    --x;



    /* Function Body */

    ret_val = (float)0.;

    i__1 = *n;

    for (i__ = 1; i__ <= i__1; ++i__) {

/* L10: */

    ret_val += x[i__] * y[i__];

    }

    return ret_val;

} /* dot_ */



const int n=40;

int main()

{

    ddouble x[n],y[n];

    ddouble u(1.3,1), z;

    x[0]=1; y[0] =1;

    for(int i=1;i<n;i++)

        { x[i] =(1-u)*x[i-1]; y[i] =y[i-1]/(1+u);}

    z = dot_(&n,x,y);

    cout<<z <<0.5*(1+1./u) << -0.5/u/u <<endl;

    return 0;
}

