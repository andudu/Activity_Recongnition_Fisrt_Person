#include <iostream>

using namespace std;

const int N = 3,  //state number 
          M = 3,  //observation number
          T = 15; //

double  pi[N],    //initial probability 
        a[N][N],  //transition probability
        b[N][M];  //emission probability

int main (int argc, const char * argv[]){

  cout <<  "HMM DIY\n";  

  return 0;
} 

/*
q: state sequence
o: observation sequence
*/ 
double probability(int* q, int* o, int T)
{
    double p = pi[q[0]] * b[q[0]][o[0]];
    for (int i=1; i<T; ++i)
        p *= a[q[i-1]][q[i]] * b[q[i]][o[i]];
    return p;
}  