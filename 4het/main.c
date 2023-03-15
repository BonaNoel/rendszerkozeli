#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// 0-1mil között random számokat akarunk irni fáljba
// szamok.txt mentsuk el az 1000db valos szamot

/* generate a random floating point number from min to max */
// double randfrom(double min, double max) // 2.feladat.
// {
//     double range = (max - min);
//     double div = RAND_MAX / range;
//     return min + (rand() / div);
// }





// int main(int argc, char const *argv[])
// {
//     srand(time(NULL));
//     FILE *f;

//     f = fopen("szamok.txt","w");
//     if(f==NULL){
//         fprintf(stderr,"File error!\n");
//         return 1;
//     }

//     double szamok[1000];
//     for (int i = 0; i < 1000; i++)
//     {
//         szamok[i] = randfrom(0,1000000);
//     }
    

//     for (int i = 0; i < 1000; i++)
//     {
//         fprintf(f,"%f ",szamok[i]);
//     }
    
//     fclose(f);
//     return 0;
// }


int main(){
    int i;
    float A[1000];

    FILE *f;
    srand(time(NULL));
    for (int i = 0; i < 1000; i++)
    {
        A[i] = (float)rand()/RAND_MAX*1000000.0;
    }


    f=fopen("szamok.txt","w");
    if (f==NULL)
    {
        fprintf(stderr,"File error!\n");
        return 1;
    }

    for (int i = 0; i < 1000; i++)
    {
        fprintf(f,"%f ",A[i]);
    }
    
    fclose(f);
   
   
    return 0;
    
   
}