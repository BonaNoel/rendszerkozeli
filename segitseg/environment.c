/*** Getting and setting environment variable by command line arguments ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NoVar 1

char string[100];

int main(int argc, char *argv[], char *envp[]){
  char *variable, *value;

  // Listing environment variables
  if(argc == 1){
    int i = 0;
    printf(" Environment variables listed by %s:\n",argv[0]);
    while(envp[i]!=NULL){
      printf("\t%s\n",envp[i]);
      i++;
      }
    }

  // Getting the value of environment variables
  if(argc == 2) {
    variable = argv[1];
    value = getenv(variable);
    if(value!=NULL){
      printf(" $%s=%s\n", variable, value);
      }
    else{
      printf(" $%s has no value.\n", variable);
      return NoVar;
      }
    }

  // Setting the value of environment variables
  if(argc == 3) {
    variable = argv[1];
    value = argv[2];
    sprintf(string,"%s=%s",variable,value);
    putenv(string);  // the parameter cannot be local array
    printf(" $%s=%s\n",variable,getenv(variable)); // check
    }

  return EXIT_SUCCESS;
  }
