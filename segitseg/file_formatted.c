/*** Example of formatted file usage ***/
#include<stdio.h>

int main(){
  int pos;
  char c, text[128];
  FILE *f;

  // Opening file
  f=fopen("file.txt","w+");
  if(f==NULL){
    fprintf(stderr," There is a file opening problem!\n");
    return 1;
    }

  // Write the file
  fprintf(stdout," What word would you like to write into the file?\n   ");
  fscanf(stdin,"%s",text);
  fprintf(f,"%s",text);

  // Positioning and rewriting
  fprintf(stdout," Let's overwrite something!\n From which character? Give a number!\n   ");
  fscanf(stdin,"%d",&pos);
  fprintf(stdout," What is the new text?\n   ");
  fscanf(stdin,"%s",text);
  fseek(f,pos-1,SEEK_SET);
  fprintf(f,"%s",text);

  // Reading the file
  fprintf(stdout," Now the file content is:\n   ");
  fseek(f,0,SEEK_SET);
  while(1){
    fscanf(f,"%c",&c);
    if(feof(f)) break;
    fprintf(stdout,"%c",c);
    }
  fprintf(stdout,"\n");

  // Closing the file
  fclose(f);

  return 0;
  }
