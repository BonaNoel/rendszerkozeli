#include <stdio.h>

void int2bin(int N)
{
    unsigned int M = 1 << (sizeof(int) * 8) - 1; // 1<<31

    while (M)
    {
        // printf("%d", N & M == 0 ? 0 : 1);
        printf("%d", N & M ? 1 : 0);

        // M = M >> 1;
        M >>= 1;
    }

    printf("\n");
}

int bin_inc(int N)
{

    int M = 1;

    while (N & M) // N&? !=0
    {
        // N=N&~M;
        N &= ~M;
        M <<= 1; // M = M<<1
    }

    return M | N;
}

// szorozzon -1-el
int pupu(int N)
{
    return bin_inc(~N);
}


int plus(int A, int B){
    // O = (A^B)^C
    // C = (A&B)|((A^c)&B)
}


int main(int argc, char const *argv[])
{
    int2bin(5);

    // int2bin(bin_inc(5));

    int2bin(pupu(5));
    return 0;
}
