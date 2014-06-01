#vider la m�moire
rm(list=ls())

#librairie lecture fichier excel
library(xlsReadWrite)

#changement de r�pertoire
setwd("D:/_Travaux/university/Cours_Universite/Supports_de_cours/Informatique/R/Tutoriels/acp")

#chargement des donn�es dans la premi�re feuille de calcul
#premi�re colonne = label des observations
#les donn�es sont dans la premi�re feuille
autos <- read.xls(file="autos_acp_pour_r.xls",rowNames=T,sheet=1)

#qqs v�rifications
#affichage
print(autos)
#statistiques descriptives
summary(autos)
#nuages de points
pairs(autos)

#partition des donn�es (var. actives et illustratives)
autos.actifs <- autos[,1:6]
autos.illus <- autos[,7:9]

#nombre d'observations
n <- nrow(autos.actifs)
print(n)

#*******************************
# ACP avec la proc�dure princomp
#*******************************

#centrage et r�duction des donn�es --> cor = T
#calcul des coordonn�es factorielles --> scores = T
acp.autos <- princomp(autos.actifs, cor = T, scores = T)
#print
print(acp.autos)
#summary
print(summary(acp.autos))
#quelles les propri�t�s associ�es � l'objet
print(attributes(acp.autos))

#*************************
#**** val. propres *******
#*************************

#obtenir les variances associ�es aux axes c.-�-d. les valeurs propres
val.propres <- acp.autos$sdev^2
print(val.propres)

#scree plot (graphique des �boulis des valeurs propres)
plot(1:6,val.propres,type="b",ylab="Valeurs propres",xlab="Composante",main="Scree plot")

#intervalle de confiance des val.propres � 95% (Saporta, page 172)
val.basse <- val.propres * exp(-1.96 * sqrt(2.0/(n-1)))
val.haute <- val.propres * exp(+1.96 * sqrt(2.0/(n-1)))

#tableau
tableau <- cbind(val.basse,val.propres,val.haute)
colnames(tableau) <- c("B.Inf.","Val.","B.Sup")
print(tableau,digits=3)

#*************************************************
#coordonn�es des variables sur les axes factoriels
#*************************************************

#**** corr�lation variables-facteurs ****
c1 <- acp.autos$loadings[,1]*acp.autos$sdev[1]
c2 <- acp.autos$loadings[,2]*acp.autos$sdev[2]
#affichage
correlation <- cbind(c1,c2)
print(correlation,digits=2)

#carr�s de la corr�lation
print(correlation^2,digits=2)

#cumul carr�s de la corr�lation
print(t(apply(correlation^2,1,cumsum)),digits=2)

#*** cercle des corr�lations - variables actives ***
plot(c1,c2,xlim=c(-1,+1),ylim=c(-1,+1),type="n")
abline(h=0,v=0)
text(c1,c2,labels=colnames(autos.actifs),cex=0.5)
symbols(0,0,circles=1,inches=F,add=T)

#***************************************************************
#*** projection des individus dans le premier plan factoriel ***
#***************************************************************

#l'option "scores" demand� dans princomp est tr�s important ici
plot(acp.autos$scores[,1],acp.autos$scores[,2],type="n",xlab="Comp.1 - 74%",ylab="Comp.2 - 14%")
abline(h=0,v=0)
text(acp.autos$scores[,1],acp.autos$scores[,2],labels=rownames(autos.actifs),cex=0.75)

#**************************************************************************************
#*** repr�sentation simultan�e : individus x variables - cf. Lebart et al., pages 46-48
#**************************************************************************************
biplot(acp.autos,cex=0.75)

#**************************************
#*** cosinus carr� des individus actifs
#**************************************

#calcul du carr� de la distance d'un individu au center de gravit�
d2 <- function(x){return(sum(((x-acp.autos$center)/acp.autos$scale)^2))}
#appliquer � l'ensemble des observations
all.d2 <- apply(autos.actifs,1,d2)
#cosinus^2 pour une composante
cos2 <- function(x){return(x^2/all.d2)}
#cosinus^2 pour l'ensemble des composantes retenues (les 2 premi�res)
all.cos2 <- apply(acp.autos$scores[,1:2],2,cos2)
print(all.cos2)

#**************************************
#*** contributions des individus actifs
#**************************************

#contributions � une composante - calcul pour les 2 premi�res composantes
all.ctr <- NULL
for (k in 1:2){all.ctr <- cbind(all.ctr,100.0*(1.0/n)*(acp.autos$scores[,k]^2)/(acp.autos$sdev[k]^2))}
print(all.ctr)

#***************************************
# traitement des variables illustratives
#***************************************

#*******************************************************************************************
#calcul des coordonn�es des variables illustratives continues sur le cercle des corr�lations

#premier axe
ma_cor_1 <- function(x){return(cor(x,acp.autos$scores[,1]))}
s1 <- sapply(autos.illus[,2:3],ma_cor_1)
#second axe
ma_cor_2 <- function(x){return(cor(x,acp.autos$scores[,2]))}
s2 <- sapply(autos.illus[,2:3],ma_cor_2)
#position sur le cercle
plot(s1,s2,xlim=c(-1,+1),ylim=c(-1,+1),type="n",main="Variables illustratives",xlab="Comp.1",ylab="Comp.2")
abline(h=0,v=0)
text(s1,s2,labels=colnames(autos.illus[2:3]),cex=1.0)
symbols(0,0,circles=1,inches=F,add=T)
#repr�sentation simultan�e
plot(c(c1,s1),c(c2,s2),xlim=c(-1,+1),ylim=c(-1,+1),type="n",main="Variables illustratives",xlab="Comp.1",ylab="Comp.2")
text(c1,c2,labels=colnames(autos.actifs),cex=0.5)
text(s1,s2,labels=colnames(autos.illus[2:3]),cex=0.75,col="red")
abline(h=0,v=0)
symbols(0,0,circles=1,inches=F,add=T)

#*******************************************************************************
#positionner les modalit�s de la variable illustrative + calcul des valeurs test
K <- nlevels(autos.illus[,"FINITION"])
var.illus <- unclass(autos.illus[,"FINITION"])
m1 <- c()
m2 <- c()
for (i in 1:K){m1[i] <- mean(acp.autos$scores[var.illus==i,1])}
for (i in 1:K){m2[i] <- mean(acp.autos$scores[var.illus==i,2])}
cond.moyenne <- cbind(m1,m2)
rownames(cond.moyenne) <- levels(autos.illus[,"FINITION"])
print(cond.moyenne)
#graphique
plot(c(acp.autos$scores[,1],m1),c(acp.autos$scores[,2],m2),xlab="Comp.1",ylab="Comp.2",main="Positionnement var.illus cat�gorielle",type="n")
abline(h=0,v=0)
text(acp.autos$scores[,1],acp.autos$scores[,2],rownames(autos.actifs),cex=0.5)
text(m1,m2,rownames(cond.moyenne),cex=0.95,col="red")

#*** calcul des valeurs test ***
#effectifs par modalit�
nk <- as.vector(table(var.illus))
#valeur test par composante (les 2 premi�res)
vt <- NULL
for (j in 1:2){vt <- cbind(vt,cond.moyenne[,j]/sqrt((val.propres[j]/nk)*((n-nk)/(n-1))))}
#affichage des valeurs
tableau <- cbind(cond.moyenne,vt)
colnames(tableau) <- c("Coord.1","Coord.2","VT.1","VT.2")
print(tableau)

#**************************************
# traitement des individus illustratifs
#**************************************

#chargement de la seconde feuille de calcul + v�rification
ind.illus <- read.xls(file="autos_acp_pour_r.xls",rowNames=T,sheet=2)
summary(ind.illus)

#centrage et r�duction en utilisant les moyennes et �carts-type de l'ACP
ind.scaled <- NULL
for (i in 1:nrow(ind.illus)){ind.scaled <- rbind(ind.scaled,(ind.illus[i,] - acp.autos$center)/acp.autos$scale)}
print(ind.scaled)

#calcul des coordonn�es factorielles (en utilisant les valeurs propres cf. loadings)
produit.scal <- function(x,k){return(sum(x*acp.autos$loadings[,k]))}
ind.coord <- NULL
for (k in 1:2){ind.coord <- cbind(ind.coord,apply(ind.scaled,1,produit.scal,k))}
print(ind.coord)

#*** projection des individus actifs ET illustratifs dans le premier plan factoriel ***
plot(c(acp.autos$scores[,1],ind.coord[,1]),c(acp.autos$scores[,2],ind.coord[,2]),xlim=c(-6,6),type="n",xlab="Comp.1",ylab="Comp.2")
abline(h=0,v=0)
text(acp.autos$scores[,1],acp.autos$scores[,2],labels=rownames(autos.actifs),cex=0.5)
text(ind.coord[,1],ind.coord[,2],labels=rownames(ind.illus),cex=0.5,col="red")


