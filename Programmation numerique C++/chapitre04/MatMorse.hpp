#include <cassert>
#include <fstream>
#include "RNM.hpp"
#include "sfem.hpp"
#include "HeapSort.hpp"

class MatriceMorseSymetrique { public:
 int n,m; 
 int nbcoef;
 KN<int> colonne, ligne;
 KN<R> a;
 
 MatriceMorseSymetrique::MatriceMorseSymetrique(const  Mesh & Th);
};
