/*
http://www.csie.ntnu.edu.tw/~u91029/HiddenMarkovModel.html
*/

#include <iostream>
#include <iomanip>

using namespace std;

const int N = 3,  //state number 
          M = 3,  //observation number
          T = 15; //number of states traversed

double  pi[N],    //initial probability 
        a[N][N],  //transition probability
        b[N][M];  //emission probability

double forward_prob[T][N]; //forward matrix 每一列代表在time t,如果位於state n的最大機率
int path_trace[T][N]; //每一列代表在time t ,最大機率路線的 argmax_i[ forward[t-1][i] × a[i][j] ] × b[i][o[t]]

double decode(int* o, int T, int* q)
{   
    //Fill the forward_prob table
    for (int t=0; t<T; ++t){
      for (int j=0; j<N; ++j){

        if (t == 0){
          //initial state
          forward_prob[t][j] = pi[j] * b[j][o[t]];
        }else{
          //normal state
          double p = -1e9;
          //find the maximum transition prob comes from last time t
          for (int i=0; i<N; ++i)
          {
            double w = forward_prob[t-1][i] * a[i][j];
            if (w > p){
              p = w;
              path_trace[t][j] = i;
            }
          }
          forward_prob[t][j] = p * b[j][o[t]];
        }
      }
    }

    //Find the max prob in the last row, which is the over all best path
    double p = -1e9;
    for (int j=0; j<N; ++j){
      if (forward_prob[T-1][j] > p){
        p = forward_prob[T-1][j], q[T-1] = j; 
      }            
    }        
    
    //Trace back to find the best state sequence
    for (int t=T-1; t>0; --t){
      q[t-1] = path_trace[t][q[t]];
    }
 
    return p;
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

int main (int argc, const char * argv[]){

  cout <<  "HMM DIY\n";  

  //隨便設參數
  pi[0] = 0.3;
  pi[1] = 0.3;
  pi[2] = 0.4;

  for(int i = 0 ; i < N ; i++){
    a[i][0] = 0.4;
    a[i][1] = 0.3;
    a[i][2] = 0.3;
    b[i][0] = 0.4;
    b[i][1] = 0.3;
    b[i][2] = 0.3;
  }

  int q[] = {0,1,2,0,1,2};
  int o[] = {0,1,2,0,1,2};

  cout <<probability(q,o,6) << endl;

  return 0;
}