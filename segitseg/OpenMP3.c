/*** Example of synchronisation ***/
// gcc OpenMP3.c -fopenmp

#include<stdio.h>
#include<unistd.h>
#include<omp.h>

int main(){
  #pragma omp parallel
    {
    sleep(omp_get_thread_num());
    printf(" No%d is present.\n",omp_get_thread_num());
    #pragma omp barrier  // Synchronisation
    #pragma omp master
      printf(" OK, everyone is here.\n");
    }
  return 0;
  }

