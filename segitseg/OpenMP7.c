/*** Complex example of OpenMP ***/
// gcc OpenMP7.c -fopenmp -lm

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#ifdef _OPENMP
  #include<omp.h>
#else
  #define omp_get_num_procs() 1
#endif

int main(int argc, char* argv[]){
  int nP       = 0;
  int NProcs   = omp_get_num_procs();
  int NThreads = (NProcs==1) ? (1)         : (NProcs-1);
  int Max      = (argc==1)   ? (100000000) : (atoi(argv[1]));
  register int N;
  printf(" Looking for prime numbers on %d threads.\n",NThreads);
  omp_set_num_threads(NThreads);
  #pragma omp parallel for schedule(guided) reduction(+:nP)
  for(N=Max; N>=2; N--){
    register int D, SqrtN;
    SqrtN=(int)sqrt((double)N);
    if(N%2==0 && N!=2) continue;
    for(D=3; (N%D!=0)&&(D<=SqrtN); D+=2);
    if(D>SqrtN) nP++;
    }
  printf(" There are %d prime numbers below %d.\n",nP,Max);
  return 0;
  }

