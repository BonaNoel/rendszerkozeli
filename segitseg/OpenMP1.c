/*** Example of parallel sections/threads ***/
// gcc OpenMP1.c -fopenmp

#include<stdio.h>
#include<omp.h>

int main(){
  #pragma omp parallel sections
    {
    #pragma omp section
      {
      printf("Algeria  (%d).\n",omp_get_thread_num());
      }
    #pragma omp section
      {
      printf("Bulgaria (%d).\n",omp_get_thread_num());
      }
    #pragma omp section
      {
      printf("Croatia  (%d).\n",omp_get_thread_num());
      }
    #pragma omp section
      {
      printf("Dominica (%d).\n",omp_get_thread_num());
      }
    #pragma omp section
      {
      printf("Ethiopia (%d).\n",omp_get_thread_num());
      }
    }
  return 0;
  }

