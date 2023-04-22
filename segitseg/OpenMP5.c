/*** Example of parallel for, critical section and reduction ***/
// gcc OpenMP5.c -fopenmp

#include<stdio.h>
#include<omp.h>

#define N 10000

int main(){
  int i, sum1, sum2, sum3, sum4;
  int A[N];

  for(i=0;i<N;i++)
    A[i] = 1;

  // Wrong result
  sum1 = 0;
  #pragma omp parallel for
    for(i=0;i<N;i++)
      sum1=sum1+A[i];
  printf(" Sum1: %d\n",sum1);

  // Slow
  sum2 = 0;
  #pragma omp parallel for
    for(i=0;i<N;i++)
      #pragma omp critical
        sum2=sum2+A[i];
  printf(" Sum2: %d\n",sum2);

  // Complicated
  sum3 = 0;
  #pragma omp parallel private(i)
    {
    int Psum = 0;
    int ThID = omp_get_thread_num();
    int NPs  = omp_get_num_procs();
    for(i=ThID*N/NPs; i<(ThID+1)*N/NPs; i++)
      Psum += A[i];
    #pragma omp critical
      sum3 += Psum;
    }
  printf(" Sum3: %d\n",sum3);

  // Advanced
  sum4 = 0;
  #pragma omp parallel for reduction(+:sum4)
    for(i=0;i<N;i++)
      sum4=sum4+A[i];
  printf(" Sum4: %d\n",sum4);

  return 0;
  }

