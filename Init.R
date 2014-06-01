#chargement du package snow
library(snow) 
#pour permettre d'avoir une trace de l'activité du démarrage du noeud
setDefaultClusterOptions('outfile'='/tmp/snow') 
#pour éviter les erreurs de noms sur la machine master 
#- l'adresse IP doit être la bonne - ou alors un FQDN
MyIp='194.167.216.245'
#MyIP='134.214.254.147'
setDefaultClusterOptions('master'=MyIp) 
#pour démarrer un worker sur hml
cl<-makeCluster(c("mini"),type="SOCK") 
cl <- makeSOCKcluster(c("localhost","mini"))
cl <- makeSOCKcluster(c("localhost","localhost"))
#pour afficher le nom de la machine, le login et le type de machine.
clusterCall(cl,function() Sys.info()[c("nodename","user","machine")]) 
# pour stopper le cluster
stopCluster(cl)
# pour démarrer 4 workers sur hml
#cl<-makeCluster(rep("hmlair",4),type="SOCK")

IP1="194.167.216.113"
IP2="194.167.216.72"
setDefaultClusterOptions('homogeneous'=FALSE)
W1<-list(host="localhost",user="mohammad",outfile='/tmp/snow_1')
W2<-list(host="mini",user="mai1332321",outfile='/tmp/snow_2')
W3 <- list(host=IP1,user="p1312311",outfile='/tmp/snow_2')
W4 <- list(host=IP2,user="p1312311",outfile='/tmp/snow_3')
cl<-makeCluster(c(list(W4),list(W3),list(W2)),type="SOCK")
cl<-makeCluster(c(list(W1),list(W2)),type="SOCK")
clusterCall(cl,function() Sys.info()[c("nodename","user","machine")]) 



## Example
myfunc <- function(x=2){x+1}
myfunc_argument <- 5
clusterCall(cl, myfunc, myfunc_argument) 
#finding the host name for each cluster
clusterEvalQ(cl, Sys.getenv("HOST"))
#Loading the boot package on all cluster nodes:
clusterEvalQ(cl, library(boot))
#clusterApply takes a cluster, a sequence of arguments 
#(can be a vector or a list), and a function, and calls the function 
#with the first element of the list on the first node, 
#with the second element of the list on the second node, and so on. 
#The list of arguments must have at most as many elements as there 
#are nodes in the cluster.
clusterApply(cl, 1:4, sum, 3)

## clusterSplit splits 'seq' into one consecutive piece for each cluster and returns
## the result as a list with length equal to the number of cluster nodes.
## The pieces are chosen to be close to equal in length. : clusterSplit(cl, seq)
clusterSplit(cl, 1:6)

## parApply(cl, X, MARGIN, fun, ...)
## X: the array to be used. 
## MARGIN: a vector giving the subscripts which the function will be applied over. '1' indicates rows, '2' indicates columns, 'c(1,2)' indicates rows and columns. 
## fun: the function to be applied. 
## parApply is the parallel version of `the R function apply.

A<-matrix(1:10, 5, 2)
A
parApply(cl, A, 1, sum)




