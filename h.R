source("Def.R")

## Classe simulant h
### Calcul de h

Rlambert <- function(x,N){
    w0=1
    N=0
    for(i in 1:N){
        w0=w0-(w0*exp(w0)-x)/((1+w0)*exp(w0))
    } 
    return (w0)
}

## U* 
u.etoile=(gamma * T + Rlambert( T * beta * S0 * exp(- sigma * X0  - gamma * T ))) / sigma + X0


##' @title 
##' @param u 
##' @param t 
##' @param x0 
##' @return 
##' @author Mohammad
h <- function(u,t=T,x0=X0){
    P0*exp(A(u)-(u-x0)^2/(2*t))
}

f.A <- function(z,u=u.etoile,T){
    return (1/sqrt(2*pi*T)*exp(-(z-u)^2/T))
}

g.A <- function(z){
    return ((beta*S0)/sigma*(1-exp(sigma*z)-sigma*z*exp(-sigma*u.etoile)))
}

C=f.A(0,u.etoile,T) * exp(g.A(0) - g.A(u.etoile)) / h(0)

                                        
##' simulate h with rejection sampling
##' @title 
##' @param t 
##' @param x0 
##' @return 
##' @author Mohammad
sim.h <- function(t=T,x0=X0){
    X=0
    repeat{
        U <- runif(1)
        Y <- rnorm(1,mean=u.etoile,sd=t)
        if (C*h(Y)/f.A(Y,u.etoile,t)>U){
            X=Y;
            break()
        }
    } 
    return (X)
}




