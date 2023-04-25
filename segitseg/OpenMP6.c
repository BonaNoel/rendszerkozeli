/*** Example of scheduling parallel for ***/
// gcc OpenMP6.c -fopenmp

#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define N 107

int main(){
  int i;
  int A[N];
  int C[4]={0,0,0,0};
  omp_set_num_threads(4);
//  #pragma omp parallel for 
  #pragma omp parallel for schedule(static,1)
//  #pragma omp parallel for schedule(static,5)
//  #pragma omp parallel for schedule(dynamic,1)
//  #pragma omp parallel for schedule(dynamic,5)
//  #pragma omp parallel for schedule(guided)
    for(i=0;i<N;i++){
      int j,ID;
      ID=omp_get_thread_num();
      A[i]=ID;
      C[ID]++;
      for(j=0;j<i;j++);
      }
  for(i=0;i<N;i++){
    printf(" %d",A[i]);
    if(i%10==9) printf("\n");
    }
  printf("\n");
  for(i=0;i<4;i++)
    printf(" Thread %d was active %d times.\n",i,C[i]);
  return 0;
  }

