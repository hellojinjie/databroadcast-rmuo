#ifndef UTIL_H_
#define UTIL_H_

#include <list>
#include "SimpleRequest.h"

using namespace std;

typedef struct
{
    /* Value range is all integers numbers from 1 to N, bounds included. */
    int N;
    /* Skewedness of the distribution.  Must be zero for uniform
     distributions, or positive for increasing skewedness. */
    double a;
    /* Values computed once for all random numbers for the given `N' and `a'. */
    double c1, c2;
} zipf_t;

void zipf_init(int N, double a, zipf_t *z);

int zipf(zipf_t z);

/**
 * 返回一个整数，这个整数服从min和max之间的均匀分布
 * 均匀分布虽简单，但是写出一正确的均匀分布却并不简单
 */
int uniform(int min, int max);

bool isInList(list<int> l, int t);

bool readSetSizeComparsion(SimpleRequest r1, SimpleRequest r2);

#endif // UTIL_H_
