#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <cstdio>
#include <cmath>
using namespace std;

#include <fstream>
#include <iostream>
#include "sfem.hpp"
#include "RNM.hpp"

const R pi=4*atan(1.); 
using namespace std;

typedef  KN<R> Rn;
const int TheDrawList=1;  // numero de la list d'affichage 

class Global {    
// une petite classe pour stoker toutes les variables globales
public:
  int Width , Height;              // taille de l'écran en pixel
  Mesh &Th;                        // maillage courant
  R rapz;
  int nbiso;
  R * viso; // le valeu de iso value
  Rn &f ;                          // solution P1 a affich\'e
  R theta,phi,coef_dist;           // coordonnée polaire de la camera
  R dtheta;                        // vitesse de rotation de la camera 
  R xmin,xmax,ymin,ymax,zmin,zmax; // borne de la scène
  R xm,ym,zm; // point regarde
  
  Global(Mesh & TTh,Rn &ff,int height,int width,R rpz,int nbisovalue) ;
  void SetView() const;           // define le point de vue
  void DefaultView();
  void MoveXView(R dx,R dx);
  void MakeListDraw() const  ;    // construit la list d'affichage
} *global ;  // la variable global

  static int xold,yold;



Global::Global(Mesh & TTh,Rn &ff,int height,int width,R rpz,int nbisovalue) 
  : Th(TTh), f(ff)
{
  nbiso = nbisovalue;
  Width= width ;
  Height=height;
  rapz=rpz;
  
  // first compute the mesh bound
  const  Vertex & v0=Th(0);
  xmin=xmax=v0.x;
  ymin=ymax=v0.y;
  zmin = f[0], zmax=f[0];
  for (int i=0;i<Th.nv;i++)
    {
      const  Vertex & v=Th(i);
      xmin= Min(xmin,v.x);
      ymin= Min(ymin,v.y);
      xmax= Max(xmax,v.x);
      ymax= Max(ymax,v.y); 
      zmin= Min(zmin,f[i]);
      zmax= Max(zmax,f[i]);                   
    }
  if(nbiso>2)
    {
      viso = new R[nbiso];
      R diso=(zmax-zmin)/(nbiso-1);
      for (int i=0;i<nbiso;i++)
	viso[i]=zmin+i*diso;
    }
  else 
    viso=0;
  
   DefaultView();
  
}

void Global::DefaultView() 
{
  coef_dist =1;
  theta=45*pi/180.;
  dtheta=0;
  coef_dist=1;
  phi = 20*pi/180.;
   xm = (xmin+xmax)*0.5;
   ym = (ymin+ymax)*0.5;
    zm = rapz*(zmin+zmax)*0.5;    
}
void Global::SetView() const
{
  glViewport( 0, 0, Width, Height );
  
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity(); 
  R ratio= (double) Width / (double)  Height; 
  R dx =(xmax-xmin), dy= (ymax-ymin), dz=(zmax-zmin)*rapz;
  
  R hx= (  ratio*dy < dx  ) ? dx : dy*ratio ;
  R hy= hx/ratio ;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); 
  R focal=10;
  R znear=0.1;
  R zfare=100;
  R aspect=ratio;
  gluPerspective(focal,aspect,znear,zfare);
  R dist = Max(dx,dy,dz)/atan(focal*pi/180.)*coef_dist;
  R camx=xm+cos(phi)*cos(theta)*dist;
  R camy=ym+cos(phi)*sin(theta)*dist;
  R camz=zm+dist*sin(phi);     
  gluLookAt(camx,camy,camz,xm,ym,zm,0.,0.,1.);
  
  
}
  void Global::MoveXView(R dx,R dy)
  {
  // cout << xm << " " << ym << " " << zm << " apres " ;
   zm -= dy*(zmax-zmin)*rapz/50.;
   xm += dx*(xmax-xmin)*sin(theta)/50;
   ym -= dx*(ymax-ymin)*cos(theta)/50;   
  // cout << xm << " " << ym << " " << zm << endl;
  }


void hsvToRgb (float h, float s, float v, float & r, float & g, float & b)
{
  int i;
  float aa, bb, cc, f;
  
  if (s == 0) /* Grayscale */
    r = g = b = v;
  else {
    if (h == 1.0) h = 0;
    h *= 6.0;
    i =  int(h);
    f = h - i;
    aa = v * (1 - s);
    bb = v * (1 - (s * f));
    cc = v * (1 - (s * (1 - f)));
    switch (i) {
    case 0: r = v;  g = cc; b = aa; break;
    case 1: r = bb; g = v;  b = aa; break;
    case 2: r = aa; g = v;  b = cc; break;
    case 3: r = aa; g = bb; b = v;  break;
    case 4: r = cc; g = aa; b = v;  break;
    case 5: r = v;  g = aa; b = bb; break;
    }
  }
}

void   SetColor(R f)
{
  float r,g,b; 
  assert(global);
  R fmin=global->zmin; // borne de la fonction
  R fmax=global->zmax;
  
  hsvToRgb(0.99*(f-fmin)/(fmax-fmin),1,1,r,g,b);
  glColor3f(r,g,b);
}

void DrawVertex(const  R2 & v,R z=0,R rapz=1) 
{  
  SetColor(z);             // la couleur
  glVertex3f(v.x, v.y, z*rapz); // le sommet
}

void DrawIsoTfill(const R2 Pt[3],const R ff[3],const R * Viso,int NbIso, R rapz=1)
{
  R2 PQ[10];
  R z[10];
  
  R eps= (Viso[NbIso-1]-Viso[0])*1e-6;
  for(int l=1;l< NbIso;l++)  //   loop on the level curves 
    {
      R xfb = Viso[l-1];
      R xfh = Viso[l];
      assert(xfb < xfh);
      int im=0;
      for(int i=0;i<3;i++) // for the  3 edges 
	{
          int j=(i+1)%3;
          R fi=(ff[i]);
          R fj=(ff[j]);
          R xxfb =  xfb;
          R xxfh =  xfh;
	  if (fj<fi ) Exchange(xxfb,xxfh);
          R xf  = xxfb;
	  if(((fi<=xf)&&(fj>=xf))||((fi>=xf)&&(fj<=xf)))
	    {
	      if (Abs(fi-fj)>=0.1e-20)
		{
		  R  xlam=(fi-xf)/(fi-fj);
		  z[im] =  ff[i] * (1.F-xlam)  +  ff[j]* xlam;
		  PQ[im++]   = Pt[i] * (1.F-xlam)  +  Pt[j]* xlam;
		  
		}
	    }
          xf = xxfh;  
	  if(((fi<=xf)&&(fj>=xf))||((fi>=xf)&&(fj<=xf)))
	    {
	      if (Abs(fi-fj)>=0.1e-20)
		{
		  R  xlam=(fi-xf)/(fi-fj);
		  z[im] =  ff[i] * (1.F-xlam)  +  ff[j]* xlam;
		  PQ[im++]   = Pt[i] * (1.F-xlam)  +  Pt[j]* xlam;
		}
	    }
	  if (  xfb-eps <=fj  && fj <= xfh+eps) 
	    z[im]=ff[j],PQ[im++] = Pt[j];
	  
	}
      if (im>2) 
	{
          glBegin(GL_POLYGON);
          SetColor((xfb+xfh)/2); 
	  for (int i=0;i<im;i++)
	    {// cout << i << " \t : " << PQ[i].x << " " <<  PQ[i].y << " " << z[i]*rapz << endl;
	      glVertex3f(PQ[i].x, PQ[i].y,z[i]*rapz);
	    }
          glEnd();
	  
	}
    }
} 


void Global::MakeListDraw() const
{
  
  glNewList(TheDrawList,GL_COMPILE); // save  la list sans affichage
  R fmn=zmin, fmx=zmax; 
  
  glPolygonMode(GL_FRONT,GL_FILL); // mode affichage des polygones  
  // constructions des triangles colorés
  if (nbiso)
    {
      for (int i=0;i<Th.nt;i++)
        {
          const Triangle & K(Th[i]);
	  R2 Pt[3]={K[0],K[1],K[2]};
	  R ff[3]={f[Th(K[0])],f[Th(K[1])],f[Th(K[2])]};
	  DrawIsoTfill(Pt,ff,viso,nbiso,rapz);
	}
    }
  else
    {
      for (int i=0;i<Th.nt;i++)
	{
	  const Triangle & K(Th[i]); 
	  int i0= Th(K[0]),  i1= Th(K[1]),   i2= Th(K[2]) ;    
	  glBegin(GL_TRIANGLES);
	  DrawVertex(K[0],f[i0],rapz);
	  DrawVertex(K[1],f[i1],rapz);
	  DrawVertex(K[2],f[i2],rapz);
	  glEnd();
	}
    }
  glEndList();  // fin de la list
}

void Clean() 
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

static void Reshape( int width, int height )
{   
  global->Width  = width;
  global->Height = height;  
  global->SetView();
  glutPostRedisplay();
}


static void Key( unsigned char key, int x, int y )
{
  switch (key) {
  case 27: // esc char
    exit(0);
    break;
  case '+':  
    global->coef_dist /= 1.2;
    break;
  case '-':  
    global->coef_dist *= 1.2;
    break;
  case 'g':  
    global->theta += pi/180.;
    break;
  case 'd':  
    global->theta -= pi/180.;
    break;
  case 'h':  
    global->phi += pi/180.;
    break;
  case 'b':  
    global->phi -= pi/180.;
    break;
  case 'a':
    global->dtheta = pi/180.;
    break;	
  case 's':
    global->dtheta = 0;
    break;	
  case '=':
    global->DefaultView();
    break;
  default:
    cout << " Key Character " << (int) key << " " << key << endl;  
    
  }
  global->SetView();  
  glutPostRedisplay();
}

void Display(void)
{ 
  Clean();
  glCallList(TheDrawList);    
  glFlush();    
  glutSwapBuffers();
}

static void Idle( void )
{
  if (global->dtheta)
    {
      global->theta += pi/180.;
      global->SetView();
      glutPostRedisplay();
    }
  
}
static void Mouse( int button,int state,int x,int y )
{
  // state up or down 
  if (state == GLUT_DOWN) { xold=x,yold=y;return;}
  // cout << "Mouse " << button<< " " << state << " " << x-xold << " " << y-yold << endl;
  //  x gauche -> droitre
  //  y  haut -> bas`
  global->phi += (y-yold)/(2.*180.);
  global->theta -= (x-xold)/(2*180.);
  global->SetView();  
  glutPostRedisplay();
  
}
static void MotionMouse(int x,int y )
{
 // cout << " MotionMouse " << " " << x << " " << y << endl;
  GLuint gtime = glutGet(GLUT_ELAPSED_TIME); //   
  global->phi += (y-yold)/(2.*180.);
  global->theta -= (x-xold)/(2*180.);
   xold=x;
   yold=y;
  global->SetView();  
  glutPostRedisplay();
}
void SpecialKey(int key, int x, int y)
{
 // cout << " SpecialKey " << key << " " << x << " " << y << " : ";
  R dx(0),dy(0);
    switch (key) {
        case  GLUT_KEY_LEFT:   dx = -1; break;
        case  GLUT_KEY_RIGHT:  dx = +1; break;
        case  GLUT_KEY_DOWN:   dy = -1; break;
        case  GLUT_KEY_UP:     dy = +1; break;
}
   // calcul du deplacement de xm,ym,zm;
  // cout << " " << dx << " " << dy << endl;
    global->MoveXView(dx,dy);
    global->SetView();  
  glutPostRedisplay();

}
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  if (argc <3)
   {
      cerr << " utilisation : " << argv[0] << " meshfile  solfile [rap in z ] [nbisovalue] " << endl;
      return 1;
   }
  assert(argc>2);
  R rapz=1; 
  int nbiso=20;
  if (argc>3) rapz=atof(argv[3]);
  if (argc>4) nbiso=atoi(argv[4]);
  cout << " Rap z " << rapz << endl;
  Mesh Th(argv[1]);
  Rn f(Th.nv); 
  {
    ifstream fdat(argv[2]);
    assert(fdat.good());
    fdat >> f;
  } // pour ferme le fichier (la variable fdat est detruite)
  
  
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  int Height = 512;
  int Width = 512; 
  glutInitWindowSize(Width , Height);
  glutInitWindowPosition(100, 100);
  string titre = "vue de ";
  titre += argv[1] ;
  titre += ", ";
  titre += argv[2];
  glutCreateWindow(titre.c_str());
  glutPushWindow();
  
  global=new Global(Th,f,Height,Width,rapz,nbiso);
  global->MakeListDraw();    
  global->SetView(); 
  
  glEnable(GL_DEPTH_TEST); 
  glutReshapeFunc( Reshape ); // pour changement de fenetre 
  glutKeyboardFunc( Key );    // pour les evenements clavier
  glutSpecialFunc(SpecialKey);
  glutMouseFunc(Mouse);       // pour les evenements sourie
  glutMotionFunc(MotionMouse); // les mouvements  de la sourie 
  glutDisplayFunc( Display ); // l'affichage
  glutIdleFunc( Idle );       // l'animation automatique
  
  glutMainLoop(); 
  return 0;
}


