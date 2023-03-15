#include<stdio.h>
#include <stdlib.h>


// 820 elem kell
int main(int argc, char const *argv[])
{
    FILE *f;
    
    double szam;


    f = fopen("szamok.txt","r");
    if (f==NULL)
    {
        fprintf(stderr,"File error!\n");
        return 1;
    }

    for (int i = 0; i < 821; i++)
    {
        fscanf(f,"%f ",&szam);
    }
    

    fclose(f);
    

    return 0;
}
