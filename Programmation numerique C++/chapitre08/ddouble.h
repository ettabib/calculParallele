// file ddouble.h, for automatic differentiation (single variable)

// adapted from M. Grundmann's MIPAD



#ifndef _DDOUBLE__H_

#define _DDOUBLE__H_



#include <stdlib.h>

#include <math.h>

#include <iostream.h>



class ddouble 

{

 public:  double val[2]; // val[0] is value and val[1] is the derivative

  

 ddouble() { val[0] = 0; val[1] = 0;}

 ddouble(const ddouble& a){ val[0] = a.val[0]; val[1] = a.val[1]; }

 ddouble(double a, double b=0){ val[0]=a; val[1]=b;} // specifies var for diff

 

 ddouble& operator=(double a)

 	{ val[0] = a; val[1] = 0.0; return *this;} 		

 ddouble& operator=(const ddouble& a)

 	{ val[0] = a.val[0]; val[1] = a.val[1]; return *this;}



		

 double& operator[](const int ii){ return this->val[ii];}

 double operator[](const int ii) const { return this->val[ii];}



  ddouble& operator  + (){return *this;};

  ddouble& operator += (double);

  ddouble& operator += (const ddouble&);

  ddouble& operator -= (double );

  ddouble& operator -= (const ddouble&);

  ddouble& operator *= (double);

  ddouble& operator *= (const ddouble&);

  ddouble& operator /= (double) ;

  ddouble& operator /= (const ddouble&) ;



  ddouble operator++(int);

  ddouble operator--(int);

  ddouble&  operator++();

  ddouble&  operator--();

  

  friend ostream& operator << (ostream&, const ddouble&);

  friend ddouble& operator << (ddouble&,double);

  

  friend ddouble parameter(double);

  

  friend int operator != (const ddouble&,const ddouble&);

  friend int operator != (double,const ddouble&);

  friend int operator != (const ddouble&,double);

  friend int operator == (const ddouble&,const ddouble&);

  friend int operator == (double,const ddouble&);

  friend int operator == (const ddouble&,double);

  friend int operator >= (const ddouble&,const ddouble&);

  friend int operator >= (double,const ddouble&);

  friend int operator >= (const ddouble&,double);

  friend int operator <= (const ddouble&,const ddouble&);

  friend int operator <= (double,const ddouble&);

  friend int operator <= (const ddouble&,double);

  friend int operator > (const ddouble&,const ddouble&);

  friend int operator > (double,const ddouble&);

  friend int operator > (const ddouble&,double);

  friend int operator < (const ddouble&,const ddouble&);

  friend int operator < (double,const ddouble&);

  friend int operator < (const ddouble&,double);





  friend ddouble operator + (const ddouble& x); //{return x + 0.0 ;} ; 

  friend ddouble operator + (const ddouble&,const ddouble&); 

  friend ddouble operator + (double, const ddouble&); 

  friend ddouble operator + (const ddouble&, double); 

  friend ddouble operator - (const ddouble& x ,double y); //{return (-y)+x;}; 

  friend ddouble operator - (const ddouble&,const ddouble&); 

  friend ddouble operator - (double, const ddouble&); 

  friend ddouble operator - ( const ddouble& );

  friend ddouble operator * (const ddouble&,const ddouble&); 

  friend ddouble operator * (double, const ddouble& ); 

  friend ddouble operator * (const ddouble& x, double y);  

  friend ddouble operator / (const ddouble& x, double y); 

  friend ddouble operator / (const ddouble&,const ddouble&); 

  friend ddouble operator / (double,const ddouble&) ; 

  friend ddouble exp (const ddouble&); 

  friend ddouble log (const ddouble&) ; 

  friend ddouble sqrt (const ddouble&) ;

  friend ddouble sin (const ddouble&); 

  friend ddouble cos (const ddouble&);

  friend ddouble tan (const ddouble&);

  friend ddouble pow (const ddouble&,double);

  friend ddouble pow (const ddouble&,const ddouble&);

  friend ddouble pow (const ddouble&, const int);

  friend ddouble abs (const ddouble&) ;

 

};



inline double sign(const ddouble& x)

{ return ( x < 0.0 ? -1.0 : 1.0); }



inline double sign(const ddouble& x, double y)

{ return ( x < 0.0 ? -fabs(y) : fabs(y)); }



// f2c



inline ddouble d_abs(ddouble * x){ return abs(*x);	}

inline ddouble d_cos(ddouble * x){ return cos(*x);	}

inline ddouble d_sin(ddouble * x){ return sin(*x);	}

inline ddouble d_tan(ddouble * x){ return tan(*x);	}

inline ddouble d_exp(ddouble * x){ return exp(*x);	}

inline ddouble d_log(ddouble * x){ return log(*x);	}



inline ddouble d_sign(ddouble * x){ return sign(*x);	}

inline ddouble d_sign(ddouble * x,double*y){ return sign(*x,*y);	}



inline ddouble d_sqrt(ddouble * x){ return sqrt(*x);	}



inline ddouble pow_dd(ddouble * x,ddouble*y)	{ return pow(*x,*y);	}

inline ddouble pow_dd(double * x,ddouble*y) 	{ return pow(*x,*y);	}

inline ddouble pow_dd(ddouble * x,double*y) 	{ return pow(*x,*y);	}

inline ddouble pow_di(ddouble * x,int*y)	{ return pow(*x,*y);	}

		

#endif



