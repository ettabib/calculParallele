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



typedef  KN<R> Rn;

 

int Width , Height;



using namespace std;

Mesh *pTh;





inline void glVertex(const  R2 & v,R z=0) {  

  glVertex3f(v.x, v.y, z);

}



void SetView(const Mesh & Th) 

 {

   // on calcule d'abord les limites du maillage

    R xmin,xmax,ymin,ymax;

    const  Vertex & v0=Th(0);

    xmin=xmax=v0.x;

    ymin=ymax=v0.y;



    for (int i=0;i<Th.nv;i++)

     {

       const  Vertex & v=Th(i);

       xmin= Min(xmin,v.x);

       ymin= Min(ymin,v.y);

       xmax= Max(xmax,v.x);

       ymax= Max(ymax,v.y); 

     }

    // on choisit le mode de visualisation

     glMatrixMode(GL_PROJECTION); 

     glLoadIdentity(); 

     R ratio= (double) Width / (double)  Height; 

     R dx =(xmax-xmin), dy= (ymax-ymin);// la taille du maillage



     //  attention, pour respecter le rapport d'aspect et que l'objet

     //  soit enti{\`e}rement vu,  il faut changer la longueur de reference

     //  si le  (ratio < dx/dy) alors dx est la ref. sinon dy

         

     R hx= (  ratio*dy < dx  ) ? dx : dy*ratio ;

     R hy= hx/ratio ;          //  == hy si ratio >= dx/dy

     R xm = (xmin+xmax)*0.5;   // milieu  du maillage

     R ym = (ymin+ymax)*0.5;



     cout <<(  ratio*dy < dx  )  << ", " <<  Width << "x" 

          << Height << ", x min max = " << xmin << " " << xmax 

          << ", y min max = " << ymin << " " << ymax <<  " " 

          << xm << " " << ym << " hx = " << hx << " " << hy 

          << endl;

     

      // on ajoute 10\% pour tout voir => coef: $ 0.55 = {1.1\over 2}$

     gluOrtho2D(xm-hx*0.55,xm+hx*0.55, ym-hy*0.55, ym+hy*0.55);

     

 }

 

void DrawMesh(const Mesh & Th)

{

  glColor3f(0.,0.,0.);

  glPolygonMode(GL_FRONT,GL_LINE);  // trace seulement le bord des polygones

  for (int i=0;i<Th.nt;i++)

  {

    const Triangle & K(Th[i]);

    glBegin(GL_TRIANGLES);

    glVertex(K[0]);

    glVertex(K[1]);

    glVertex(K[2]);

    glEnd();

   }

 glFlush();

  

}

void Clean() 

{

    glClearColor(1.0, 1.0, 1.0, 0.0);  // couleur du fond =  blanc en RGB

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

}

static void Reshape( int width, int height )

{

   Width  = width;

   Height = height;

    glViewport( 0, 0, width, height );

   if(pTh) 

     SetView(*pTh);

    glutPostRedisplay();  //  pour redessiner

}





static void Key( unsigned char key, int x, int y )

{

   (void) x;

   (void) y;

   switch (key) {

      case 27: // esc char

         exit(0);

         break;

   }

   glutPostRedisplay();

}



void Display(void)

{

    Clean();                //nettoye l'écran

    if(pTh) DrawMesh(*pTh); // dessine le maillage

    glFlush();              // vide les tampons GL

    // seulement si GLUT\_DOUBLE mode est utilisé

    glutSwapBuffers();     // affiche le buffer du dessin

}

//static void Idle( void )

//{

//   glutPostRedisplay();

//}



int main(int argc, char** argv)

{

    glutInit(&argc, argv);

    //  mode graphique   couleur

    //  double buffer d'affichage

    //  Z buffer

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );

    Height = 512;

    Width = 512; 

    glutInitWindowSize(Width , Height);

    glutInitWindowPosition(100, 100);

    glutCreateWindow("Mesh");

    glutPushWindow();



       assert(argc>1);

    // lecture du maillage

    Mesh Th(argv[1]);

    pTh=&Th;

    SetView(Th);



    glutReshapeFunc( Reshape ); // si la fenetre graphique change 

    glutKeyboardFunc( Key );    // pour les événements clavier

    glutDisplayFunc( Display ); // pour l'affichage

    //    glutIdleFunc( Idle ); // 

    

    glutMainLoop();

    return 0;

}





