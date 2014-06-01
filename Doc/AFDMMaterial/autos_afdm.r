#loading the database
autos <- read.table(file="AUTOS2005subset.txt",row.names=1,header=T,sep="\t")
print(summary(autos))

#fonction pour centrage-réduction
CR <- function(x){
  n <- length(x)
  m <- mean(x)
  v <- (n-1)/n*var(x)
  return((x-m)/sqrt(v))
}

#appliquer la fonction sur les variables continues
autos.cont <- data.frame(lapply(subset(autos,select=1:5),CR))
print(autos.cont)

library(ade4)
#codage disjonctif complet
autos.disc <- acm.disjonctif(subset(autos,select=6:8))

#pondération par la fréquence des indicatrices
PF <- function(x){
  m <- mean(x)
  return(x/sqrt(m))
}

#appliquer la pondération sur les indicatrices
autos.disc.pond <- data.frame(lapply(autos.disc,PF))
print(autos.disc.pond)

#données transformées envoyées à l'ACP
autos.pour.acp <- cbind(autos.cont,autos.disc.pond)
rownames(autos.pour.acp) <- rownames(autos)
print(round(autos.pour.acp,3))

#acp
library(ade4)
acp.autos <- dudi.pca(autos.pour.acp,center=T,scale=F,scannf=F)

#valeurs propres
print(round(acp.autos$eig,5))

#coordonnées des variables
print(acp.autos$co[,1:2])
#pour les quanti -> corrélations
#pour les quali -> additionner le carré des valeurs 
#pour les modalités d'une même variable 
#on obtient le carré du rapport de corrélation

#on peut obtenir directement les moyennes conditionnelles ici !!!

#coord. acp des modalités
moda <- acp.autos$co[6:12,1:2]

#fréquence des modalités
freq.moda <- colMeans(autos.disc)

#calcul des moyennes conditionnelles sur les 2 premiers facteurs
coord.moda <- moda[,1]*sqrt(acp.autos$eig[1]/freq.moda)
coord.moda <- cbind(coord.moda,moda[,2]*sqrt(acp.autos$eig[2]/freq.moda))
print(coord.moda)

#coordonnées des individus
print(round(acp.autos$li[,1:2],5))

#représentation graphique
plot(-acp.autos$li[,1],acp.autos$li[,2],type="p",xlim=c(-3.5,3.5),ylim=c(-3.5,3.5),main="Projection des individus",xlab="Fact.1 (47%)",ylab="Fact.2 (24%)")
text(-acp.autos$li[,1],acp.autos$li[,2],label=rownames(autos),cex=0.75)
abline(h=0,v=0)

#carré des corrélations 1er facteur
r2 <- acp.autos$co[1:5,1]^2

#carré du rapport de corrélation, var. qualitatives
eta2 <- NULL
eta2[1] <- sum(acp.autos$co[6:8,1]^2)
eta2[2] <- sum(acp.autos$co[9:10,1]^2)
eta2[3] <- sum(acp.autos$co[11:12,1]^2)

#critères à sommer
criteres <- c(r2,eta2)
names(criteres) <- colnames(autos)
print(criteres)

#inertie projetée
lambda1 <- sum(criteres)
print(lambda1)

#vérification
print(acp.autos$eig[1])



#chargement du package
library(FactoMineR)
#lancement de la procédure
afdm.autos <- FAMD(autos,ncp=2)
#affichage des résultats
print(summary(afdm.autos))
#tables provided by AFDM
print(afdm.autos$eig)
print(afdm.autos$var)
print(afdm.autos$quali.var)
print(afdm.autos$quanti.bar)
print(afdm.autos$ind)
#graphical representation of instances for the first two factors
plot(afdm.autos$ind$coord[,1],afdm.autos$ind$coord[,2],type="n")
text(afdm.autos$ind$coord[,1],afdm.autos$ind$coord[,2],labels=rownames(autos),cex=0.35)
abline(h=0,v=0)



#ADE4
#attribué à un autre auteur Hill & Smith, 1976
library(ade4)
dudi.autos <- dudi.mix(autos,scannf=F,nf=2)

print(round(dudi.autos$eig,5))
print(round(dudi.autos$co,5))

scatter(dudi.autos,posieig="top",clab.row=0)

#moralité - on a les mêmes résultats globalement - sauf les coordonnées pour les modalités des var. quali
#pour ces indicatrices, il faut diviser par la racine de la valeur propre du facteur les coordonnées de FactorMineR pour obtenir celles de ADE4
#on retrouve là la pratiqe de l'ACM [coord.fact = moyenne.cond / racine(v.p.) ]

library(PCAmixdata)
pcamix.autos <- PCAmix(autos[1:5],autos[6:8],ndim=2,graph=T)
#valeurs propres
print(round(pcamix.autos$eig,5))
#corrélations
print(round(pcamix.autos$quanti.cor,5))
#coord. modalités
print(round(pcamix.autos$categ.coord,5))

#rotation varimax
rot.autos <- PCArot(pcamix.autos,dim=2,graph=T)
#valeurs propres
print(round(rot.autos$eig,5))
#corrélations
print(round(rot.autos$quanti.cor,5))
#coord. modalités
print(round(rot.autos$categ.coord,5))

