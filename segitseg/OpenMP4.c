/*** Examples of private and shared variables ***/
// gcc OpenMP4.c -fopenmp

#include<stdio.h>
#include<omp.h>

int main(){
  int a=0, b=0, c=0;
  printf(" Before the parallel region: a=%2d,  b=%3d,  c=%3d\n",a,b,c);
  omp_set_num_threads(100);
  #pragma omp parallel private(a) shared(b)
    {
    int d=0;
    a++; // private (explicitely)      (initially undefined in parallel)
    b++; // shared  (explicitely)      (deadlock is possibe)
    c++; // shared  (declared outside) (deadlock is possibe)
    d++; // private (declared inside)  (exists only here)
    printf(" Within the parallel region: a=%2d,  b=%3d,  c=%3d,  d=%2d\n",a,b,c,d);
    }
  printf(" After the parallel region:  a=%2d,  b=%3d,  c=%3d\n",a,b,c);
  return 0;
  }

