// la regle de programmation 3
#include "assertion.hpp" 
#include <cstdlib>
// some usefull function
template<class T> inline T Min (const T &a,const T &b){return a < b ? a : b;}
template<class T> inline T Max (const T &a,const T & b){return a > b ? a : b;}
template<class T> inline T Abs (const T &a){return a <0 ? -a : a;}
template<class T> inline double Norme (const T &a){return sqrt(a*a);}
template<class T> inline void Exchange (T& a,T& b) {T c=a;a=b;b=c;}
template<class T> inline T Max (const T &a,const T & b,const T & c){return Max(Max(a,b),c);}
template<class T> inline T Min (const T &a,const T & b,const T & c){return Min(Min(a,b),c);}

using namespace std;
// definition R
typedef double R;

// The class R2
class R2 {
  friend ostream& operator <<(ostream& f, const R2 & P )
  { f << P.x << ' ' << P.y   ; return f; }
  friend istream& operator >>(istream& f,  R2 & P)
  { f >>  P.x >>  P.y  ; return f; }
  
public:  
  R x,y;
  R2 () :x(0),y(0) {}
  R2 (R a,R b):x(a),y(b)  {}
  R2 (R2 a,R2 b):x(b.x-a.x),y(b.y-a.y)  {}
  R2   operator+(R2 P)const   {return R2(x+P.x,y+P.y);}
  R2   operator+=(R2 P)  {x += P.x;y += P.y;return *this;}
  R2   operator-(R2 P)const   {return R2(x-P.x,y-P.y);}
  R2   operator-=(R2 P) {x -= P.x;y -= P.y;return *this;}
  R2   operator-()const  {return R2(-x,-y);}
  R2   operator+()const  {return *this;}
  R   operator,(R2 P)const  {return  x*P.x+y*P.y;} // produit scalaire
  R   operator^(R2 P)const {return  x*P.y-y*P.x;} // produit mixte
  R2   operator*(R c)const {return R2(x*c,y*c);}
  R2   operator/(R c)const {return R2(x/c,y/c);}
  R2   perp() {return R2(-y,x);} // the perpendiculare
};
inline R2 operator*(R c,R2 P) {return P*c;}

class Label {  // reference number for the physics
  friend ostream& operator <<(ostream& f,const Label & r  )
  { f <<  r.lab ; return f; }
  friend istream& operator >>(istream& f, Label & r  )
  { f >>  r.lab ; return f; }
public: 
  int lab;
  Label(int r=0):lab(r){}
  int onGamma() const { return lab;} 
};

class Vertex : public R2,public Label {
public:
  friend ostream& operator <<(ostream& f, const Vertex & v )
  { f << (R2) v << ' ' << (Label &) v   ; return f; }
  friend istream& operator >> (istream& f,  Vertex & v )
  { f >> (R2 &) v >> (Label &) v ; return f; }
  Vertex() : R2(),Label(){};
  Vertex(R2 P,int r=0): R2(P),Label(r){}
private:
  Vertex(const Vertex &);
  void operator=(const Vertex &);
};

class BoundaryEdge: public Label {
public:
  Vertex *vertices[2];
  void set(Vertex * v0,int i0,int i1,int r)
  { vertices[0]=v0+i0; vertices[1]=v0+i1; lab=r; }
  bool in(const Vertex * pv) const {return pv == vertices[0] || pv == vertices[1];}
  BoundaryEdge(){}; // constructor empty for array 
  void Draw() const;
  Vertex & operator[](int i) const {ASSERTION(i>=0 && i <2);
  return *vertices[i];}
  R length() const { R2 AB(*vertices[0],*vertices[1]);return sqrt((AB,AB));}
private:
  BoundaryEdge(const BoundaryEdge &); // pas de construction par copie
  void operator=(const BoundaryEdge &);// pas affectation par copy 
};

class Triangle: public Label {
  Vertex *vertices[3]; // an array of 3 pointer to vertex
public:
  R area;
  Triangle(){}; // constructor empty for array
  Vertex & operator[](int i) const {
    ASSERTION(i>=0 && i <3);
    return *vertices[i];} // to see traingle as a array of vertex
  void set(Vertex * v0,int i0,int i1,int i2,int r) {
    vertices[0]=v0+i0; vertices[1]=v0+i1; vertices[2]=v0+i2; 
    R2 AB(*vertices[0],*vertices[1]);
    R2 AC(*vertices[0],*vertices[2]);
    area = (AB^AC)*0.5;
    lab=r;
    ASSERTION(area>=0); }
  
  R2 Edge(int i) const {
    ASSERTION(i>=0 && i <3);
    return R2(*vertices[(i+1)%3],*vertices[(i+2)%3]);}// opposite edge vertex i
  R2 H(int i) const { ASSERTION(i>=0 && i <3);
  R2 E=Edge(i);return E.perp()/(2*area);} // heigth 
  R lenEdge(int i) const {
    ASSERTION(i>=0 && i <3);
    R2 E=Edge(i);return sqrt((E,E));}
private:
  Triangle(const Triangle &); // pas de construction par copie
  void operator=(const Triangle &);// pas affectation par copy 
  
};

class Mesh { public:
  int nt,nv,neb;
  R area;
  Vertex *vertices;
  Triangle *triangles;
  BoundaryEdge  *bedges;  
  
  Triangle & operator[](int i) const {return triangles[CheckT(i)];}
  Vertex & operator()(int i) const {return vertices[CheckV(i)];}
  inline Mesh(const char * filename); // read on a file
  int operator()(const Triangle & t) const {return CheckT(&t - triangles);}
  int operator()(const Triangle * t) const {return CheckT(t - triangles);}
  int operator()(const Vertex & v) const {return CheckV(&v - vertices);}
  int operator()(const Vertex * v) const{return CheckT(v - vertices);}
  int operator()(int it,int j) const {return (*this)(triangles[it][j]);}// Nu vertex j of traingle it
  //  to check the bound 
  int CheckV(int i) const { ASSERTION(i>=0 && i < nv); return i;} 
  int CheckT(int i) const { ASSERTION(i>=0 && i < nt); return i;}
  Triangle & GammaT(int ieb, int & e) {int  k=kBoundaryEdgeT[ieb]; e=k%3; return triangles[k/3];}
private:
  int * kBoundaryEdgeT; 
  Mesh(const Mesh &); // pas de construction par copie
  void operator=(const Mesh &);// pas affectation par copy 
};


inline Mesh::Mesh(const char * filename)
{ // read the mesh
  int i,i0,i1,i2,ir;
  ifstream f(filename);
  if(!f) {cerr << "Mesh::Mesh Erreur openning " << filename<<endl;exit(1);}
  cout << " Read On file \"" <<filename<<"\""<<  endl;
  f >> nv >> nt >> neb ;
  cout << " Nb of Vertex " << nv << " " << " Nb of Triangles " << nt << " Nb d'arete frontiere " << neb << endl;
  assert(f.good() && nt && nv) ;
  triangles = new Triangle [nt];
  vertices = new Vertex[nv];
  bedges = new BoundaryEdge[neb];
  kBoundaryEdgeT = new int[neb];
  area=0;
  assert(triangles && vertices);
  for (i=0;i<nv;i++)    
    f >> vertices[i],assert(f.good());
  for (i=0;i<nt;i++) { 
    f >> i0 >> i1 >> i2 >> ir;
    assert(f.good() && i0>0 && i0<=nv && i1>0 && i1<=nv && i2>0 && i2<=nv);
    triangles[i].set(vertices,i0-1,i1-1,i2-1,ir); 
    area += triangles[i].area;}
  
  for (i=0;i<neb;i++) { 
    f >> i0 >> i1  >> ir;
    assert(f.good() && i0>0 && i0<=nv && i1>0 && i1<=nv );
    bedges[i].set(vertices,i0-1,i1-1,ir); 
    
  }
  //  construction du tableau  kBoundaryEdgeT [e] = 3*k + i
  // ou l'arete e est l'arete i du triangle k
  //  1) construction des chaines  de  arete ayant meme petit numero
  // Merci pour le chapitre  chaine et chainage 
  { 
    Mesh & Th(*this); 
    int * head = new int[nv];
    int * next = new int[neb];
    for (int i=0;i<nv;i++) head[i]=-1;
    for (int j=0;j<neb;j++) { 
      kBoundaryEdgeT[j]=-1; // indefini
      int i= Min(Th(bedges[j][0]),Th(bedges[j][1]));
      next[j]= head[i];
      head[i]=j; }
    int nb=0; 
    for (int k=0;k<nt;k++)
      for (int i=0;i<3;i++)
        {
          int i1= Th(k,(i+1)%3);
          int i2= Th(k,(i+2)%3);
          if (i1>i2) Exchange(i1,i2);
          int i12 = i1+i2;
          //  recherche de l'arete i1,i2 i1 < i2
          for (int e=head[i1];e!=-1; e=next[e])
            if ( Th(bedges[e][0]) + Th(bedges[e][1]) == i12 )
              {  //  on a trouve  un arete frontiere
                if (kBoundaryEdgeT[e] < 0) 
                  { 
                    nb++;
                    kBoundaryEdgeT[e]= 3*k+i;
                  }
                break;
              } 
        }
    delete [] next;
    delete [] head;
    if ( nb != neb) {         
      cerr << "Erreur  lire mesh:, il y a des " << neb-nb 
           << " aretes frontiere que ne sont dans aucun triangles \n";
      exit(1);
    }
    
  } 
   cout << " End of read: area = " << area <<endl;  
} 
