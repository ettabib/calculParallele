### Requierement
source("h.R")

##' build a complete biaised brownian motion
##' path starting at x and ending at y
##' 
##' @title 
##' @param X 
##' @param Y 
##' @param t0 X corresponding time 
##' @param t1 Y corresponding time
##' @param pas the feet of discretization
##' @return path as vector
##' @author Ettabib Mohammad
sim.path.brow.t0.t1 <- function(X,Y,t0,t1,pas){
    if (t1-t0>=pas){
        sigma = sqrt(t1 - t0) / 2
        Z <- rnorm(n = 1, mean = (X + Y) / 2, sd = sigma)
        pathl <- sim.path.brow.t0.t1(X = X,Y = Z,t0 = t0,t1 = (t0 + t1) / 2,pas = pas)
        pathr <- sim.path.brow.t0.t1(X = Z,Y = Y,t0=(t0 + t1) / 2,t1 = t1,pas = pas)
        return (c(pathl,Z,pathr))
    }else{
        cat("T0=",t0)
        cat("T1=",t1)
        return (c())
    }
}

sim.path.brow.t0.t1.bis <- function(arg){
  return ( sim.path.brow.t0.t1(X=arg[1],Y=arg[2],t0=arg[3],t1=arg[4],pas=arg[5]))
}

clusterExport(cl=cl,list="sim.path.brow.t0.t1")

CP.sim.path.brow.t0.t1 <- function(X,Y,t0,t1,pas,cl){
  if (t1-t0>=pas){
    #browser()
    sigma = sqrt(t1 - t0) / 2
    Z <- rnorm(n = 1, mean = (X + Y) / 2, sd = sigma);
    ARG <- list(c(X,Z,t0,(t0+t1)/2,pas),c(Z,Y,(t0+t1)/2,t1,pas));
    path <- clusterMap(cl=cl,fun=sim.path.brow.t0.t1,X=c(X,Z),Y=c(Z,Y),t0=c(t0,(t0 + t1) / 2),
                       t1=c((t0 + t1) / 2,t1),MoreArgs=list(pas=pas));
    return (c(path[[1]],Z,path[[2]]))
  }else{
    return (c())
  }
}

##' @title simulate a biaised brownian path at the time t which start at
##' x_t0 and end at y_t1
##' @param t 
##' @param x start point
##' @param y end point
##' @param t0 starting time
##' @param t1 ending time
##' @return vector lenght = 1
##' @author Mohammad

sim.brow.x.y <- function(t,x,y,t0,t1){
    m <- x * (t1-t) / (t1-t0) + y * (t-t0) / (t1-t0)
    v <- (t-t0) * (t1-t) / (t1-t0)
    return (rnorm(n=1,mean=m,sd=sqrt(v)))
}


##' compute a complete biased brownian path starting at x and ending at y 
##' at te time instances Time
##' @title 
##' @param x 
##' @param y 
##' @param Time : must be c(0,t1,..,tn,T) -> only t1,..,tn we
##'                generated
##' @return sampled brownian motion on Time instances
##' @author Mohammad
sim.brow.C <- function(x,y,Time){
    Z <- c()
    time <- sort(unique(Time))
    Z[length(time)] = y; Z[1] = x;
    for(i in 2:(length(time)-1)){
        Z[i] <- sim.brow.x.y(t=time[i],x=Z[i-1],y=y,t0=time[i-1],t1=time[length(time)])
    }
    return (Z)
}

##'
##' 
##' @title 
##' @param B need to be conformed with Time
##' @param Time need to be sorted 
##' @param pas 
##' @return 
##' @author Mohammad
sim.brow.Fil <- function(B,Time,pas){
    path <- c();res <- c()
    monpas=pas
    for(i in 2:length(Time)){
        path <- sim.path.brow.t0.t1(X=B[i-1],Y=B[i],t0=Time[i-1],t1=Time[i],pas=monpas)
        res <- c(res,path)
    }
    return (res)
}
## Simulation of a brownian path knowing that x and y
##plot(sim.path.brow.t0.t1(x=0,y=1,t0=0,t1=1,pas=2^-10),type="l")

library(statmod)
dens.inv.invgauss <- function(mu,lambda,u){
    return (sqrt(lambda/(2*pi*u))*exp(lambda/(2*mu))*exp(-lambda/(2*mu^2)*(1/u+u*mu^2)))
}

g <- function(u,lambda) {
    return (-lambda/2*exp(-lambda/2*u))
}

rinv.invgauss <- function(n=1,mu,lambda){
                                        # on simule l'inverse de invgauss en prenant g ~ exp(-lambda/2)
    c <- dens.inv.invgauss(mu=mu,lambda=lambda,u=lambda/(mu^2))/g(u=lambda/(mu^2),lambda=lambda)
    while(TRUE){
        y <- rexp(n=1,rate=lambda/2)
        u <- runif(n=1,min=0,max=1)
        if(-c*u*g(y,lambda)<dens.inv.invgauss(mu=mu,lambda=lambda,u=y)){
            return(y)
        }
    }
}

Algorithme.min.2 <- function(a,T,X0=X0){
   #browser()  
    a <- a - X0
    U <- runif(n=1,min=0,max=1)
    Z1 <- (a-sqrt(2*T*rexp(n=1,rate=1)+a^2))/2
    b <- Z1
    c1=(a-b)^2/T
    c2=b^2/(2*T)
    I1 <- rinvgauss(n=1,mu=sqrt(c1/c2),lambda=2*c1)
    I2 <- rinv.invgauss(n=1,mu=sqrt(c2/c1),lambda=2*c2)
    if(U < (1+sqrt(c1/c2))^-1){
        V <- I1
    }else{
        V <- I2
    }
    Z2=T/(1+V)
    res <- c()
    res$min <- Z1 + X0
    res$t.min <- Z2
    return (res)
}

R <- function(t,delta){
    W.br.1 <- rnorm(n=1,mean=0,sd=sqrt(t))-t*rnorm(n=1,mean=0,sd=1)
    W.br.2 <- rnorm(n=1,mean=0,sd=sqrt(t))-t*rnorm(n=1,mean=0,sd=1)
    W.br.3 <- rnorm(n=1,mean=0,sd=sqrt(t))-t*rnorm(n=1,mean=0,sd=1)
    return (sqrt((delta*t*W.br.1)^2+(W.br.2)^2+(W.br.3)^2))
}
##' .. content for \description{} (no empty lines) ..
##'
##' .. content for \details{} ..
##' @title 
##' @param mT the minimum
##' @param theta the time at minimum
##' @param WT the final point of the brownian motion
##' @param s the times of discretization
##' @return 
##' @author Mohammad
decompose <- function(mT,theta,WT,s,W0){
    S <- sort(s)
    Z <- c()
    for(i in 1:length(S)){
        delta.1 <- -mT/sqrt(theta)
        delta.2 <- (WT-mT)/sqrt(T-theta)
        if(S[i]<= theta){
            Z[i] <- sqrt(theta)*R(delta=delta.1,t=(theta-S[i])/theta)+mT+W0
        }else{
            Z[i] <- sqrt(T-theta)*R(delta=delta.2,t=(S[i]-theta)/(T-theta))+mT+W0
        }
    }
    return (Z)
}

Algorithme.2 <-function(T0=0,T=T,PAS=2^-4){
    while(TRUE){
        Z <- c()
        # draw the ending point ZT of the process Z with respect to the density h
        Z.T <- sim.h()
        # simulate th minimum m of the process Z giving Z.T
        m <- Algorithme.min.2(a=Z.T,T=T,X0=X0)
        #Fix an upper bound M(m)
        M.m <- Max.phi(m$min)-min.phi
        #Draw N with poisson distribution T*M
        N <- rpois(n=1,lambda=T*M.m)
        U <- runif(n=N,min=0,max=T)
        V <- runif(n=N,min=0,max=M.m)
        # Fill in the path of Z at the remaining times U
        i=floor((U-T0)/PAS)+1
        Z <- decompose(mT=m$min,theta=m$t.min,WT=Z.T,s=U,W0=X0)
        # evaluate the number of points under M.m
        if(prod(phi(Z)-min.phi<V)==1){
            return (Z)
        }
    }
}



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

Algorithme.2.bis <- function(N=100,NT=100,T=1,x0=X0){
    Traj <- c()
    res <- c()
    Max=0
    Z.T <- sim.h()
    for(i in 1:NT){
        Traj <- sim.path.brow.t0.t1(X=x0,Y=Z.T,t0=0,t1=T,pas=T/N)
        Max <- Max + max(Traj)
    }
    M <- Max-min.phi
    while(TRUE){
        N <- rpois(n=1,lambda=T*M)
        U <- runif(n=N,min=0,max=T)
        V <- runif(n=N,min=0,max=M)
        Z.T <- sim.h()
        Z <- sim.brow.C(x=X0,y=Z.T,Time=U)
        if(prod(phi(Z)-min.phi<V)==1){
            res$Z <- Z
            res$U <- U
            res$V <- V
            res$Max <- M
            return (res)
        }
    }
}



