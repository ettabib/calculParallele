### Requierement
source("Def.R")


A <- function(u){
  return (gamma / sigma * u - beta * S0 / sigma^2 * exp(-sigma * u) + cte)
}
a <- function(u){
  return (gamma / sigma + beta * S0 / sigma * exp(-sigma * u))
}
phi <- function(u){
  return ((1/2)*(0.1666666667e-1+133.3333333*exp(-.3*u))^2-20.00000000*exp(-.3*u))
}

if((2 * gamma )>= sigma ^ 2){
  min.phi= gamma ^ 2 / (2 * sigma ^ 2)
}else{
  min.phi=phi((1 / sigma ) * log((2 * beta * S0) / ( (sigma ^ 2) - (2 * gamma))))
}

Max.phi <- function(m){
  if(2 * gamma < sigma^2){
    m.min = (1 / sigma ) * log((2 * beta * S0) / ( (sigma ^ 2) - (2 * gamma)))
    if(m <= m.min){
      return (max(phi(m),(gamma ^ 2) / ( 2 * (sigma ^ 2))))
    }else{
      return ((gamma ^ 2) / ( 2 * (sigma ^ 2)))
    }
  }else{
    return ((gamma ^ 2) / ( 2 * (sigma ^ 2)))
  }
}


test.loi.T <- function(Nsim=100,Ndisc=100,x0=X0){
  X <- c()
  for(i in 1:Nsim){
    Z <- Algorithme.1.bis(N=Ndis,T=1,x0=x0)
    X <- c(X,Z[length(Z)])
  }   
  return (X)
}


