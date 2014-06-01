#vider la m�moire
rm(list=ls())

#changement de r�pertoire
setwd("D:/_Travaux/university/Cours_Universite/Supports_de_cours/Informatique/R/Tutoriels/afcm")

#chargement des donn�es
library(xlsReadWrite)
canines <- read.xls(file="races_canines_acm.xls",rowNames=T,sheet=1)
summary(canines)
#distribution de fr�quences (graphique) des variables actives
par(mfrow=c(2,3))
for (j in 1:6){plot(canines[,j],main=colnames(canines)[j])}
layout(1)

#********************
#AFCM avec FactoMineR
#********************

#charger le package
library(FactoMineR)

#lancer l'ACM 
#voir l'aide pour plus de d�tails sur les options
canines.acm <- MCA(canines,ncp=2,quali.sup=c(7),graph=F)
print(canines.acm)
#les valeurs propres
print(canines.acm$eig)
#scree plot
plot(canines.acm$eig[,1],type="b",main="Scree plot")

#coordonn�es, cos2 et contribution des individus
individus <- cbind(canines.acm$ind$coord,canines.acm$ind$cos2,canines.acm$ind$contrib)
colnames(individus) <- c("Coord.1","Coord.2","COS2.1","COS2.2","CTR.1","CTR.2")
print(round(individus,2))
#graphique associ�
plot(individus[,1],individus[,2],main="Projection des observations",xlab="Comp.1",ylab="Comp.2",type="n")
abline(h=0,v=0)
text(individus[,1],individus[,2],labels=rownames(individus),cex=0.75)

#coordonn�es, cos2 et contribution des modalit�s
modalites <- cbind(canines.acm$var$coord,canines.acm$var$cos2,canines.acm$var$contrib,canines.acm$var$v.test)
colnames(modalites) <- c("Comp.1","Comp.2","COS2.1","COS2.2","CTR.1","CTR.2","VT.1","VT.2")
print(round(modalites,2))
#graphique associ�
plot(modalites[,1],modalites[,2],main="Projection des modalit�s",xlab="Comp.1",ylab="Comp.2",type="n")
abline(h=0,v=0)
text(modalites[,1],modalites[,2],labels=rownames(modalites),cex=0.75)

#graphiques automatiques de MCA
plot(canines.acm,cex=0.75,choix="ind")
plot(canines.acm,cex=0.75,choix="var")

#****************************
#repr�sentation barycentrique
#****************************
#composantes principales r�duites pour les individus
f1etoile <- canines.acm$ind$coord[,1]/sqrt(canines.acm$eig[1,1])
f2etoile <- canines.acm$ind$coord[,2]/sqrt(canines.acm$eig[2,1])
#composantes principales non modifi�e pour les modalit�s
g1 <- canines.acm$var$coord[,1]
g2 <- canines.acm$var$coord[,2]
#graphique
plot(c(f1etoile,g1),c(f2etoile,g2),type="n",main="Repr�sentation barycentrique de type (a)")
abline(h=0,v=0)
text(f1etoile,f2etoile,labels=names(f1etoile),cex=0.5)
text(g1,g2,labels=names(g1),cex=0.5,col="red")





