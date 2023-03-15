#include <stdlib.h>
#include <time.h>
#include <unistd.h>   
#include <fcntl.h>    
#include <sys/stat.h> 

// 0-1mil között random számokat akarunk irni fáljba
// szamok.txt mentsuk el az 1000db valos szamot

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    float A[1000];
    int f;

    f = open("nums.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (f < 0)
    {
        write(2, "File error!\n",12);
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < 1000; i++)
    {
        A[i] = (float)rand() / RAND_MAX * 1000000.0;
    }

    // for (int i = 0; i < 1000; i++)
    // {
    //     write(f,&A[i],sizeof(A[i]));    
    // }

    write(f,A,sizeof(A));

    close(f);
    return 0;
}
