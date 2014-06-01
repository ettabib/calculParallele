##' version de nipals sans donnees manquantes
##' @title 
##' @param cl 
##' @param X 
##' @param r 
##' @param epsilon 
##' @param Iter 
##' @return 
##' @author Mohammad
Nipals.sdm <- function(cl,X,r,epsilon=10^-6,Iter=5,scale=TRUE,center=TRUE){
    DATA=scale(x=X,scale=scale,center=scale)
    #Etape 1
    XH.1=DATA
    XH=DATA
    n=dim(X)[1]
    p=dim(X)[2]
    T <- matrix(nrow=n,ncol=r)
    P <- matrix(nrow=p,ncol=r)
    
    for(h in 1:r){
        
        ph=rep(x=0,times=p)
        ph.1 <- ph+2*epsilon
        #Etape 2.1
        th <- XH.1[,1]
        #Etabpe2.2
        iter=0
        while(norm(ph-ph.1,type="2") > epsilon && iter < Iter){
            ph <- P.Mult.Matrix.Vect(cl=cl,M=as.matrix(t(XH.1)),V=th) / P.Mult.Vect.Vect(cl=cl,U=th,V=th)
            #Normer ph
            ph <- ph  / sqrt(P.Mult.Vect.Vect(cl=cl,U=ph,V=ph))
            #Etape 2.2.3
            th <- P.Mult.Matrix.Vect(cl=cl,M=as.matrix(XH.1),V=ph)  / P.Mult.Vect.Vect(cl=cl,U=ph,V=ph)
            iter=iter+1
        }
        #Etape 2.3
        XH <- XH.1 - P.Mult.Matrix.Matrix(cl=cl,M=as.matrix(th),N=t(as.matrix(ph)))
        XH.1 <- XH
        T[,h] <- th
        P[,h] <- ph
    }
    
    res <- list()
    res$li <- T
    res$c1 <- P
    res$eig <- apply(T,2,var)
    res$tab  <- 
    return (res)
}


##' dim(XH.1) = (n,p)
##' dim(th)   = (n,1)
##' dim(ph)   = (p,1)
##' @title 
##' @return 
##' @author Mohammad
##' @param cl the cluster (snow)
##' @param X The matrice of data (n,p)
##' @param r The rank of X
##' @param epsilon the presicion
##' @param Iter 
##' @param scater 
##' @param center 
Nipals <- function(cl,X,r,epsilon=10^-6,Iter=5,scale=TRUE,center=TRUE){
    DATA=scale(x=X,scale=scale,center=scale)
    #Etape 1
    XH.1=DATA
    XH=DATA
    n=dim(X)[1]
    p=dim(X)[2]
    T <- matrix(nrow=n,ncol=r)
    P <- matrix(nrow=p,ncol=r)
    #Etape 2
    for(h in 1:r) {
        ph=rep(x=0,times=p)
        ph1 <- ph+2*epsilon
       #Etape 2.1
       # th recoit la premiere colonne de X(h-1)
        th <- XH.1[ ,1]

        #Etape 2.2.2
       # repeter jusqu'a la convergence de ph
        iter=0
        while( norm(x=(ph-ph1),type="2")>epsilon && iter < Iter) {
            ph1 <- ph
            #Etape 2.2.1
            #traitement iteratif pour chaque colonne
            for(j in 1:p) {
                EXIST.XH <- (XH.1[,j] != Inf)
                EXIST.XH[is.na(EXIST.XH)] <- FALSE
                
            # Demasquer les NA pour th
               EXIST.th <- (th != Inf)
               EXIST.th[is.na(EXIST.th)] <- FALSE

            #Traitement commun
               EXIST <- EXIST.XH * EXIST.th == TRUE

            #Calcul de denominateur de ph
               a <- P.Mult.Vect.Vect(cl=cl,U=XH.1[,j][EXIST],V=th[EXIST])
               b <- P.Mult.Vect.Vect(cl=cl,U=th[EXIST],V=th[EXIST])
               if(b==0){
                   cat("b est null")
                   exit;
               }
               ph[j] <- a / b
           }
           
            #Etape 2.2.2
           #normer ph a 1
           ph <- ph/norm(x=ph,type="2")

            #Etape 2.2.3
           for(i in 1:n){
               EXIST.XH <- (XH.1[i,] != Inf)
               EXIST.XH[is.na(EXIST.XH)] <- FALSE

               a <- P.Mult.Vect.Vect(cl=cl,U=XH.1[i,][EXIST.XH],V=ph[EXIST.XH])
               b <- P.Mult.Vect.Vect(cl=cl,U=ph[EXIST.XH],V=ph[EXIST.XH])
               th[i] <- a / b
           }
            iter=iter+1
       }

        #Etape 2.3
        XH <- XH.1 - th %*% t(ph)
        XH.1 <- XH
        T[,h] <- th
        P[,h] <- ph
    }
    res <- list()
    res$li <- T
    res$c1 <- P
    res$tab <- DATA
    res$eig <- apply(T,2,var)
    return (res)
}

##' @requirement M and V needs to be compatible in dimensions
##' the calculations are done in parallel
##' In the case of nrow >> nclo
##' @title compute mathematical multiplication matrix-vector
##' @param cl cluster
##' @param M The matrix
##' @param V The vector
##' @return M %*% V
##' @author Mohammad
P.Mult.Matrix.Vect <- function(cl,M,V){
  #  browser()
    SPLIT <- clusterSplit(cl=cl,seq=1:dim(M)[1])
    ARG <- list()
    for(i in 1:2){
        ARG[[i]] <- M[SPLIT[[i]],]
    }
    return(unlist(clusterApply(cl=cl,x=ARG,fun="%*%",...=as.matrix(V))))
}
##' .. content for \description{} (no empty lines) ..
##'
##' .. content for \details{} ..
##' @title 
##' @param cl 
##' @param U 
##' @param V 
##' @return 
##' @author Mohammad
P.Mult.Vect.Vect <- function(cl,U,V) {
    #browser()
    SPLIT <- clusterSplit(cl=cl,seq=1:length(U))
    ARG <- list()
    for(i in 1:2){
        ARG[[i]] <- list(U[SPLIT[[i]]],V[SPLIT[[i]]])
    }
    return(sum(unlist(clusterApply(cl=cl,x=ARG,fun=Prod.scalar.list))))
}
##' @requirement M and N needs to be compatible in dimensions
##' In the case of nrow >> ncol
##' 
##' @title Parallel multiplication M*N
##' @param cl 
##' @param M 
##' @param N 
##' @return 
##' @author Mohammad
P.Mult.Matrix.Matrix <- function(cl,M,N){
    m <- dim(M)[1]
    n <- dim(N)[2]
    MN <- matrix(nrow=m,ncol=n)
    for(j in 1:n){
        MN[,j] <- P.Mult.Matrix.Vect(cl=cl,M=M,V=as.vector(N[,j]))
    }
    return(MN)
}

Prod.scalar.list <- function(L){
    return (sum(L[[1]]*L[[2]]))
}
