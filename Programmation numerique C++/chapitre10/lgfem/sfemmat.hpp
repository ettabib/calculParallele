class MatLap: public VirtualMatrice<R> { public:
    //  une class pour modeliser la matrice  suivante:
    //  $ \int_\Omega \alpha w^i w^j + \int_\Omega \beta(K) \nabla w^i \nabla w^j +  \int_{\Gamma_r} \alpha_r w^i w^j $
    //  $ \forall i  \not\in  \Gamma_d \}$
    // ------------------------------------------
    const  Mesh & Th;
    const KN<bool> & Gamma_d;    //   Gamma\_d[i] = $ i \in \Gamma_d$
    R (*beta)(const Triangle &); // fonction coef du $\int  \beta_K \nabla w_i \nabla w_j $

    const R alpha;       // coef de la masse de masse $\int \alpha w_i*w_j$
    const int Gamma_r;   // numero de $\Gamma_{r}$
    const R  alpha_r;    // coef $\int_{\Gamma_{r}}\alpha_r w_i*w_j$

    typedef  VirtualMatrice<R>::plusAx plusAx;

    MatLap(const  Mesh & T,const KN<bool> & g_d,R a,R (*b)(const Triangle &)=0 ,R a_r=0,int g_r=0)
        : Th(T),Gamma_d(g_d),alpha(a),beta(b),alpha_r(a_r), Gamma_r(g_r) {};

    void addMatMul(const  KN_<R>  & x, KN_<R> & Ax) const;
    plusAx operator*(const KN<R> &  x) const {return plusAx(this,x);}
    
};
