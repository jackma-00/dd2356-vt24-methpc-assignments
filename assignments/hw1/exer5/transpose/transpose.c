#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define N 64

double a[N][N];
double b[N][N];

// timer
double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

// basic transposition without code transformations
void transposeBase(){
  int i, j;

  for (i = 0; i < N; ++i)
    for (j = 0; j < N; ++j)
      b[i][j] = a[j][i];

}


void initialize_matrices(){
  int i, j ;
 
  for (i = 0 ; i < N ; i++) {
    for (j = 0 ; j < N ; j++) {
      a[i][j] = (double) rand() / RAND_MAX;
      b[i][j] = (double) rand() / RAND_MAX;
    }
  }


}

int main( int argc, char *argv[] )
{
    
    int maxTest=10;
    int i, j, tests;
    int k, rept=10;
    double tStart, tEnd;
    double tLoop, t, rate;

    
    printf( "Transpose %d x %d\n", N, N );

    initialize_matrices();
    
    // Start tests
    tLoop = 1.0e10; // take a large number
    for (tests=0; tests<maxTest; tests++) {
	     
       tStart = mysecond();
	     
       for (k=0; k<rept; k++) {
	        transposeBase();
	     }
       
       tEnd = mysecond();
	     t =  tEnd - tStart;
	     t = t / rept;
       // record the best time for all the different tests
	     if (t < tLoop) tLoop = t;
    }
    rate = 8*(N*N) / tLoop;
    printf( "BaseTime = %.2e s, Base Rate = %.2e MB/s\n", tLoop, rate*1.0e-6);

    // reuse b to avoid smart compiler issues
    for (i = 0 ; i < N ; i++) 
      for (j = 0 ; j < N ; j++) 
        b[i][j]++;
    

    return 0;
}
