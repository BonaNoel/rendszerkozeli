/*** Pseudo-random numbers ***/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define N 1000000

int main(){
  int i, ri;
  double rf;
  int a[6] = {0,0,0,0,0,0};
  int b[6] = {0,0,0,0,0,0};

  srand(time(NULL)); // initialization of generator
  //rand();  // necessary only on Windows

  // integer numbers (int)
  printf("0 <= %d <= %d\n",rand(),RAND_MAX);
  printf("123 <= %d <= 456\n",rand()%(456-123+1)+123);

  // real number (float)
  printf("0.000000 <= %f <=1.000000\n",(float)rand()/RAND_MAX);
  printf("6.250000 <= %f <=9.876500\n",(float)rand()/RAND_MAX*(9.8765-6.25)+6.25);

  // Equal chance
  printf("\nRegular dice (%d throw):\n",N);
  for(i=0;i<N;i++){
    do
      ri = rand();
    while(ri>=RAND_MAX-(RAND_MAX%6));  // To ensure equal probability
    a[ri%6]++;
    }
  for(i=0;i<6;i++)
    printf(" %d:\t%.4f%%\n",i+1,(float)a[i]/N*100);

  // Not equal chance
  printf("\nTricky dice (%d throw):\n",N);
  for(i=0;i<N;i++){
    rf = (double)rand()/((unsigned)RAND_MAX+1); // 0.0 <= y < 1.0
    if(rf<0.2) b[0]++;  //20%
    else       b[(int)((rf-0.2)/((1.0-0.2)/5))+1]++;
    }
  for(i=0;i<6;i++)
    printf(" %d:\t%.4f%%\n",i+1,(float)b[i]/N*100);

  return 0;
  }
