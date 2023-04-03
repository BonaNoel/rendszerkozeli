/*** Example of inode reading ***/
#include<stdio.h>
#include<sys/stat.h>
#include<time.h>
#include<pwd.h>

int main(int argc, char* argv[]){
  int tmp;
  struct stat inode;
  struct passwd *pwd;

  if(argc==1){
    fprintf(stderr," File or directory name is necessary.\n");
    return 1;
    }

  // Getting inode information
  tmp = stat(argv[1],&inode);
  if(tmp<0){
    fprintf(stderr," Bad name!\n");
    return 2;
    }

  // Interpretation and printing fields
  if(inode.st_mode&S_IFDIR)
    printf(" \"%s\" is a directory.\n",argv[1]);
  if(inode.st_mode&S_IFREG)
    printf(" \"%s\" is a file.\n",argv[1]);
/*  // Proffessional solution
  switch (inode.st_mode & S_IFMT) {
    case S_IFREG:  printf(" regular file\n");     break;
    case S_IFDIR:  printf(" directory\n");        break;
    case S_IFBLK:  printf(" Block device\n");     break;
    case S_IFCHR:  printf(" Character device\n"); break;
    case S_IFSOCK: printf(" Socket\n");           break;
    case S_IFIFO:  printf(" FIFO/pipe\n");        break;
    case S_IFLNK:  printf(" link\n");             break;
    }*/
  printf(" Inode number:\t%d\n",(int)inode.st_ino);
  printf(" Rights:\t");
  printf("%c",(inode.st_mode&S_IRUSR)?'r':'-');
  printf("%c",(inode.st_mode&S_IWUSR)?'w':'-');
  printf("%c",(inode.st_mode&S_IXUSR)?'x':'-');
  printf("%c",(inode.st_mode&S_IRGRP)?'r':'-');
  printf("%c",(inode.st_mode&S_IWGRP)?'w':'-');
  printf("%c",(inode.st_mode&S_IXGRP)?'x':'-');
  printf("%c",(inode.st_mode&S_IROTH)?'r':'-');
  printf("%c",(inode.st_mode&S_IWOTH)?'w':'-');
  printf("%c",(inode.st_mode&S_IXOTH)?'x':'-');
  printf("\n Link count:\t%d\n",(int)inode.st_nlink);
  printf(" Size:\t\t%d\n",(int)inode.st_size);
  printf(" Blocksize:\t%dbytes\n",(int)inode.st_blksize);
  printf(" Block number:\t%d\n",(int)inode.st_blocks);
  printf(" Last status change:\t%s",ctime(&(inode.st_ctime)));
  printf(" Last access:\t\t%s",ctime(&(inode.st_atime)));
  printf(" Last modification:\t%s",ctime(&(inode.st_mtime)));
  pwd = getpwuid(inode.st_uid);
  if(pwd) printf(" Owner's username:\t%s\n", (*pwd).pw_name);

  return 0;
  }
