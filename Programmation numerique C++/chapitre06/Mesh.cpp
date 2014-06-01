#include "Mesh.hpp"

using namespace std; 

namespace bamg {
  int verbosity=10;  //  niveau d'impression
  

///////////////////////////////////////////////////////////////////////////  

  void  swap(Triangle *t1,short a1,
             Triangle *t2,short a2,
             Vertex *s1,Vertex *s2,Icoor2 det1,Icoor2 det2)
  { 
    // swap  \hfilll
    // --------------------------------------------------------------\hfilll
    //\verb; les 2 numero de l arête dans les 2 triangles           ;\hfilll
    //\verb;                                                        ;\hfilll
    //\verb;               sb                     sb                ;\hfilll
    //\verb;             / | \                   /   \              ;\hfilll
    //\verb;         as1/  |  \                 /a2   \             ;\hfilll
    //\verb;           /   |   \               /    t2 \            ;\hfilll
    //\verb;       s1 /t1  | t2 \s2  -->   s1 /---as2---\s2         ;\hfilll
    //\verb;          \  a1|a2  /             \   as1   /           ;\hfilll
    //\verb;           \   |   /               \ t1    /            ;\hfilll
    //\verb;            \  |  / as2             \   a1/             ;\hfilll
    //\verb;             \ | /                   \   /              ;\hfilll
    //\verb;              sa                       sa               ;\hfilll
    //  -------------------------------------------------------------\hfilll
    int as1 = NextEdge[a1];
    int as2 = NextEdge[a2];
    //    int ap1 = PreviousEdge[a1];
    //int ap2 = PreviousEdge[a2];
    (*t1)(VerticesOfTriangularEdge[a1][1]) = s2 ; // avant sb
    (*t2)(VerticesOfTriangularEdge[a2][1]) = s1  ; // avant sa
    // mise a jour des 2 adjacences externes 
    TriangleAdjacent taas1 = t1->Adj(as1),    taas2 = t2->Adj(as2),
      tas1(t1,as1), tas2(t2,as2),   ta1(t1,a1),  ta2(t2,a2);
    // externe haut gauche
    taas1.SetAdj2(ta2,taas1.GetAllFlag_UnSwap());
    // externe bas droite
    taas2.SetAdj2(ta1, taas2.GetAllFlag_UnSwap());
    // interne 
    tas1.SetAdj2(tas2);
    
    t1->det = det1;
    t2->det = det2;
    
    t1->SetTriangleContainingTheVertex();
    t2->SetTriangleContainingTheVertex();
    
  } // end swap 
  
///////////////////////////////////////////////////////////////////////////  

  int SwapForForcingEdge(Vertex   *  & pva ,Vertex  * &   pvb ,
                         TriangleAdjacent & tt1,Icoor2 & dets1,
                         Icoor2 & detsa,Icoor2 & detsb, int & NbSwap)
  { // l'arête ta coupe l'arête pva pvb de cas apres le swap sa coupe toujours
    // on cherche l'arête suivante  on suppose que detsa >0 et detsb <0
    // attention la routine echange pva et pvb \hfilll
    
    if(tt1.Locked()) return 0; // frontiere croise 
    
    TriangleAdjacent tt2 = Adj(tt1);
    Triangle *t1=tt1,*t2=tt2;// les 2 triangles adjacent
    short a1=tt1,a2=tt2;// les 2 numero de l arête dans les 2 triangles
    assert ( a1 >= 0 && a1 < 3 );
    
    Vertex & sa= (* t1)[VerticesOfTriangularEdge[a1][0]];
    //    Vertex & sb= (*t1)[VerticesOfTriangularEdge[a1][1]];
    Vertex & s1= (*t1)[OppositeVertex[a1]];
    Vertex & s2= (*t2)[OppositeVertex[a2]];
    
    
    Icoor2 dets2 = det(*pva,*pvb,s2);
    Icoor2 det1=t1->det , det2=t2->det ;
    Icoor2 detT = det1+det2;
    assert((det1>0 ) && (det2 > 0));
    assert ( (detsa < 0) && (detsb >0) ); // [a,b] cut infinite line va,bb
    Icoor2 ndet1 = bamg::det(s1,sa,s2);
    Icoor2 ndet2 = detT - ndet1;
    
    int ToSwap =0; //pas de swap
    if ((ndet1 >0) && (ndet2 >0)) 
      { // on peut swaper  
        if ((dets1 <=0 && dets2 <=0) || (dets2 >=0 && detsb >=0))
          ToSwap =1; 
        else // swap alleatoire 
          if (BinaryRand()) 
            ToSwap =2; 
      }
    
    if (ToSwap) NbSwap++,
                  bamg::swap(t1,a1,t2,a2,&s1,&s2,ndet1,ndet2);
    
    int ret=1;
    
    if (dets2 < 0) {// haut
      dets1 = ToSwap ? dets1 : detsa ;
      detsa = dets2; 
      tt1 =  Previous(tt2) ;}
    else if (dets2 > 0){// bas 
      dets1 = ToSwap ? dets1 : detsb ;
      detsb = dets2;
      if(!ToSwap) tt1 =  Next(tt2);
    }
    else { // changement de sens 
      ret = -1;
      Exchange(pva,pvb);
      Exchange(detsa,detsb);
      Exchange(dets1,dets2);
      Exchange(tt1,tt2);
      dets1=-dets1;
      dets2=-dets2;
      detsa=-detsa;
      detsb=-detsb;
      
      if (ToSwap) 
        if (dets2 < 0) {// haut
          dets1 = (ToSwap ? dets1 : detsa) ;
          detsa = dets2; 
          tt1 =  Previous(tt2) ;}
        else if (dets2 > 0){// bas 
          dets1 = (ToSwap ? dets1 : detsb) ;
          detsb =  dets2;
          if(!ToSwap) tt1 =  Next(tt2);
        }
        else {// on a fin ???
          tt1 = Next(tt2);
          ret =0;}
    }
    return ret;
  }
  
///////////////////////////////////////////////////////////////////////////

  int ForceEdge(Vertex &a, Vertex & b,TriangleAdjacent & taret)  
  { 
    
    int NbSwap =0;
    assert(a.t && b.t); // the 2 vertex is in a mesh 
    int k=0;
    taret=TriangleAdjacent(0,0); // erreur 
    
    TriangleAdjacent tta(a.t,EdgesVertexTriangle[a.vint][0]);
    Vertex   *v1, *v2 = tta.EdgeVertex(0),*vbegin =v2;
    // we turn around a in the  direct sens  
    
    Icoor2 det2 = v2 ? det(*v2,a,b): -1 , det1;
    if(v2) // normal case 
      det2 = det(*v2,a,b);
    else { // no chance infini vertex try the next
      tta= Previous(Adj(tta));
      v2 = tta.EdgeVertex(0);
      vbegin =v2;
      assert(v2);
      det2 = det(*v2,a,b);
    }
    
    
    while (v2 != &b) {
      TriangleAdjacent tc = Previous(Adj(tta));    
      v1 = v2; 
      v2 = tc.EdgeVertex(0);
      det1 = det2;
      det2 =  v2 ? det(*v2,a,b): det2; 
      
      if((det1 < 0) && (det2 >0)) { 
        // try to force the edge 
        Vertex * va = &a, *vb = &b;
        tc = Previous(tc);
        assert ( v1 && v2);
        Icoor2 detss = 0,l=0,ks;
        while ((ks=SwapForForcingEdge(  va,  vb, tc, detss, det1,det2,NbSwap)))
          if(l++ > 1000000) {
            cerr << " Loop in forcing Egde AB" << endl;
            MeshError(990);
          }
        Vertex *aa = tc.EdgeVertex(0), *bb = tc.EdgeVertex(1);
        if (( aa == &a ) && (bb == &b) ||  (bb ==  &a ) && (aa == &b)) {
          tc.SetLock();
          a.DelaunayOptim(1);
          b.DelaunayOptim(1);
          taret = tc;
          return NbSwap;
        } 
      }
      tta = tc;
      assert(k++<2000);
      if ( vbegin == v2 ) return -1;// error 
    }
    
    tta.SetLock();
    taret=tta;
    a.DelaunayOptim(1);
    b.DelaunayOptim(1);
    return NbSwap; 
  }
  
///////////////////////////////////////////////////////////////////////////
  
  int Triangle::DelaunaySwap(short a){
    if(a/4 !=0) return 0;// arête lock or MarkUnSwap
    
    Triangle *t1=this,*t2=at[a];// les 2 triangles adjacent
    short a1=a,a2=aa[a];// les 2 numero de l arête dans les 2 triangles
    if(a2/4 !=0) return 0; // arête lock or MarkUnSwap
    
    Vertex  *sa=t1->ns[VerticesOfTriangularEdge[a1][0]];
    Vertex  *sb=t1->ns[VerticesOfTriangularEdge[a1][1]];
    Vertex  *s1=t1->ns[OppositeVertex[a1]];
    Vertex  *s2=t2->ns[OppositeVertex[a2]];
    
    Icoor2 det1=t1->det , det2=t2->det ;
    Icoor2 detT = det1+det2;
    Icoor2 detA = Abs(det1) + Abs(det2);
    Icoor2 detMin = Min(det1,det2);
    
    int OnSwap = 0;       
    // si 2 triangle infini (bord) => detT = -2;
    if (sa == 0) {// les deux triangles sont frontieres
      det2=bamg::det(s2->i,sb->i,s1->i);
      OnSwap = det2 >0;}
    else if (sb == 0) { // les deux triangles sont frontieres
      det1=bamg::det(s1->i,sa->i,s2->i);
      OnSwap = det1 >0;}
    else if(( s1 != 0) && (s2 != 0) ) {
      det1 = bamg::det(s1->i,sa->i,s2->i);
      det2 = detT - det1;
      OnSwap = (Abs(det1) + Abs(det2)) < detA;
      
      Icoor2 detMinNew=Min(det1,det2);
      if (! OnSwap &&(detMinNew>0)) {
        OnSwap = detMin ==0;
        if (! OnSwap) {
          
          while (1)
            {
              // critere de Delaunay pure isotrope
              Icoor2 xb1 = sb->i.x - s1->i.x,
                x21 = s2->i.x - s1->i.x,
                yb1 = sb->i.y - s1->i.y,
                y21 = s2->i.y - s1->i.y,
                xba = sb->i.x - sa->i.x, 
                x2a = s2->i.x - sa->i.x,
                yba = sb->i.y - sa->i.y,
                y2a = s2->i.y - sa->i.y;
              double
                cosb12 =  double(xb1*x21 + yb1*y21),
                cosba2 =  double(xba*x2a + yba*y2a) ,
                sinb12 = double(det2),
                sinba2 = double(t2->det);
              
              OnSwap =  ((double) cosb12 * (double)  sinba2) 
                <  ((double) cosba2 * (double) sinb12);
              
              break;
            }
          
        } // OnSwap 
      } // (! OnSwap &&(det1 > 0) && (det2 > 0) )
    }
    
    if( OnSwap ) 
      bamg::swap(t1,a1,t2,a2,s1,s2,det1,det2);
    
    return OnSwap;
  }
  
///////////////////////////////////////////////////////////////////////////
  
  void Triangles::Add( Vertex & s,Triangle * t, Icoor2 * det3) 
  {
    // -------------------------------------------\hfilll
    //\verb;             s2                       ;\hfilll
    //\verb;                                      ;\hfilll
    //\verb;             /|\                      ;\hfilll
    //\verb;            / | \                     ;\hfilll
    //\verb;           /  |  \                    ;\hfilll
    //\verb;    tt1   /   |   \ tt0               ;\hfilll
    //\verb;         /    |s   \                  ;\hfilll
    //\verb;        /     .     \                 ;\hfilll
    //\verb;       /  .      `   \                ;\hfilll
    //\verb;      / .           ` \               ;\hfilll
    //\verb;      ----------------                ;\hfilll
    //\verb;    s0       tt2       s1             ;\hfilll
    //--------------------------------------------\hfilll
      
    Triangle * tt[3]; // the 3 new Triangles
    Vertex &s0 = (* t)[0], &s1=(* t)[1], &s2=(* t)[2];
    Icoor2  det3local[3];
    
    int infv = &s0 ?  ((  &s1 ? ( &s2  ? -1 : 2) : 1  )) : 0;
    
    int nbd0 =0; // number of zero det3
    int izerodet=-1,iedge; // izerodet = egde contening the vertex s
    Icoor2 detOld = t->det;
    
    if ( ( infv <0 ) && (detOld <0) ||  ( infv >=0  ) && (detOld >0) ) 
      {
        cerr << "  infv " << infv << " det = " << detOld << endl;
        MeshError(3);
      }
    
    if (!det3) { 
      det3 = det3local; // alloc 
      if ( infv<0 ) {
        det3[0]=bamg::det(s ,s1,s2);
        det3[1]=bamg::det(s0,s ,s2);
        det3[2]=bamg::det(s0,s1,s );}
      else { 
        // one of &s1  &s2  &s0 is NULL so (&si || &sj) <=> !&sk
        det3[0]=  &s0 ? -1  : bamg::det(s ,s1,s2) ;
        det3[1]=  &s1 ? -1 : bamg::det(s0,s ,s2) ;
        det3[2]=  &s2 ? -1 : bamg::det(s0,s1,s ) ;}}
    
    
    if (!det3[0]) izerodet=0,nbd0++;
    if (!det3[1]) izerodet=1,nbd0++;
    if (!det3[2]) izerodet=2,nbd0++;
    
    if  (nbd0 >0 ) // point s on a egde or on a vertex 
      if (nbd0 == 1) {
        iedge = OppositeEdge[izerodet];
        TriangleAdjacent ta = t->Adj(iedge);
        
        // the point is on the edge 
        // if the point is one the boundary 
        // add the point in outside part 
        if ( t->det >=0) { // inside triangle
          if ((( Triangle *) ta)->det < 0 ) {
            // add in outside triangle 
            Add(s,( Triangle *) ta);
            return;}
        }}
      else {
        cerr << " bug  " << nbd0 <<endl;
        cerr << " Bug double points in " << endl ;
        MeshError(5);}
    
    tt[0]= t;
    tt[1]= &triangles[nbt++];
    tt[2]= &triangles[nbt++];
    
    if (nbt>nbtx) {
      cerr << " No enougth triangles " << endl;
      MeshError(999);
    }
    
    *tt[1]=   *tt[2]= *t;
    
    
    (* tt[0])(OppositeVertex[0])=&s;
    (* tt[1])(OppositeVertex[1])=&s;
    (* tt[2])(OppositeVertex[2])=&s;
    
    tt[0]->det=det3[0];
    tt[1]->det=det3[1];
    tt[2]->det=det3[2];         
    
    //  update adj des triangles externe 
    tt[0]->SetAdjAdj(0);
    tt[1]->SetAdjAdj(1);
    tt[2]->SetAdjAdj(2);
    //  update des adj des 3 triangle interne
    const int i0 = 0;
    const int i1= NextEdge[i0];
    const int i2 = PreviousEdge[i0];
    
    tt[i0]->SetAdj2(i2,tt[i2],i0);
    tt[i1]->SetAdj2(i0,tt[i0],i1);
    tt[i2]->SetAdj2(i1,tt[i1],i2);
    
    tt[0]->SetTriangleContainingTheVertex();
    tt[1]->SetTriangleContainingTheVertex();
    tt[2]->SetTriangleContainingTheVertex();
    
    
    // swap if the point s is on a edge
    if(izerodet>=0) {
      int rswap =tt[izerodet]->DelaunaySwap(iedge);
      
      if (!rswap) 
        {
          cout << " Pb swap the point s is on a edge =>swap " << iedge << endl;
          MeshError(98);
        }
    }
  }  
  
///////////////////////////////////////////////////////////////////////////  

  void Triangles::Insert() 
  {
    NbUnSwap=0;
    if (verbosity>2) 
      cout << " -- Insert initial " << nbv << " vertices " << endl ;

    SetIntCoor();
    int i;
    Vertex**  ordre=new (Vertex* [nbvx]);

    for (i=0;i<nbv;i++) 
      ordre[i]= &vertices[i] ;
    
    // construction d'un ordre aleatoire
    const int PrimeNumber=  (nbv % 567890621L)  ? 567890629L : 567890621L;
    int k3 = rand()%nbv ; 
    for (int is3=0; is3<nbv; is3++) 
      ordre[is3]= &vertices[k3 = (k3 + PrimeNumber)% nbv];
    
    for (i=2 ; det( ordre[0]->i, ordre[1]->i, ordre[i]->i ) == 0;) 
      if  ( ++i >= nbv) {
        cerr << " All the vertices are aline " << endl;
        MeshError(998); }
    
    // echange i et 2 dans ordre afin 
    // que les 3 premiers ne soit pas aligne
    Exchange( ordre[2], ordre[i]);
    
    // on ajoute un point à l'infini pour construire le maillage
    // afin d'avoir une définition simple des arêtes frontieres
    nbt = 2;
    // on construit un maillage trival forme
    // d'une arête et de 2 triangles
    // construit avec le 2 arêtes orientes et 
    Vertex *  v0=ordre[0], *v1=ordre[1];
    
    triangles[0](0) = 0; // sommet pour infini (la frontière)
    triangles[0](1) = v0;
    triangles[0](2) = v1;
    
    triangles[1](0) = 0;// sommet pour infini  (la frontière)
    triangles[1](2) = v0;
    triangles[1](1) = v1;

    const int e0 = OppositeEdge[0];
    const int e1 = NextEdge[e0];
    const int e2 = PreviousEdge[e0];
    triangles[0].SetAdj2(e0, &triangles[1] ,e0);
    triangles[0].SetAdj2(e1, &triangles[1] ,e2);
    triangles[0].SetAdj2(e2, &triangles[1] ,e1);
    
    triangles[0].det = -1;  // faux triangles
    triangles[1].det = -1;  // faux triangles
    
    triangles[0].SetTriangleContainingTheVertex();
    triangles[1].SetTriangleContainingTheVertex();
    nbtf = 2;  // Il y a deux triangle frontiere invalide
    
    // -------- on ajoute les sommets un a un  -----------
    
    int NbSwap=0;

    if (verbosity>3) cout << " -- Begin of insertion process " << endl;
    
    Triangle * tcvi=triangles;
    
    for (int icount=2; icount<nbv; icount++) 
      {
        Vertex *vi  = ordre[icount];
        Icoor2 dete[3];
        tcvi = FindTriangleContening(vi->i,dete,tcvi);
        Add(*vi,tcvi,dete);
        NbSwap += vi->DelaunayOptim(1);
      }  // fin de boucle en icount

    if (verbosity>3) 
      cout << "    NbSwap of insertion " <<    NbSwap 
           << " NbSwap/Nbv " <<  (float) NbSwap / (float) nbv 
           << " NbUnSwap " << NbUnSwap << " Nb UnSwap/Nbv " 
           << (float)NbUnSwap /(float) nbv 
           <<endl;
    NbUnSwap = 0;
    delete [] ordre;
  }
  
///////////////////////////////////////////////////////////////////////////  
  
  void Triangles::RandomInit() 
  { 
    nbv = nbvx;
    for (int i = 0; i < nbv; i++)
      {
        vertices[i].r.x= rand();
        vertices[i].r.y= rand();
        vertices[i].Label = 0;
      }
  }

///////////////////////////////////////////////////////////////////////////  
  
  Triangles::~Triangles() 
  {
    if(vertices)  delete [] vertices;
    if(triangles) delete [] triangles;
     PreInit(0); // set all to zero 
    
  }
  
///////////////////////////////////////////////////////////////////////////  
  
  void Triangles::SetIntCoor()
  {
    pmin =  vertices[0].r;
    pmax =  vertices[0].r;
    
    // recherche des extrema des vertices pmin,pmax
    int i;
    for (i=0;i<nbv;i++) {
      pmin.x = Min(pmin.x,vertices[i].r.x);
      pmin.y = Min(pmin.y,vertices[i].r.y);
      pmax.x = Max(pmax.x,vertices[i].r.x);
      pmax.y = Max(pmax.y,vertices[i].r.y);
    }
    R2 DD = (pmax-pmin)*0.05;
    pmin = pmin-DD;
    pmax = pmax+DD; 
    coefIcoor= (MaxICoor)/(Max(pmax.x-pmin.x,pmax.y-pmin.y));
    assert(coefIcoor >0);
    
    // generation des coordonnees entieres
    for (i=0;i<nbv;i++) {
      vertices[i].i = toI2(vertices[i].r);    
    }
    
    // calcule  des determinants si neccessaire \hfilll
    int Nberr=0;
    for (i=0;i<nbt;i++)
      {
        Vertex & v0 = triangles[i][0];
        Vertex & v1 = triangles[i][1];
        Vertex & v2 = triangles[i][2];
        if ( &v0 && &v1 &&  &v2 ) // a good triangles;
          {
            triangles[i].det= det(v0,v1,v2);
            if (triangles[i].det <=0 && Nberr++ <10)
              {
                if(Nberr==1)
                    cerr << "+++ Fatal Error "
                         << "(SetInCoor)  Error :  area of Triangle < 0\n";
              }
          }
        else
          triangles[i].det= -1; // le  triangle est dégénéré; 
      }
    if (Nberr) MeshError(899);
  }
  
///////////////////////////////////////////////////////////////////////////  
  
  int  Triangle::DelaunayOptim(short i)
  {
    // nous tournons dans le sens trigonométrique
    int NbSwap =0;
    //    Vertex * s  = ns[i];
    Triangle  *t = this;
    int k=0,j =OppositeEdge[i];
    int jp = PreviousEdge[j];
    // initialise   tp, jp avec l'arête precedente de $j$
    Triangle *tp= at[jp];
    jp = aa[jp]&3;
    do {
      while (t->DelaunaySwap(j))
        {
          NbSwap++;
          assert(k++<20000);
          t=  tp->at[jp];      // set unchange t qnd j for previous triangles
          j=  NextEdge[tp->aa[jp]&3];
        }
      // end on this  Triangle 
      tp = t;
      jp = NextEdge[j];
      
      t=  tp->at[jp];      // set unchange t qnd j for previous triangles
      j=  NextEdge[tp->aa[jp]&3];
      
    } while( t != this);
    return NbSwap;
  }
  
///////////////////////////////////////////////////////////////////////////  

  void Triangles::PreInit(int inbvx)
  {
    // fonction  d'initialisation -------------------------------------
    // ----------------------------------------------------------------
    srand(19999999); 
    NbOfSwapTriangle =0;
    nbiv=0;
    nbv=0;
    nbvx=inbvx;
    nbt=0;
    nbtx=2*inbvx-2;
    
    if (inbvx) { 
      vertices=new Vertex[nbvx];
      assert(vertices);
      triangles=new Triangle[nbtx];
      assert(triangles);}
    else {
      vertices=0;
      triangles=0;
      nbtx=0;
    }  
  }
  
///////////////////////////////////////////////////////////////////////////
  
  Triangle * Triangles::FindTriangleContening(const I2 & B,
                                              Icoor2 dete[3], 
                                              Triangle *tstart) const
  { // in: B 
    // out: t
    // out : dete[3]
    // t the triangle and s0,s1,s2 the 3 vertices of t
    // in dete[3] = { det(B,s1,s2) , det(s0,B,s2), det(s0,s1,B)}
    // with det(a,b,c ) = -1 if one of 3 vertices a,b,c is NULL 
    Triangle * t=0;     
    int j,jp,jn,jj;
    t=tstart;
    assert(t>= triangles && t < triangles+nbt);
    Icoor2  detop ;
    int kkkk =0; // number of test triangle 
    
    while ( t->det < 0) 
      { // the initial triangles is outside  
        int k0=(*t)(0) ?  ((  (*t)(1) ? ( (*t)(2) ? -1 : 2) : 1  )) : 0;
        assert(k0>=0); // k0 the NULL  vertex 
        int k1=NextVertex[k0],k2=PreviousVertex[k0];
        dete[k0]=det(B,(*t)[k1],(*t)[k2]);
        dete[k1]=dete[k2]=-1;     
        if (dete[k0] > 0) // outside B 
          return t; 
        t = t->TriangleAdj(OppositeEdge[k0]);
        assert(kkkk++ < 2);
      }
    
    jj=0;
    detop = det(*(*t)(VerticesOfTriangularEdge[jj][0]),
                *(*t)(VerticesOfTriangularEdge[jj][1]),B);
    
    while(t->det  > 0 ) 
      { 
        assert( kkkk++ < 2000 ); 
        j= OppositeVertex[jj];
        
        dete[j] = detop;  //det(*b,*s1,*s2);
        jn = NextVertex[j];
        jp = PreviousVertex[j];
        dete[jp]= det(*(*t)(j),*(*t)(jn),B);
        dete[jn] = t->det-dete[j] -dete[jp];
        
        // count the number k of  dete <0
        int k=0,ii[3];
        if (dete[0] < 0 ) ii[k++]=0; 
        if (dete[1] < 0 ) ii[k++]=1;
        if (dete[2] < 0 ) ii[k++]=2;
        // 0 => ok
        // 1 => go in way 1
        // 2 => two way go in way 1 or 2 randomly
        
        if (k==0) 
          break;
        if (k == 2 && BinaryRand())
          Exchange(ii[0],ii[1]);
        assert ( k  < 3);
        TriangleAdjacent t1 = t->Adj(jj=ii[0]);
        if ((t1.det() < 0 ) && (k == 2))
          t1 = t->Adj(jj=ii[1]);
        t=t1;
        j=t1;// for optimisation we now the -det[OppositeVertex[j]];
        detop = -dete[OppositeVertex[jj]];
        jj = j;
      }
    
    if (t->det<0) // outside triangle 
      dete[0]=dete[1]=dete[2]=-1,dete[OppositeVertex[jj]]=detop;
    
    return t;
  }
} // \hfill --------  end namespace ----------

