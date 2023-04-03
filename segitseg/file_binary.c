/*** Example of low-level file usage ***/
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

#define BlockSize 1024

int main(int argc, char *argv[]){
  char buff[BlockSize];
  int in,out;
  int nread;

  // Without 2 arguments: error message
  if(argc<3){
    write(2," Arguments needed!\n  1st argument: origin file\n  2nd argument: copy file\n",73);
    return 1;
    }

  // Opening files
  in  = open(argv[1],O_RDONLY);
  out = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

  // Content copy from input file to output file (reads and writes)
  while((nread=read(in,buff,BlockSize))>0)
    write(out,buff,nread);

  // Closing the files
  close(in);
  close(out);

  return 0;
  }
