library(ade4)
SR0910 <- read.table("http://pbil.univ-lyon1.fr/R/donnees/SecRoutiere0910.txt",header=T)

# on choisit ici d'enlever aleatoirement N.NA donnees
data=as.matrix(SR0910[,5:10])
N.NA <- 5
for(i in 1:N.NA){
    rand.y <- floor(runif(n=1,min=1,max=6))
    rand.x <- floor(runif(n=1,min=1,max=100))
    data[rand.x,rand.y] <- NA
}
