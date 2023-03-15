#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int **Values;
    **Values = calloc(1,(sizeof(Values))*5);
    

    int x = 0;
    for (int i = 0; i < 5; i++)
    {
        x++;
        *(*Values+i) = x;
        printf("%d\n", *(*Values+i));
    }
    printf("pupu2");
    


    free(Values);
    return 0;
}
