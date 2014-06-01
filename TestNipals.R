library(mixOmics)
library(ade4)
# data est initialise par DataNipas.R
# Ici on choisit de ne garder que 2 axes principaux
# Comparaison de resultats avec la fonction nipals de ade4
myNipals<-Nipals(cl=cl,X=data,r=2,epsilon=0.00001,Iter=1000)
N.nipals<-nipals(df=SR0910[,5:10],nf=2)

# Valeurs propres
N.nipals$eig
myNipals$eig
norm(N.nipals$eig-myNipals$eig,type="2")

# Correlation des axes principaux
cor(N.nipals$c1[,1],myNipals$c1[,1])
cor(N.nipals$c1[,2],myNipals$c1[,2])
norm(N.nipals$c1-myNipals$c1,type="2")

# Correlation des composantes principales
cor(N.nipals$li[,1],myNipals$li[,1])
cor(N.nipals$li[,2],myNipals$li[,2])
norm(N.nipals$li-myNipals$li,type="2")


