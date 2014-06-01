#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
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
 Rn &f ;                          // solution P1 a affich\'e
 R theta,phi,coef_dist;           // coordonnée polaire de la camera
 R dtheta;                        // vitesse de rotation de la camera 
 R xmin,xmax,ymin,ymax,zmin,zmax; // borne de la scène
 
  Global(Mesh & TTh,Rn &ff,int height,int width,R rpz) ;
  void SetView() const;           // define le point de vue
  void MakeListDraw() const  ;    // construit la list d'affichage
} *global ;  // la variable global




Global::Global(Mesh & TTh,Rn &ff,int height,int width,R rpz) 
   : Th(TTh), f(ff)
  {
   Width= width ;
   Height=height;
   theta=45*pi/180.;
   dtheta=0;
   coef_dist=1;
   rapz=rpz;
   phi = 20*pi/180.;
   
   // first compute the mesh bound
   const  Vertex & v0=Th(0);
   xmin=xmax=v0.x;
   ymin=ymax=v0.y;
   zmin = rapz*f[0], zmax=rapz*f[0];
   for (int i=0;i<Th.nv;i++)
     {
       const  Vertex & v=Th(i);
       xmin= Min(xmin,v.x);
       ymin= Min(ymin,v.y);
       xmax= Max(xmax,v.x);
       ymax= Max(ymax,v.y); 
       zmin= Min(zmin,f[i]*rapz);
       zmax= Max(zmax,f[i]*rapz);                   
     }
   }
 
   
void Global::SetView() const
 {
     glViewport( 0, 0, Width, Height );

     glMatrixMode(GL_PROJECTION); 
     glLoadIdentity(); 
     R ratio= (double) Width / (double)  Height; 
     R dx =(xmax-xmin), dy= (ymax-ymin), dz=(zmax-zmin);
         
     R hx= (  ratio*dy < dx  ) ? dx : dy*ratio ;
     R hy= hx/ratio ;
     R xm = (xmin+xmax)*0.5;
     R ym = (ymin+ymax)*0.5;
     R zm = (zmin+zmax)*0.5;
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

 void DrawVertex(const  R2 & v,R z=0) 
{  
  SetColor(z);             // la couleur
  glVertex3f(v.x, v.y, z); // le sommet
}
 
void Global::MakeListDraw() const
{

  glNewList(TheDrawList,GL_COMPILE); // save  la list sans affichage
  R fmn=zmin, fmx=zmax; 
  
  glPolygonMode(GL_FRONT,GL_FILL); // mode affichage des polygones  
  // constructions des triangles colorés
  for (int i=0;i<Th.nt;i++)
  {
    const Triangle & K(Th[i]); 
    int i0= Th(K[0]),  i1= Th(K[1]),   i2= Th(K[2]) ;    
    glBegin(GL_TRIANGLES);
    DrawVertex(K[0],rapz*f[i0]);
    DrawVertex(K[1],rapz*f[i1]);
    DrawVertex(K[2],rapz*f[i2]);
    glEnd();
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
        global->coef_dist =1;
        global->theta = 45;
        global->phi = 45;
        break;
        
      	
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
 // cout << "Mouse " << button<< " " << state << " " << x << " " << y << endl;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    int Height = 512;
    int Width = 512; 
    glutInitWindowSize(Width , Height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mesh");
    glutPushWindow();
    assert(argc>2);
    R rapz=1; 
    if (argc>3) rapz=atof(argv[3]);
    cout << " Rap z " << rapz << endl;
    Mesh Th(argv[1]);
    Rn f(Th.nv); 
    {
     ifstream fdat(argv[2]);
     assert(fdat.good());
     fdat >> f;
    } // pour ferme le fichier (la variable fdat est detruite)
    
    global=new Global(Th,f,Height,Width,rapz);
    global->MakeListDraw();    
    global->SetView(); 
    
    glEnable(GL_DEPTH_TEST); 
    glutReshapeFunc( Reshape ); // pour changement de fenetre 
    glutKeyboardFunc( Key );    // pour les evenements clavier
    glutMouseFunc(Mouse);
    glutDisplayFunc( Display ); // l'affichage
    glutIdleFunc( Idle );       // l'animation automatique
    
    glutMainLoop(); 
    return 0;
}


