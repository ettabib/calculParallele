#ifndef __FONCTION__
#define __FONCTION__

struct CVirt {
  mutable CVirt *md;              //  le pointeur sur la fonction dérivée
  CVirt () : md (0) {}
  virtual R operator () (R) = 0;
  virtual CVirt *de () {return zero;}
  CVirt *d () {if (md == 0) md = de (); return md;}
  static CVirt *zero;
};

class Fonction {                              // Fonction} d'une variable
  CVirt *p;
public:
  operator CVirt *() const {return p;}
  Fonction (CVirt *pp) : p(pp) {}
  Fonction (R (*) (R));             // Création à partir d'une fonction C
  Fonction (R x);                                   // Fonction constante
  Fonction (const Fonction& f) : p(f.p) {}            //Constructeur par copie
  Fonction d () {return p->d ();}                              // Dérivée
  void setd (Fonction f) {p->md = f;}
  R operator() (R x) {return (*p)(x);}              // Valeur en un point
  Fonction operator() (Fonction);             // Composition de fonctions
  friend class Fonction2;
  Fonction2 operator() (Fonction2);
  static Fonction monome (R, int);
};

struct CVirt2 {
  CVirt2 ():  md1 (0), md2 (0) {}
  virtual R operator () (R, R) = 0;
  virtual CVirt2 *de1 () {return zero2;}
  virtual CVirt2 *de2 () {return zero2;}
  CVirt2 *md1, *md2;
  CVirt2 *d1 () {if (md1 == 0) md1 = de1 (); return md1;}
  CVirt2 *d2 () {if (md2 == 0) md2 = de2 (); return md2;}
  static CVirt2 *zero2;
};

class Fonction2 {                          // Fonction de deux variables
  CVirt2 *p;
public:
  operator CVirt2 *() const {return p;}
  Fonction2 (CVirt2 *pp) : p(pp) {}
  Fonction2 (R (*) (R, R));        // Création à partir d'une fonction C
  Fonction2 (R x);                                 // Fonction constante
  Fonction2 (const Fonction2& f) : p(f.p) {}         //Constructeur par copie
  Fonction2 d1 () {return p->d1 ();}
  Fonction2 d2 () {return p->d2 ();}
  void setd (Fonction2 f1, Fonction2 f2) {p->md1 = f1; p->md2 = f2;}
  R operator() (R x, R y) {return (*p)(x, y);}
  friend class Fonction;
  Fonction operator() (Fonction, Fonction);  // Composition de fonctions
  Fonction2 operator() (Fonction2, Fonction2);
  static Fonction2 monome (R, int, int);
};

extern Fonction Chs,Identity;
extern Fonction2 Add, Sub, Mul, Div, Abscisse, Ordonnee;

inline Fonction operator+ (Fonction f, Fonction g) {return Add(f, g);}
inline Fonction operator- (Fonction f, Fonction g) {return Sub(f, g);}
inline Fonction operator* (Fonction f, Fonction g) {return Mul(f, g);}
inline Fonction operator/ (Fonction f, Fonction g) {return Div(f, g);}
inline Fonction operator- (Fonction f) {return Chs(f);}

inline Fonction2 operator+ (Fonction2 f, Fonction2 g) {return Add(f, g);}
inline Fonction2 operator- (Fonction2, Fonction2 g) {return Sub(f, g);}
inline Fonction2 operator* (Fonction2 f, Fonction2 g) {return Mul(f, g);}
inline Fonction2 operator/ (Fonction2 f, Fonction2 g) {return Div(f, g);}
inline Fonction2 operator- (Fonction2 f) {return Chs(f);}

#endif
