#loading the database
autos <- read.table(file="AUTOS2005subset.txt",row.names=1,header=T,sep="\t")
print(summary(autos))

#fonction pour centrage-r�duction
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

#pond�ration par la fr�quence des indicatrices
PF <- function(x){
  m <- mean(x)
  return(x/sqrt(m))
}

#appliquer la pond�ration sur les indicatrices
autos.disc.pond <- data.frame(lapply(autos.disc,PF))
print(autos.disc.pond)

#donn�es transform�es envoy�es � l'ACP
autos.pour.acp <- cbind(autos.cont,autos.disc.pond)
rownames(autos.pour.acp) <- rownames(autos)
print(round(autos.pour.acp,3))

#acp
library(ade4)
acp.autos <- dudi.pca(autos.pour.acp,center=T,scale=F,scannf=F)

#valeurs propres
print(round(acp.autos$eig,5))

#coordonn�es des variables
print(acp.autos$co[,1:2])
#pour les quanti -> corr�lations
#pour les quali -> additionner le carr� des valeurs 
#pour les modalit�s d'une m�me variable 
#on obtient le carr� du rapport de corr�lation

#on peut obtenir directement les moyennes conditionnelles ici !!!

#coord. acp des modalit�s
moda <- acp.autos$co[6:12,1:2]

#fr�quence des modalit�s
freq.moda <- colMeans(autos.disc)

#calcul des moyennes conditionnelles sur les 2 premiers facteurs
coord.moda <- moda[,1]*sqrt(acp.autos$eig[1]/freq.moda)
coord.moda <- cbind(coord.moda,moda[,2]*sqrt(acp.autos$eig[2]/freq.moda))
print(coord.moda)

#coordonn�es des individus
print(round(acp.autos$li[,1:2],5))

#repr�sentation graphique
plot(-acp.autos$li[,1],acp.autos$li[,2],type="p",xlim=c(-3.5,3.5),ylim=c(-3.5,3.5),main="Projection des individus",xlab="Fact.1 (47%)",ylab="Fact.2 (24%)")
text(-acp.autos$li[,1],acp.autos$li[,2],label=rownames(autos),cex=0.75)
abline(h=0,v=0)

#carr� des corr�lations 1er facteur
r2 <- acp.autos$co[1:5,1]^2

#carr� du rapport de corr�lation, var. qualitatives
eta2 <- NULL
eta2[1] <- sum(acp.autos$co[6:8,1]^2)
eta2[2] <- sum(acp.autos$co[9:10,1]^2)
eta2[3] <- sum(acp.autos$co[11:12,1]^2)

#crit�res � sommer
criteres <- c(r2,eta2)
names(criteres) <- colnames(autos)
print(criteres)

#inertie projet�e
lambda1 <- sum(criteres)
print(lambda1)

#v�rification
print(acp.autos$eig[1])



#chargement du package
library(FactoMineR)
#lancement de la proc�dure
afdm.autos <- FAMD(autos,ncp=2)
#affichage des r�sultats
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
#attribu� � un autre auteur Hill & Smith, 1976
library(ade4)
dudi.autos <- dudi.mix(autos,scannf=F,nf=2)

print(round(dudi.autos$eig,5))
print(round(dudi.autos$co,5))

scatter(dudi.autos,posieig="top",clab.row=0)

#moralit� - on a les m�mes r�sultats globalement - sauf les coordonn�es pour les modalit�s des var. quali
#pour ces indicatrices, il faut diviser par la racine de la valeur propre du facteur les coordonn�es de FactorMineR pour obtenir celles de ADE4
#on retrouve l� la pratiqe de l'ACM [coord.fact = moyenne.cond / racine(v.p.) ]

library(PCAmixdata)
pcamix.autos <- PCAmix(autos[1:5],autos[6:8],ndim=2,graph=T)
#valeurs propres
print(round(pcamix.autos$eig,5))
#corr�lations
print(round(pcamix.autos$quanti.cor,5))
#coord. modalit�s
print(round(pcamix.autos$categ.coord,5))

#rotation varimax
rot.autos <- PCArot(pcamix.autos,dim=2,graph=T)
#valeurs propres
print(round(rot.autos$eig,5))
#corr�lations
print(round(rot.autos$quanti.cor,5))
#coord. modalit�s
print(round(rot.autos$categ.coord,5))

