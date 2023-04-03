/*** Example of arbitrary size array with dynamic memory allocation ***/
#include<stdio.h>
#include<malloc.h>

float average(int *A, int N){
  int i, sum=0;
  for(i=0;i<N;i++)
    sum+=A[i];
  return (float)sum/N;
  }

int main(){
  int i = -1;
  int Size = 1;
  int *Data;

  printf(" Give integers until 0.\n");
  Data = (int*)malloc(sizeof(int));
  do{
    i++;
    if(i==Size){ // If array is full
      Size *= 2;
      Data = (int*)realloc(Data,sizeof(int)*Size);
    }
    printf("  No %d: ",i+1);
    scanf("%d",&Data[i]);
    }
  while(Data[i]!=0);
  Size = i;
  Data = (int*)realloc(Data,sizeof(int)*Size);

  printf(" Average of all number: %f\n",average(Data,Size));
  printf(" Average of the second half: %f\n",average(&Data[Size/4],Size/2));

  free(Data);

  return 0;
  }
