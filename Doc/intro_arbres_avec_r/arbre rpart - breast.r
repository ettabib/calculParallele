rm(list=ls())
#package pour la lecture des fichiers XLS
library(xlsx)
#�chantillon d'apprentissage
breast.app <- read.xlsx(file="breast.xls",sheetIndex=1,header=T)
print(summary(breast.app))
#�chantillon test
breast.test <- read.xlsx(file="breast.xls",sheetIndex=2,header=T)
print(summary(breast.test))
#package 'rpart' pour l'induction des arbres
library(rpart)
#arbre de d�cision avec param�tres par d�faut
arbre.1 <- rpart(classe ~ ., data=breast.app, method="class")
print(arbre.1)
plot(arbre.1)
text(arbre.1)
#pr�diction sur l'�chantillon test
pred.classe <- predict(arbre.1,newdata=breast.test,type="class")
print(summary(pred.classe))
#matrice de confusion
mc <- table(breast.test$classe,pred.classe)
print(mc)
#taux d'erreur
erreur <- (mc[2,1]+mc[1,2])/sum(mc)
print(erreur)
#modifier les param�tres d'induction
parametres <- rpart.control(minsplit=50, minbucket=20)
#induction avec nouveaux param�tres
arbre.2 <- rpart(classe ~ ., data=breast.app, method="class",control=parametres)
print(arbre.2)
#package 'tree' pour l'induction des arbres
library(tree)
param.3 <- tree.control(nobs=nrow(breast.app), mincut=20, minsize=50)
arbre.3 <- tree(classe ~ ., data=breast.app,control=param.3)
print(arbre.3)
#package 'party' pour les arbres de d�cision
library(party)
param.4 <- ctree_control(minsplit=20,minbucket=10)
arbre.4 <- ctree(classe ~ ., data = breast.app, controls=param.4)
plot(arbre.4)
