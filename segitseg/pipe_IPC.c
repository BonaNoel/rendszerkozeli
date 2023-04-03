/*** Example of pipe ***/
#include <stdio.h>
#include <unistd.h>

int main(){
  int Pipe[2];    // File descriptor of pipe ends 
   
  pipe(Pipe);
  if (fork()==0){ // Child process
    int Value;       
    read(Pipe[0], &Value, sizeof(int));
    printf(" Child> The value is %d.\n", Value);
    }
  else{           // Parent process
    int Num;
    printf(" Parent> Write an iteger: ");
    scanf("%d",&Num);
    write(Pipe[1], &Num, sizeof(int));
    }
  return 0;
  }
