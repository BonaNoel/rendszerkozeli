/*** Example of pipe as a FIFO ***/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int IsPrime(int N){
  int d;
  for(d=2;d<N-1;d++)
    if(N%d==0)                // not prime
      return 0;
  return 1;                   // prime  
  }

int main(){
  int Pipe[2];                // File descriptor of pipe ends 
  int Num, Div, Q_length=0;

  srand(getpid());
  pipe(Pipe);                 // Create a pipe (as a queue)
  printf("Please, enter a positive integer: ");
  scanf("%d",&Num);
  write(Pipe[1], &Num, sizeof(int));
  Q_length++;
  while(Q_length>0){
    read(Pipe[0], &Num, sizeof(int));
    Q_length--;
    if(IsPrime(Num))
      printf("%d\n",Num);
    else{
      do
        Div=rand()%((Num-1)-2+1)+2;  // 2 <= Div <= Num-1
      while(Num%Div!=0);
      write(Pipe[1], &Div, sizeof(int));
      Div=Num/Div;
      write(Pipe[1], &Div, sizeof(int));
      Q_length+=2;
      }
    }
  return 0;
  }

