/*** Example of master and single threads. Number of threads by clause. ***/
// gcc OpenMP2.c -fopenmp

#include<stdio.h>
#include<omp.h>

int main(){
  printf(" Number of processors: %d\n",omp_get_num_procs());
  printf(" Number of threads in sequential phase: %d\n",omp_get_num_threads());
  #pragma omp parallel num_threads(5)
    {
    #pragma omp master
      printf(" I am the master (No%d).\n",omp_get_thread_num());
    #pragma omp single
      {
      printf(" I am one of the %d threads (No%d).\n",omp_get_num_threads(),omp_get_thread_num());
      }
    printf(" I am the thread No%d.\n",omp_get_thread_num());
    }
  printf(" I am alone. Bye!\n");
  return 0;
  }

