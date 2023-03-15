#include <stdio.h>
#include <stdlib.h>

typedef struct ize
{
    int adat;
    struct ize *bal, *jobb;
} ELEM;

void beszur(int d, ELEM **H) // elem beszuras
{
    if (*H == NULL) // ha ures a fa
    {
        *H = calloc(1, sizeof(ELEM));
        (**H).adat = d;
    }
    else
    {
        if ((**H).adat > d) // beszuras ballra ||rekurzivan||
        {
            beszur(d, (&(**H).bal));
        }
        else // beszuras jobbra
        {
            beszur(d, (&(**H).jobb));
        }
    }
}

void inorder(ELEM *H)
{
    if (H != NULL)
    {
        inorder((*H).bal);
        printf("%d ", (*H).adat);
        inorder((*H).jobb);
    }
}

int main()
{
    ELEM *FEJ = NULL;
    for (int i = 0; i < 10; i++)
    {
        beszur(rand() % 1000, &FEJ);
    }

    inorder(FEJ);

    return 0;
}
