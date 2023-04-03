/***  Example of synchronization by signals  ***/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int isNotice = 0;      // it is 1 when child notice the parent
int isAcknowledge = 0; // it is 1 when parent has a response for child
int isOver = 0;        // it is 1 when child is terminated

void SignalHandler(int sig){
  if(sig==SIGUSR1)
    isNotice = 1;
  if(sig==SIGUSR2)
    isAcknowledge = 1;
  if(sig==SIGCHLD)
    isOver = 1;
  if(sig==SIGINT)
    printf("\n The process is unshootable.\n");
  }

int main(){
  pid_t pid;
  pid = fork();
    int taskID = 1;
  if(pid==0){  // child
    srand(getpid());
    signal(SIGUSR2,SignalHandler);
    signal(SIGINT, SIG_IGN);
    while(rand()%5!=0){
      printf(" Child: I am working on Task #%d.\n",taskID++);
      sleep(rand()%3+1); // Child is working
      kill(getppid(),SIGUSR1);
      while(!isAcknowledge)
        pause();
      isAcknowledge = 0;
      }
    printf(" Child: I am ready.\n");
    }
  else {  // parent
    signal(SIGUSR1,SignalHandler);
    signal(SIGINT, SignalHandler);
    signal(SIGCHLD,SignalHandler);
    printf(" Parent: I am waiting for Child's notice.\n");
    while(1){
      while(!isNotice && !isOver)
        pause();
      if(isOver) break;
      isNotice = 0;
      printf(" Parent: OK. You can go on the Task #%d.\n",++taskID);
      kill(pid,SIGUSR2);
      }
    printf(" Parent: The child process is over. Bye!\n");
    }
  return 0;
  }
