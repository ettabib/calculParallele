library(mixOmics)
library(ade4)
n=nrow(SR0910[,5:10])
# data est initialise par DataNipas.R
# Ici on choisit de ne garder que 2 axes principaux
# Comparaison de resultats avec la fonction nipals de ade4
myNipals<-Nipals(cl=cl,X=data,nf=2,epsilon=0.00001,Iter=10)
N.nipals<-dudi.pca(SR0910[,5:10],center=TRUE, scale=TRUE, scannf=FALSE, nf=3)

# Valeurs propres
N.nipals$eig
myNipals$eig
norm(N.nipals$eig[1:2]-myNipals$eig,type="2")

# Correlation des axes principaux
cor(N.nipals$c1[,1],myNipals$c1[,1])
cor(N.nipals$c1[,2],myNipals$c1[,2])
norm(N.nipals$c1-myNipals$c1,type="2")

# Correlation des composantes principales
cor(N.nipals$li[,1],myNipals$li[,1])
cor(N.nipals$li[,2],myNipals$li[,2])
# en Graphique 
plot(N.nipals$li[,1],myNipals$li[,1])
plot(N.nipals$li[,2],myNipals$li[,2])


par(mfrow=c(1,2))
barplot(N.nipals$eig)
barplot(myNipals$eig)
pc.cr <- princomp(SR0910[,5:10])


## Cercle des correlations
s.corcircle(N.nipals$c1, xax = 1, yax = 2)
s.corcircle(myNipals$c1, xax = 1, yax = 2)
##le premier axe correspond aux individus blesses le deuxieme au morts


s.label(N.nipals$li,label=SR0910$numdep,clabel=0.75)
s.label(myNipals$li,label=SR0910$numdep,clabel=0.75)

popu <- scale(SR0910$population,center=TRUE,scale=TRUE)
s.value(N.nipals$li[,1:2],popu)
s.value(myNipals$li[,1:2],popu)





######### Comparaison avec Nipals sans donnees manquantes
myNipals.sdm <- Nipals.sdm(cl=cl,X=SR0910[,5:10],nf=3,epsilon=0.00001)


# Valeurs propres
N.nipals$eig[1:2]
myNipals.sdm$eig
norm(N.nipals$eig[1:2]-myNipals.sdm$eig,type="2")

# Correlation des axes principaux
cor(N.nipals$c1[,1],myNipals.sdm$c1[,1])
cor(N.nipals$c1[,2],myNipals.sdm$c1[,2])


# Correlation des composantes principales
cor(N.nipals$li[,1],myNipals.sdm$li[,1])
cor(N.nipals$li[,2],myNipals.sdm$li[,2])

par(mfrow=c(1,2))
barplot(N.nipals$eig)
barplot(myNipals.sdm$eig)
pc.cr <- princomp(SR0910[,5:10])


## Cercle des correlations
s.corcircle(N.nipals$c1, xax = 1, yax = 2)
s.corcircle(myNipals.sdm$c1, xax = 1, yax = 2)
##le premier axe correspond aux individus blesses le deuxieme au morts


s.label(N.nipals$li,label=SR0910$numdep,clabel=0.75)
s.label(myNipals.sdm$li,label=SR0910$numdep,clabel=0.75)

popu <- scale(SR0910$population,center=TRUE,scale=TRUE)
s.value(N.nipals$li[,1:2],popu)
s.value(myNipals.sdm$li[,1:2],popu)


