#include <stdio.h>
#include <stdlib.h>

#define DSFMT_MEXP 19937
#define HAVE_SSE2 1

#include "dSFMT.h"

int main(int argc, char* argv[])
{
    int i, cnt, seed;
    double x, y, pi;
    const int NUM = 100000000;
    dsfmt_t dsfmt;

    seed = 12345;
    cnt = 0;

    dsfmt_init_gen_rand(&dsfmt, seed);
    //srand(seed);
    //double norm = 1. / ((double)RAND_MAX);

    for (i = 0; i < NUM; i++)
    {
        x = dsfmt_genrand_close_open(&dsfmt);
        y = dsfmt_genrand_close_open(&dsfmt);
        //x = ((double)rand()) * norm;
        //y = ((double)rand()) * norm;
        if (x * x + y * y < 1.0)
        {
            cnt++;
        }
    }

    pi = (double)cnt / NUM * 4;
    printf("%f\n", pi);

    return 0;
}
