#include <cstdio>
#include <cstring>
#include <cmath> 

#include <iostream>

namespace bamg {
using namespace std;


template <class R,class RR>
class P2 {
  
public:  
  R x,y;
  P2 () :x(0),y(0) {};
  P2 (R a,R b)  :x(a),y(b)  {}
  P2<R,RR>   operator+(const P2<R,RR> & cc) const 
                    {return P2<R,RR>(x+cc.x,y+cc.y);}
  P2<R,RR>   operator-(const P2<R,RR> & cc) const 
                    {return P2<R,RR>(x-cc.x,y-cc.y);}
  P2<R,RR>   operator-()  const{return P2<R,RR>(-x,-y);}
  RR   operator,(const P2<R,RR> & cc) const // produit scalaire
                     {return  (RR) x* (RR) cc.x+(RR) y* (RR) cc.y;} 
  P2<R,RR>  operator*(R  cc) const 
                     {return P2<R,RR>(x*cc,y*cc);}
  P2<R,RR>  operator/(R  cc) const 
                      {return P2<R,RR>(x/cc,y/cc);}
  P2<R,RR>  operator+=(const  P2<R,RR> & cc) 
                      {x += cc.x;y += cc.y;return *this;}
  P2<R,RR>  operator/=(const  R r) 
                      {x /= r;y /= r;return *this;}
  P2<R,RR>  operator*=(const  R r) 
                      {x *= r;y *= r;return *this;}
  P2<R,RR>  operator-=(const  P2<R,RR> & cc) 
                      {x -= cc.x;y -= cc.y;return *this;}
 };

template  <class R,class RR>  
inline RR Det(const P2<R,RR> x,const P2<R,RR> y) {
  return (RR) x.x * (RR) y.y - (RR) x.y * (RR) y.x ;} 

template  <class R,class RR>  
inline RR Area2 (const P2<R,RR> a,const P2<R,RR> b,const P2<R,RR> c) {
  return Det(b-a,c-a) ;} 

template <class R,class RR>
inline  ostream& operator <<(ostream& f, const P2<R,RR> & c)
  { f <<  c.x << "   " << c.y << ' ' ; return f; }

} 
