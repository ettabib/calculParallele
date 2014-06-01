#vider la mémoire
rm(list=ls())

#changement de répertoire
setwd("D:/_Travaux/university/Cours_Universite/Supports_de_cours/Informatique/R/Tutoriels/afc")

#chargement des données
library(xlsReadWrite)
media <- read.xls(file="media_prof_afc.xls",rowNames=T,sheet=1)
print(media)

#fonction : calcul du profil
profil <- function(x){x/sum(x)*100}

#profil ligne
prof.ligne <- t(apply(media,1,profil))
print(round(prof.ligne,1))

#profil colonne
prof.colonne <- apply(media,2,profil)
print(round(prof.colonne,1))

#*******************
#AFC avec FactoMineR
#*******************

#charger le package
library(FactoMineR)

#lancer l'AFC
media.afc <- CA(media,ncp=2,graph=FALSE)

#liste des objets disponibles
print(attributes(media.afc))

#quelques statistiques sur le tableau de données
print(media.afc$call)

#tableau des valeurs propres et Scree plot
print(media.afc$eig)
plot(media.afc$eig[,1],type="b",main="Scree plot")

#coordonnées, contributions et cos2 - lignes
print(media.afc$row)

#coordonnées, contributions et cos2 - colonnes
print(media.afc$col)

#graphique
plot(media.afc,cex=0.75)
