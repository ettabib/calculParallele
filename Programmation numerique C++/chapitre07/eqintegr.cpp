#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

typedef struct {int v[3]; double area;} triangle;
typedef struct { double x,y,z;} vertex;
typedef struct { triangle* t; vertex* v;
				 int nv,nt;	} triangulation;



double 	norm(vertex* p);
void 	readmesh(triangulation* g, char* path);
double 	getarea(vertex q1, vertex q2, vertex q3);
void 	buildmat(triangulation* g, double* a);



double 	norm(vertex* p)
{
	return sqrt(p->x*p->x + p->y*p->y + p->z*p->z);
}



void readmesh(triangulation* g, char* path)
{
  int i,j,jp,jpp,aux;

  char *filename = new char[256];
  strcpy(filename,path); 
  strcat(filename,".points");

  cout<<"fichier .points : "<< filename<<endl;
  ifstream ff(filename);
  ff>>g->nv;
  g->v = new vertex[g->nv];

  for(i=0;i<g->nv;i++)
  	ff>>g->v[i].x>>g->v[i].y>>g->v[i].z>>aux;

  ff.close();

  strcpy(filename,path); 
  strcat(filename,".faces");
  cout<<"fichier .faces : "<< filename<<endl;

  ff.open(filename);
  ff>>g->nt;
  g->t = new triangle[g->nt];

  for( i=0; i<g->nt; i++ ) 
  {
  	ff>>aux>>j>>jp>>jpp>>aux>>aux>>aux>>aux;

  	 g->t[i].v[0] = j-1;
  	 g->t[i].v[1] = jp-1;
  	 g->t[i].v[2] = jpp-1;
      	g->t[i].area = getarea(g->v[j-1],g->v[jp-1],g->v[jpp-1]);
  }
}


double getarea(vertex q1, vertex q2, vertex q3)
{ 
	vertex s;

	s.x = (q2.x - q1.x)*(q3.y-q1.y) - (q3.x - q1.x)*(q2.y - q1.y);
	s.y = (q2.y - q1.y)*(q3.z-q1.z) - (q3.y - q1.y)*(q2.z - q1.z);
	s.z = (q2.z - q1.z)*(q3.x-q1.x) - (q3.z - q1.z)*(q2.x - q1.x);

	return norm(&s)/2;
}



void 	buildmat(triangulation* g, double* a)
{
	int i,j,k,i0,i1,i2,j0,j1,j2; 

	vertex p,q;

	for(i=0;i<g->nt*g->nt;i++) a[i]=0;
	for(i=0;i<g->nt;i++)
	{
		i0 = g->t[i].v[0]; i1 = g->t[i].v[1]; i2 = g->t[i].v[2];
		for(j=0;j<g->nt;j++)
		{
			j0 = g->t[j].v[0]; j1 = g->t[j].v[1]; j2 = g->t[j].v[2];
			q.x = (g->v[i0].x + g->v[i1].x + g->v[i2].x)/3;
			q.y = (g->v[i0].y + g->v[i1].y + g->v[i2].y)/3;
			q.z = (g->v[i0].z + g->v[i1].z + g->v[i2].z)/3;

			if(i!=j)
			{	
			  p.x = q.x - ( g->v[j0].x + g->v[j1].x + g->v[j2].x )/3;
			  p.y = q.y - ( g->v[j0].y + g->v[j1].y + g->v[j2].y )/3;
			  p.z = q.z - ( g->v[j0].z + g->v[j1].z + g->v[j2].z )/3;
			  a[i*g->nt+j] = g->t[i].area * g->t[j].area /norm(&p);

			} else 

			   for(k=0;k<3;k++)
			   {
				 j0 = g->t[j].v[k];
				 p.x = (q.x-g->v[j0].x)/3;
				 p.y = (q.y-g->v[j0].y)/3;
				 p.z = (q.z-g->v[j0].z)/3;
				a[i*g->nt+i] +=g->t[i].area * g->t[i].area /norm(&p)/3;			
			   }
	    }
	}
}





double gauss(triangulation* g, double* a, double* b)
{

  int i,j,k, N = g->nt;
  double s, s1, s2;
  double saux, smin = double(1e20), eps = 1/smin;

  for (i=0;i<N;i++) 
  {
      for(j=0;j<=i;j++)
	{
	  s=0; for (k=0; k<j;k++)  s += a[i*N+k]*a[k*N+j] ;
	  a[i*N+j] -= s ;
	}

      for(j=i+1;j<N;j++)
	{
	  s=0; for (k=0;k<i;k++)  s += a[i*N+k]*a[k*N+j] ;
	  s1 = a[i*N+i];

	  if(saux=s1*s1, saux< smin) smin=saux;
	  if(saux < eps) {s1 = eps;cout<<"small subpivot"<<s1<<endl;}
	  a[i*N+j] = ( a[i*N+j] - s)/s1;
	}
  }

  for (i=0;i<N;i++)							
  {	
      s2=0; for (k=0;k<i;k++)  s2 += a[i*N+k] * b[k];
      b[i] = (b[i] - s2) /  a[i*N+i] ;
  }

  for (i=N-1;i>=0;i--)
  {
      s2=0; for (k=i+1; k<=N-1;k++)  s2 += a[i*N+k] * b[k];
      b[i] -= s2 ;
  }

  return smin;	
}


// La fonction qui sauvegarde le maillage 3D pour gnuplot
// ------------------------------------------
void gnuplot_mesh3d(char * filename, triangulation & g)
{  
	ofstream solg(filename);  // maillage
	for (int i=0;i<g.nt;i++)
	   {	for (int j=0;j<=3;j++)
	          solg<< g.v[g.t[i].v[j%3]].x <<"\t"
                      << g.v[g.t[i].v[j%3]].y <<"\t"
                      << g.v[g.t[i].v[j%3]].z <<endl;
                 
	          solg<<endl<<endl;
           }
        solg.close();
}


// La fonction qui sauvegarde les iso-lignes 3D pour gnuplot
// ------------------------------------------
void gnuplot_isos3d(char * fileiso, triangulation & g, double * U ,int niso)
{  
 ofstream f(fileiso); 

 // sélection des valeurs des iso-lignes
 double umax=0., umin=0.;
 for(int i=0;i<g.nv;i++)
   { umax=max(U[i],umax);
     umin=min(U[i],umin);
   }
   
 if((umax == umin) | (niso == 0)) 
   niso=1;

for(int imu=0;imu<=niso;imu++)
{   double muc=umin+(umax-umin)*imu/niso;
    cout<<"Isoligne No "<<imu<<" = "<<muc<<endl;

    for(int k = 0; k<g.nt; k++)
    { int m=0;
      for(int i1=0;i1<3;i1++)
	{ int i = g.t[k].v[i1%3];
          int j = g.t[k].v[(i1+1)%3];
        double a=2.;
        if((U[i] - U[j])!=0)
          a = (muc - U[j]) / (U[i] - U[j]);
	else
	 {if(muc ==U[j])
	   {f<<g.v[i].x <<"\t"<< g.v[i].y <<"\t"<< g.v[i].z <<endl;
	     a=0.;}
         }

        if((a>=0)&&(a<=1))
        { m++;
          f<<a*g.v[i].x + (1-a)*g.v[j].x<<"\t"<<
             a*g.v[i].y + (1-a)*g.v[j].y<<"\t"<<
	     a*g.v[i].z + (1-a)*g.v[j].z<<endl;
        }
       }
      if(m)
        f<<endl<<endl;
    }
}
    f.close();
}


int main( void )
{
	triangulation g;

	double pi = 4.*atan(1.);

	char* path= new char[256];
	char* filename= new char[256];
        strcpy(path,"sphere");

	

	readmesh(&g,path);
	double *b = new double[g.nt], *a = new double[g.nt*g.nt];

	for(int i =0;i<g.nt;i++)
		b[i] = 4*pi*g.t[i].area * cos(pi*( g.v[g.t[i].v[0]].x
				 +g.v[g.t[i].v[1]].x + g.v[g.t[i].v[2]].x)/3);

	buildmat(&g,a);
	
	/*for (int i=0;i<5;i++)
	{	for (int j=0;j<10;j++) cout << a[i*g.nt+j]<<'\t';
		cout<<endl<<endl<<endl;
	}*/
        
	printf("%i",gauss(&g,a,b));cout<<endl;
        

	//interpolation of result on vertices

	double *c = new double[g.nv];
        double *d = new double[g.nv];
        double *v = new double[g.nv];

	

	for (int i=0;i<g.nv;i++)
	{ c[i]=0; d[i]=0;}

	

	for (int i=0;i<g.nt;i++)
		for (int j=0;j<3;j++)
		{
			c[g.t[i].v[j]] += b[i]*g.t[i].area;
			d[g.t[i].v[j]] += g.t[i].area;
		}

	

	// sorties pour le logiciel de visualisation medit


        strcpy(filename,path);
        strcat(filename,".bb");
        cout<<"fichier ecrit : " << filename<<endl;

	ofstream sol(filename);

/*	sol<<"3 1 "<<g.nt<<" 1"<<endl;

	for (int i=0;i<g.nt;i++) sol<<b[i]<<endl;// piecewise constant

*/	sol<<"3 1 "<<g.nv<<" 2"<<endl;

	for (int i=0;i<g.nv;i++)
        { v[i]=c[i]/d[i];
          sol<<v[i]<<endl;// piecewise linear
        }

        sol.close();

	// sorties pour le logiciel de visualisation gnuplot			

        strcpy(filename,path);
        strcat(filename,"-gnuplot-mesh");

	gnuplot_mesh3d(filename,g);
        cout<<"fichier ecrit : " << filename<<endl;

        strcpy(filename,path);
        strcat(filename,"-gnuplot-isos");

	gnuplot_isos3d(filename,g,v,20);
        cout<<"fichier ecrit : " << filename<<endl;

	return 0;

}



