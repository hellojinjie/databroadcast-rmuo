/*
 * util.cpp
 *
 *  Created on: 2011-1-12
 */

#include "util.h"
#include <cstdlib>
#include <cmath>

/* Allocate a `zipf_t' and precompute values into it. */
void zipf_init(int N, double a, zipf_t *z)
{
    z->N = N;

    if (fabs(a) < 0.0001)
        z->a = 0;
    else if (fabs(a - 1) < 0.0001)
    {
        z->a = 1;
        z->c1 = log(N + 1);
    }
    else
    {
        z->a = a;
        /* Below pow(x,y) has been rewritten to exp(y*log(x)) in order to
         allow non-integer y. */
        z->c1 = exp((1 - a) * log(N + 1)) - 1;
        z->c2 = 1 / (1 - a);
    }
}

int zipf(zipf_t z)
{
    int r;
    double x;

    if (z.a == 0)
        return 1 + (rand() % z.N);
    do
    {
        x = rand() / (double) RAND_MAX;
        if (z.a == 1)
            x = exp(x * z.c1);
        else
            x = exp(z.c2 * log(x * z.c1 + 1));
        r = (int) x;
    } while (r <= 1 || r > z.N); /* Hedging. */
    return r;
}

int uniform(int min, int max)
{
    int r = rand() % (max - min + 1);

    return min + r ;
}
