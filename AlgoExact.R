source("Myfunction.R")


##' the first version of the Exact Algorithm 
##' requires phi bounded
##' @title Exact Algorithm 1
##' @param T the time intervall
##' @param x0 the initial condition on X
##' @return 
##' @author Mohammad
Algorithme.1 <- function(T=1,x0=X0,pas=1){
    while(TRUE){
        N <- rpois(n=1,lambda=T*Max.phi(0))
        if(N==0){
            return (sim.path.brow.t0.t1(X=X0,Y=sim.h(),t0=0,t1=T,pas=PAS))
        }
        U <- runif(n=N,min=0,max=T)
        V <- runif(n=N,min=0,max=Max.phi(0))
        Z.T <- sim.h()
        Z <- sim.brow.C(x=x0,y=Z.T,Time=sort(c(0,U,T)))
        rejet=phi(Z[2:(length(Z)-1)])-min.phi;
        if(prod(rejet<V)==1){
           return (Z) 
          #return (sim.brow.Fil(B=Z,Time=sort(c(0,U,T)),pas=pas))
        }
    }
}

